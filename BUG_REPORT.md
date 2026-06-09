# Bug Report — CppMobileFramework (feature/smart-garbage-collection)

Audit date: 2026-06-09 | Branch: `feature/smart-garbage-collection` | HEAD: `805a3f2`

## Summary

| Severity | Count | Area |
|----------|-------|------|
| Critical | 2 | GC mutex design, iteration safety |
| High | 5 | Thread safety, TryLock misuse, const-correctness |
| Medium | 4 | Locking gaps, static state, silent operation drops |
| Low | 2 | Fragile API contracts, UB in utility code |

**Root cause pattern**: Non-recursive `std::mutex` combined with pervasive `TryLock`-and-skip-silently creates a system where operations are silently dropped when locks are already held — particularly during garbage collection cycles. This is the likely cause of the crashes mentioned in commit messages.

---

## Critical

### BUG-1: DestroyQueued iterates _destructionQueue without proper locking during CollectGarbage

**File**: [ObjectCollector.cpp:258-301](Core/Modules/Core/Sources/Object/Collector/ObjectCollector.cpp#L258-L301)
**Severity**: **Critical** — Data race / UB

**Description**:
`CollectGarbage()` acquires `_destructionQueueCriticalSection` via `Lock()` (line 265), then calls `DestroyQueued()` (line 284). `DestroyQueued()` attempts `TryLock()` on the SAME mutex (line 293). Since `std::mutex` is non-recursive, `try_lock()` on an already-owned mutex is implementation-defined — on Windows/MSVC it returns `false`. With `_bLocked = false`, the function body executes **without holding the lock**:

```cpp
// CollectGarbage (line 264-265): ACQUIRES both locks
SScopeLock ObjectsLock(_objectsCriticalSection);
SScopeLock DestructionLock(_destructionQueueCriticalSection); // LOCK HELD
// ...
DestroyQueued();  // line 284

// DestroyQueued (line 291-301): TRIES to lock — FAILS silently
void CObjectCollector::DestroyQueued()
{
    SScopeLock DestructionLock(_destructionQueueCriticalSection, true); // TRY LOCK → FAILS
    // Body runs WITHOUT lock
    for (CObject* Object : _destructionQueue) // UNSAFE iteration
    {
        DestroyObject(Object);
    }
    _destructionQueue.RemoveAll();
}
```

**Consequences**:
- While `CollectGarbage` holds the lock, other-thread `SetQueuedForDestruction` calls are **silently dropped** (their `TryLock` also fails) — objects that should be queued for destruction are leaked
- If `DestroyQueued` were ever called from a code path that does NOT pre-hold the lock (e.g., `PopCollector` works correctly), the lock would succeed. But from `CollectGarbage`, it's unsafe
- `TSet` iteration over `_destructionQueue` uses raw pointer begin/end; any concurrent modification (from another thread that acquires the lock between TryLock-fail and iteration) causes iterator invalidation / use-after-free

**Fix direction**: Either use `std::recursive_mutex` or restructure so `DestroyQueued` does not try to re-acquire a lock the caller already holds. Split into an inner `DestroyQueuedUnsafe()` called by both `CollectGarbage` (which holds locks) and `DestroyQueued()` (which acquires them).

---

### BUG-2: GC TryLock cascade causes silent operation drops during collection

**File**: [ObjectCollector.cpp:258-341](Core/Modules/Core/Sources/Object/Collector/ObjectCollector.cpp#L258-L341)
**Severity**: **Critical** — Correctness (objects silently not collected/destroyed)

**Description**:
The TryLock pattern cascades through the entire GC call stack. When `CollectGarbage` holds both `_objectsCriticalSection` and `_destructionQueueCriticalSection`, ALL of these functions use TryLock and silently skip their work:

| Function | Mutex | Effect when skipped |
|----------|-------|---------------------|
| `DestroyQueued()` (line 293) | `_destructionQueueCriticalSection` | Destroys objects unguarded |
| `DestroyObject()` (line 305) | `_objectsCriticalSection` | Accesses `_objectLinksMap` unguarded |
| `SetQueuedForDestruction()` (line 238) | `_destructionQueueCriticalSection` | **Objects queued during GC are silently dropped** |
| `RemoveObjectLink()` (line 115) | `_objectsCriticalSection` | **Object links not removed** |

When `DestroyObject` calls `Link->Referencer()->ReleaseLinks()` (line 312), which calls `TObjectPtr::ReleaseLinks()` → `RemoveObjectLink()`, the TryLock on `_objectsCriticalSection` fails (already held by `CollectGarbage`), so the link removal is silently skipped. The check at line 326 then throws:

```cpp
if (Links.Num() > 0) throw new std::runtime_error("should not have any live links");
```

This throw may be what's manifesting as the "crashing" behavior on Windows.

**Fix direction**: Same as BUG-1 — restructure to use internal unsafe variants that assume locks are held, or use recursive mutexes. The TryLock pattern should be reserved for non-GC code paths that can safely no-op under contention, not for internal GC operations that MUST complete.

---

## High

### BUG-3: CReferencer constructor and destructor modify _parent->_referencers without locking

**File**: [Referencer.cpp:5-19](Core/Modules/Core/Sources/Object/Collector/Referencer.cpp#L5-L19)
**Severity**: **High** — Data race

**Description**:
```cpp
CReferencer::CReferencer(CObject* InParent) : _parent(InParent)
{
    if (_parent != nullptr)
    {
        _parent->_referencers.Add(this);  // NO LOCK on _parent->_criticalSection
    }
}

CReferencer::~CReferencer()
{
    if (_parent != nullptr)
    {
        _parent->_referencers.Remove(this);  // NO LOCK on _parent->_criticalSection
    }
}
```

Meanwhile, `RecursivelyMarkObjects` iterates `_referencers` (ObjectCollector.cpp:357) under `_objectsCriticalSection`, and `DestroyObject` iterates it (ObjectCollector.cpp:316) under `_objectsCriticalSection`. But `_objectsCriticalSection` is the COLLECTOR'S lock, not the parent CObject's lock. The parent's `_criticalSection` is what should protect `_referencers`.

**Consequence**: Concurrent construction/destruction of `TObjectPtr`/`TProperty` instances on different threads while GC is running causes a data race on `TArray<CReferencer*> _referencers`. Can manifest as crashes in TArray operations (corrupted size/capacity, dangling pointers during iteration).

**Fix direction**: Acquire `_parent->_criticalSection` in CReferencer constructor/destructor when modifying `_parent->_referencers`. Or ensure all referencer lifecycle changes happen on the main thread only.

---

### BUG-4: TProperty::ReleaseLinks and EnumerateLinks use TryLock — silently skip under contention

**File**: [Property.h:80-98](Core/Modules/Core/Sources/Object/Properties/Property.h#L80-L98)
**Severity**: **High** — Thread safety / correctness

**Description**:
```cpp
inline void ReleaseLinks() override
{
    SScopeLock Lock(_criticalSection, true);  // TRY LOCK — fails silently
    GObjectCollector.RemoveObjectLink(_link);
    _object = nullptr;
    _link = nullptr;
}

inline void EnumerateLinks(const TFunction<void (CObjectLink*)>& InFunc) const override
{
    SScopeLock Lock(_criticalSection, true);  // TRY LOCK — fails silently
    if (_link != nullptr)
    {
        InFunc(_link);
    }
}
```

If `TryLock` fails (lock held by another thread, or by the GC cascade described above), the critical section is bypassed entirely. `_object` and `_link` are read/written without synchronization.

**Consequence**: During GC, `ReleaseLinks` may silently fail to remove the link. The `DestroyObject` check (`Links.Num() > 0`) then throws. During GC marking, `EnumerateLinks` may return stale data or crash on a concurrently-modified `_link`.

**Fix direction**: `ReleaseLinks` and `EnumerateLinks` must NOT use TryLock — they are called by the GC during collection and must always succeed. Use `Lock()` instead.

---

### BUG-5: TWeakObjectPtr destructor has no locking

**File**: [WeakObjectPtr.h:43-47](Core/Modules/Core/Sources/Object/WeakObjectPtr.h#L43-L47)
**Severity**: **High** — Data race

**Description**:
```cpp
~TWeakObjectPtr()
{
    GObjectCollector.RemoveOnObjectCollectedListener(_object, _handle);
    _object = nullptr;
}
```

No `_criticalSection` lock is acquired. If another thread is concurrently calling `Get()`, `IsValid()`, or the collection callback (which locks `_criticalSection`), there is a data race on `_object`.

**Consequence**: Tear of `_object` pointer value (though unlikely on 64-bit), or `RemoveOnObjectCollectedListener` racing with listener callback invocation during GC. The listener callback sets `_object = nullptr` under lock, but the destructor also sets `_object = nullptr` without lock — potential for lost updates.

**Fix direction**: Lock `_criticalSection` in the destructor before modifying shared state.

---

### BUG-6: TWeakObjectPtr copy constructor — const-correctness violation (latent compilation error)

**File**: [WeakObjectPtr.h:32-41](Core/Modules/Core/Sources/Object/WeakObjectPtr.h#L32-L41)
**Severity**: **High** — Latent compile error

**Description**:
```cpp
TWeakObjectPtr(const TWeakObjectPtr& Weak)
{
    _object = Weak;  // Weak is const TWeakObjectPtr&, but operator TObject*() is NOT const
    // ...
}
```

`operator TObject*()` is non-const (it locks `_criticalSection`). This conversion cannot be called on a `const` reference. The code may only compile because this template is never instantiated for copy construction (templates are only compiled when used). Any future code that copy-constructs a `TWeakObjectPtr` from a const reference will hit a compilation error.

**Fix direction**: Either make `Get()` const (the mutex is already `mutable`, and `Lock()` is `const`), or access `Weak._object` directly (using friend access).

---

### BUG-7: TAtomic::CompareExchange is not a compare-exchange

**File**: [Atomic.h:16-28](Core/Modules/Core/Sources/Threading/Atomic.h#L16-L28)
**Severity**: **High** — Semantic bug (latent)

**Description**:
```cpp
TType CompareExchange(TType NewValue)
{
    SScopeLock Lock(_criticalSection);
    TType Current = _value;
    if (_value != NewValue)    // Compares with NEW value, not EXPECTED value!
    {
        _value = NewValue;
    }
    return Current;
}
```

A proper `CompareExchange(expected, desired)` atomically checks if the current value equals `expected` and only then replaces it with `desired`. This implementation compares against the NEW value and swaps if different — this is `std::atomic::exchange()`, not `compare_exchange()`.

**Current usage** (ObjectCollector.cpp:262):
```cpp
if (_bIsGarbageCollecting.CompareExchange(true)) return;
```

This accidentally works for booleans: if `_bIsGarbageCollecting` is `false`, calling `CompareExchange(true)` sets it to `true` and returns `false` (→ continue with GC). If already `true`, returns `true` (→ early return). But this semantic equivalence holds ONLY for booleans and ONLY when the new value is the opposite of the expected old value.

**Fix direction**: Rename to `Exchange()` or implement proper `CompareExchange(TType Expected, TType Desired)`.

---

## Medium

### BUG-8: PopCollector has a window where GCurrentCollector is nullptr

**File**: [ObjectCollector.cpp:53-85](Core/Modules/Core/Sources/Object/Collector/ObjectCollector.cpp#L53-L85)
**Severity**: **Medium** — Correctness

**Description**:
```cpp
void CObjectCollector::PopCollector()
{
    // ...
    TArray<CObject*> WatchedObjects = GCurrentCollector->WatchedObjects();
    SCollector* Parent = GCurrentCollector->Parent();
    GCurrentCollector = nullptr;  // line 64 — WINDOW OPEN

    for (CObject* Obj : WatchedObjects)
    {
        if (!HasLinks(Obj))
        {
            Obj->SetQueuedForDestruction(true);
        }
        else if (Parent != nullptr)
        {
            Parent->WatchObject(Obj);
        }
    }

    GCurrentCollector = Parent;  // line 79 — WINDOW CLOSED
    // ...
}
```

Between lines 64 and 79, `GCurrentCollector` is `nullptr` on this thread. While `GCurrentCollector` is `thread_local`, any code running on the SAME thread during this window (e.g., inside `HasLinks`, `SetQueuedForDestruction`, or the `else if` branch) that calls `WatchObject` will find `GCurrentCollector == nullptr` and skip adding to the collector. Newly allocated objects in this window are tracked in `_globalObjects` but NOT in any active collector scope, potentially causing them to be prematurely collected.

**Fix direction**: Move `GCurrentCollector = Parent;` to AFTER the for loop but before calling `DestroyQueued()`. Or better, use a scope guard pattern.

---

### BUG-9: static TSet `Marked` in CollectGarbage — read of indeterminate values

**File**: [ObjectCollector.cpp:267](Core/Modules/Core/Sources/Object/Collector/ObjectCollector.cpp#L267)
**Severity**: **Medium** — Undefined behavior

**Description**:
```cpp
static TSet<CObject*> Marked;  // persists across GC calls
```

`TSet::RemoveAll()` (Set.h:153-164) does:
```cpp
inline void RemoveAll(bool bKeepSetSize = true)
{
    for (UInt32 Index = 0; Index < ItemNum; ++Index)
    {
        TElement tempItem;      // CObject* tempItem; — UNINITIALIZED
        SetAt(Index, tempItem); // reads indeterminate value — UB
    }
    ItemNum = 0;
    DecreaseSet(bKeepSetSize);
}
```

For `TElement = CObject*`, `TElement tempItem;` leaves the pointer uninitialized. Reading it (to pass to `SetAt`) is undefined behavior in C++.

**Fix direction**: Value-initialize: `TElement tempItem{};` (which zero-initializes pointers).

---

### BUG-10: DestroyObject clears _objectLinksMap entry then checks it's non-empty — throws on GC cascade

**File**: [ObjectCollector.cpp:303-326](Core/Modules/Core/Sources/Object/Collector/ObjectCollector.cpp#L303-L326)
**Severity**: **Medium** — Incorrect throw during valid GC

**Description**:
```cpp
void CObjectCollector::DestroyObject(CObject* Obj)
{
    SScopeLock ObjectsLock(_objectsCriticalSection, true);  // TryLock!
    // ...
    for (CObjectLink* Link : LinksCopy)
    {
        Link->Referencer()->ReleaseLinks();  // calls RemoveObjectLink
    }
    for (CReferencer* Referencer : Obj->_referencers)
    {
        Referencer->ReleaseLinks();  // calls RemoveObjectLink
    }
    // ...
    TArray<CObjectLink*> Links;
    _objectLinksMap.RemoveAndCopyValue(Obj, Links);  // removes entry
    if (Links.Num() > 0) throw new std::runtime_error("should not have any live links");
}
```

The loops call `ReleaseLinks()` on every referencer, which calls `RemoveObjectLink()`. But `RemoveObjectLink` uses TryLock on `_objectsCriticalSection` (line 115), which is already held by the GC cascade. So the TryLock **fails silently** — the links are NEVER removed from `_objectLinksMap`. Then `RemoveAndCopyValue` extracts them, `Links.Num() > 0` evaluates true, and a `std::runtime_error` is thrown (via `new`, which also leaks).

**Fix direction**: This is a consequence of BUG-1/BUG-2. Fixing the TryLock cascade will fix this. In the meantime, the throw should be a logged warning, not a throw, since the condition is expected during the GC cascade.

---

### BUG-11: CObject::_referencers modified under _objectsCriticalSection but not _criticalSection

**File**: [Object.h:35](Core/Modules/Core/Sources/Object/Object.h#L35) vs [ObjectCollector.cpp:316](Core/Modules/Core/Sources/Object/Collector/ObjectCollector.cpp#L316)
**Severity**: **Medium** — Locking discipline violation

**Description**:
`CObject::_referencers` is documented (by the presence of `_criticalSection`) as needing synchronization. But:
- `CReferencer` constructor/destructor modifies it without ANY lock (BUG-3)
- `DestroyObject` iterates it under `_objectsCriticalSection` (collector lock), not `_criticalSection` (object lock)

Two different locks are used to protect the same data, and one modifier uses neither lock. This violates the locking discipline and makes reasoning about thread safety impossible.

**Fix direction**: Decide on ONE lock for `_referencers`. Either always use `_criticalSection` (object-local) or always use `_objectsCriticalSection` (collector-global). Do not mix.

---

## Low

### BUG-12: AddOnObjectCollectedListener returns reference to map-internal storage

**File**: [ObjectCollector.cpp:148](Core/Modules/Core/Sources/Object/Collector/ObjectCollector.cpp#L148)
**Severity**: **Low** — Fragile API

**Description**:
```cpp
return Listeners->Add(Handle, OnCollected).Key;
```

Returns a `const SObjectCollectedListenerHandle&` pointing to a key inside a `TMap`. If the map is later modified (resize, remove another entry), this reference could dangle. Currently safe because callers immediately copy the value (TWeakObjectPtr stores by value), but the API implies the reference is stable.

**Fix direction**: Return by value instead of by const reference.

---

### BUG-13: ~~ operator deletes allocated exception (leak on error path)

**File**: [ObjectCollector.cpp:326](Core/Modules/Core/Sources/Object/Collector/ObjectCollector.cpp#L326)
**Severity**: **Low** — Memory leak on error

**Description**:
```cpp
throw new std::runtime_error("should not have any live links");
```

`throw new` allocates a `std::runtime_error*` on the heap and throws the pointer. The catch site would need to `delete` it, which is non-idiomatic. Should be `throw std::runtime_error(...)` (by value).

---

## Verification Notes

All findings were verified by direct source code inspection of the exact files referenced. Key verification steps:

1. **BUG-1/2 (GC TryLock cascade)**: Traced the complete call chain: `CollectGarbage` → `DestroyQueued` → `DestroyObject` → `ReleaseLinks` → `RemoveObjectLink` → `SetQueuedForDestruction`. Confirmed every function in the chain uses TryLock on mutexes the caller already holds via Lock().

2. **BUG-3 (CReferencer no lock)**: Confirmed `_parent->_referencers.Add/Remove` in Referencer.cpp have no lock acquisition. Confirmed `RecursivelyMarkObjects` and `DestroyObject` iterate `_referencers` with only the collector lock, not the parent object's lock.

3. **BUG-4 (TProperty TryLock)**: Confirmed lines 82 and 91 use `SScopeLock Lock(_criticalSection, true)` (TryLock), while these methods are called by the GC during critical operations.

4. **BUG-5 (TWeakObjectPtr destructor)**: Confirmed lines 43-47 have no `SScopeLock` at all.

5. **BUG-7 (TAtomic)**: Confirmed the comparison is `_value != NewValue` (comparing with new value), not `_value == Expected` (standard CAS).

6. **BUG-9 (static TSet)**: Confirmed `TElement tempItem;` in Set.h:157 default-initializes pointer types to indeterminate values.
