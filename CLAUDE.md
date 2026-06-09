# CppMobileFramework — Project Documentation

Cross-platform C++20/23 mobile & desktop framework for single-source, per-platform-native application development. Provides a complete platform abstraction layer, custom STL-equivalent containers and smart pointers, a garbage-collected object system, HTTP networking, and C++ wrappers for Apple platform APIs — all with zero external C++ dependencies.

- **Author**: Anderson Lucas C. Ramos
- **License**: MIT (Copyright 2025)
- **Repository**: https://github.com/andersonlucasg3/CppMobileFramework
- **Build System Repo**: https://github.com/andersonlucasg3/CppProjectTools (git submodule)
- **Target Platforms**: Windows (desktop), macOS (desktop), iOS, tvOS, visionOS, Android

---

## Project Overview

### Design Philosophy

1. **Single-source, native output** — One C++ codebase compiles to platform-native binaries. No JavaScript bridges, no embedded web views, no VM runtimes.
2. **Zero external dependencies** — All containers, smart pointers, the GC, the test framework, and even the build system are hand-rolled. Only the platform SDK and C++ standard library are used.
3. **Compile-time platform dispatch** — Platform-specific code lives in named subdirectories (`Windows/`, `Apple/`, `Android/`, `Mac/`). The build system auto-includes the right files per target. No runtime `if` chains for platform detection.
4. **Custom STL** — `TArray`, `TMap`, `TVector`, `TSet`, `TLinkedList`, `TQueue`, `TFunction`, etc. are all custom header-only templates. They use `TSharedPtr` for memory management, giving them shared ownership semantics.
5. **Custom smart pointers** — `TSharedPtr`/`TWeakPtr` with `SRefCounter`, independent of `std::shared_ptr`. `TSharedFromThis` CRTP for shared-from-this. `MakeShared`/`StaticCastSharedPtr`/`StaticCastWeakPtr` factory and cast functions.
6. **Garbage-collected object system** — `CObject` is the base for GC-tracked objects. `TObjectPtr<T>` is a GC-aware smart pointer that registers with `GObjectCollector`. The collector does mark-and-sweep with cycle detection.
7. **Custom build system** — `ProjectTools` is a C#/.NET 9 application that discovers projects via `*.Project.cs` and `*.Module.cs` definitions, generates Visual Studio solutions or `compile_commands.json`, and orchestrates cross-compilation across all platforms.

### High-Level Goals

- Provide a complete framework for building mobile/desktop applications from a single C++ codebase
- Abstract all OS services: filesystem, threading, memory, processes, environment, logging, time, resources
- Provide cross-platform HTTP networking with platform-native backends (WinHTTP on Windows, NSURLSession on Apple)
- Support garbage-collected object lifecycle management with cycle detection
- Expose Apple platform APIs (Foundation, Metal, MetalFX, AppKit, QuartzCore) as C++ wrappers
- Support cross-compilation from a single host machine to all target platforms

---

## Architecture & Code Organization

### Directory Structure

```
CppMobileFramework/
├── Core/                          # Core cross-platform C++ library
│   ├── Core.Project.cs            # Project definition (modules: Core, CoreTests; deps: AppleFrameworks, TestSuit)
│   └── Modules/
│       ├── Core/Sources/          # The Core library (see subsystems below)
│       └── CoreTests/Sources/     # Core test suite (Arrays, Fixed, Strings, SharedPtrs, Filesystem, Queues, ObjectCollector)
│
├── Networking/                    # HTTP networking module
│   ├── Networking.Project.cs      # Project definition (modules: Http, NetworkingTests; deps: Core)
│   └── Modules/
│       ├── Http/Sources/          # HTTP library (WinHTTP on Windows, NSURLSession on Apple)
│       └── NetworkingTests/Sources/  # HTTP tests
│
├── AppleFrameworks/               # C++ wrappers for Apple platform APIs
│   ├── AppleFrameworks.Project.cs # Project definition
│   ├── README.md                  # Documents upstream metal-cpp library
│   ├── SingleHeader/MakeSingleHeader.py  # Script to merge Metal headers
│   └── Modules/
│       ├── Foundation/Sources/    # NSObject, NSString, NSArray, NSDictionary, NSURLSession, etc.
│       ├── Metal/Sources/         # MTLDevice, MTLBuffer, MTLTexture, MTLRenderPipeline, MTLComputePipeline, etc.
│       ├── MetalFX/              # MTLFXSpatialScaler, MTLFXTemporalScaler
│       ├── AppKit/               # NSImage, NSBitmapImageRep
│       └── QuartzCore/           # CAMetalLayer, CAMetalDrawable
│
├── TestSuit/                      # Custom C++ test framework
│   ├── TestSuit.Project.cs
│   └── Modules/TestSuitLib/Sources/  # CAssert, CTestBase, CTestCoordinator
│
├── ProjectTools/                  # Git submodule — C#/.NET 9 build system
│   ├── BuildTool/                 # Main executable (Program.cs, Compilation/, ProjectGeneration/)
│   ├── Shared/                    # Shared library (Platforms/, Toolchains/, Projects/, Sources/, Processes/)
│   └── Scripts/                   # PowerShell build scripts (Compile.ps1, GenCodeProject.ps1, Commons.ps1)
│
├── Binaries/                      # Compiled output
│   ├── DotNet/ProjectTools/       # Compiled BuildTool C# binaries
│   └── Windows/                   # Windows C++ binaries
│
├── Intermediate/                  # Build artifacts (gitignored)
│   ├── Checksums/                 # Incremental build cache
│   ├── Projects/                  # Generated .vcxproj/.sln files
│   ├── CSharpBinaries/            # BuildTool intermediate output
│   └── Windows/                   # Windows .obj files
│
├── Compile.ps1                    # Entry point: delegates to ProjectTools/Scripts/Compile.ps1
├── compile_commands.json          # Generated clangd compilation database (gitignored)
├── .gitmodules                    # Declares ProjectTools submodule
├── .gitignore                     # Ignores binaries, intermediates, .sln, compile_commands.json, .vs/, .cache/
├── .vscode/                       # VS Code: settings.json, tasks.json, launch.json
├── README.md                      # Minimal (just "CppCore")
└── LICENSE                        # MIT License
```

### Core Subsystems (`Core/Modules/Core/Sources/`)

#### Defines/ — Fundamental Types & Platform Detection
| File | Contents |
|------|----------|
| `Types.h` | `Int8`–`Int64`, `UInt8`–`UInt64`, `SizeT`, `Byte`, `UByte`, `Float`, `Double` in `namespace Core::Types` |
| `Preprocessors.h` | `COMPILE_PLATFORM_HEADER(x)`, `COMPILE_PLATFORM_TYPE_HEADER(x)` macros; stringification helpers |
| `Asserts.h` | `assertm`, `assert`, `assert_equal`, `assert_lesser`, `assert_greater` macros |

#### Object/ — Garbage-Collected Object System
| File | Contents |
|------|----------|
| `Object.h/.cpp` | `CObject` — base for all GC-tracked objects. Has `_referencers` array, `_criticalSection`, rooting support (`IsRooted`/`MakeRooted`/`MakeUnrooted`), `IsA<T>()` (dynamic_cast) |
| `Class.h` | `CClass` abstract factory, `TClass<TObject>` concrete factory, `GetClass<TType>()` free function |
| `ClassMacros.h` | `DECLARE_CLASS_HEADER(ClassName)` — forward-declares class + creates `Ptr`/`WeakPtr` typedefs |
| `New.h` | `New<T>(args...)` — heap-allocate via `CClass::CreateRaw`, return `TSharedPtr` |
| `ObjectPtr.h` | `TObjectPtr<TObject>` — GC-aware smart pointer, inherits `CReferencer`, registers with `GObjectCollector` |
| `WeakObjectPtr.h` | `TWeakObjectPtr<TObject>` — weak pointer that nulls itself when the object is collected |
| `Properties/Property.h` | `TProperty<TObject>` — GC-aware property wrapper; `PROPERTY(Type, Name)` macro |
| `Properties/ArrayProperty.h/.cpp` | Array property variant |

#### Object/Collector/ — Garbage Collector Subsystem
| File | Contents |
|------|----------|
| `ObjectCollector.h/.cpp` | `CObjectCollector` — mark-and-sweep with cycle detection. `AddObjectLink`/`RemoveObjectLink`, `WatchObject`/`UnWatchObject`, `AddToRoot`/`RemoveFromRoot`, `CollectGarbage`, `DestroyQueued`. Global: `GObjectCollector` |
| `ObjectLink.h/.cpp` | `CObjectLink` — link between a `CObject` and its `CReferencer` |
| `Referencer.h/.cpp` | `CReferencer` — abstract base; `ReleaseLinks()` + `EnumerateLinks()` virtuals |
| `Collector.h` | `SCollector` — scope-based watched-object list |
| `CollectorScope.h` | `SCollectorScope` — RAII scope push/pop for `SCollector` |
| `ObjectCollectedListenerHandle.h/.cpp` | Listener handle for object-collected notifications |

#### SmartPointer/ — Custom Smart Pointer System
| File | Contents |
|------|----------|
| `RefCounter.h` | `SRefCounter` — strong/weak reference counting |
| `Deleter.h` | `IDeleter` interface, `TDeleter<TPtr>`, `SArrayDeleter<TElement>` |
| `SharedPointer.h` | `TSharedPtr<TPtr>` — `RetainStrong`/`ReleaseStrong`, copy from `TWeakPtr`/`TSharedFromThis`, raw pointer conversion |
| `WeakPointer.h` | `TWeakPtr<TPtr>` — `RetainWeak`/`ReleaseWeak`, construction from `TSharedPtr`/`TSharedFromThis` |
| `SharedFromThis.h` | `TSharedFromThis<TThis>` — CRTP, provides `AsShared()` and `AsWeak()` |
| `MakeAndCasts.h` | `MakeShared<T>(Args...)`, `MakeShareable<T>(Ptr)`, `StaticCastSharedPtr<To>(From)`, `StaticCastWeakPtr<To>(From)` |
| `PointerEquality.h` | Pointer comparison helpers |

#### Templates/ — Custom Container Library
| File | Contents |
|------|----------|
| `Array.h` | `TArray<TElement>` — dynamic array with `TSharedPtr` + `SArrayDeleter`. `Add`, `Insert`, `Remove`, `RemoveAt`, `RemoveWhere`, `Contains`, `ForEach`, `Reverse`, `Resize`, `Cast`, iterator support |
| `Vector.h` | `TVector<TElement, Size>` — 2D/3D/4D vector. SFINAE constructors per dimension. `Dot`, `Cross` (3D only), `Normalize`, `Length`, direction constants |
| `Matrix.h` | `TMatrix<TElement, Size>` — square matrix |
| `Map.h` | `TMap<TKey, TValue>` — hash map with `TLinkedList` buckets. SFINAE hash for integral/enum/pointer/`CHashable` keys. 75% load factor resize |
| `Set.h` | `TSet<TElement>` — linear-probe set backed by `TSharedPtr` array |
| `LinkedList.h` | `TLinkedList<TElement>` — doubly-linked list with `TNode<TValue>` |
| `Queue.h` | `TQueue<TElement>` — FIFO, privately inherits `TLinkedList` |
| `Point.h` | `TPoint<TElement>` (X, Y) |
| `Size.h` | `TSize<TElement>` (Width, Height) |
| `Rect.h` | `TRect<TElement, TPoint, TSize>` (Origin + Size) |
| `Functions.h` | `TFunction<TReturn(TArgs...)>` — type-erased callable via `CCallableBase`/`TCallableStorage` |
| `Hashable.h` | `CHashable` — abstract base with virtual `Hash()` |
| `Strings.h` | Platform include bridge for string conversion |

#### Math/
| File | Contents |
|------|----------|
| `Math.h` | `TMath<TElement>` in `namespace Core::Maths`: trig (Sin/Cos/Tan/Asin/Atan2), Rad2Deg/Deg2Rad, Sqrt, Pow, Abs, CopySign |
| `Fixed.h` | `TFixed<Int64 P>` — scaled-integer fixed-point math. Extensive operator overloading. `SFixed = TFixed<1000000000L>` typedef |

#### Threading/
| File | Contents |
|------|----------|
| `Thread.h/.cpp` | `CThread` (inherits `TSharedFromThis<CThread>`): `Start(TFunction)`, `Join`, `Exit`, `Create()`, `Current()`, `IsMainThread()` |
| `CriticalSection.h/.cpp` | `SCriticalSection` — wraps `std::mutex` with `Lock`/`TryLock`/`Unlock` |
| `ScopeLock.h/.cpp` | `SScopeLock` — RAII mutex lock |
| `Semaphore.h/.cpp` | Semaphore wrapper |
| `Atomic.h` | `TAtomic<TType>` — thread-safe atomic using critical section for compare-exchange |

#### Platform Abstraction Services
All follow the same pattern: **abstract base class** → **platform-specific implementation** → **global extern singleton**.

| Abstract Base | Global | Platform Implementations |
|---------------|--------|-------------------------|
| `CEnvironment` | `GEnvironment` | `WindowsEnvironment`, `AppleEnvironment`, `AndroidEnvironment`, `MacEnvironment` |
| `CPath` | `GPath` | `WindowsPath`, `ApplePath`, `AndroidPath`, `MacPath` (typedef of `ApplePath`) |
| `CFile` | *(factory)* | `WindowsFile`, `AppleFile`, `AndroidFile`, `MacFile` |
| `CDirectoryReference` | *(factory)* | `WindowsDirectoryReference`, `AppleDirectoryReference`, `AndroidDirectoryReference`, `MacDirectoryReference` |
| `CFileReference` | *(factory)* | `WindowsFileReference`, `AppleFileReference`, `AndroidFileReference`, `MacFileReference` |
| `CMemory` | `GMemory` | `WindowsMemory`, `AppleMemory`, `AndroidMemory`, `MacMemory` |
| `CLogger` | `GLogger` | `WindowsLogger`, `AppleLogger` (`.mm`, uses NSLog), `AndroidLogger`, `MacLogger` |
| `CProcess` | `GProcess` | `WindowsProcess` ← `DesktopProcess`, `MacProcess` ← `DesktopProcess`, `AndroidProcess` ← `MobileProcess` |
| `CResources` | `GResources` | `WindowsResources`, `AndroidResources`, `MacResources` |
| `CTime` | `GTime` | `WindowsTime`, `AppleTime` (mach_absolute_time), `AndroidTime` (clock_gettime), `MacTime` |
| `CThread` | `Create()` | `WindowsThread`, `MacThread`, `AndroidThread` (Apple/iOS variant also) |

**Dispatch mechanism**: Each `.cpp` file uses `#include COMPILE_PLATFORM_HEADER(Path.h)` which expands to `Windows/WindowsPath.h` (or equivalent). That header `typedef`s the platform class to `CPlatformPath`, which is instantiated as the global singleton.

#### String/
| File | Contents |
|------|----------|
| `String.h/.cpp` | `CString` (inherits `CHashable`). Format-string constructor using `std::vformat`. `SubString`, `Replace`, `Split`, `StartsWith`, `EndsWith`, comparison operators, `WStr()` for Windows `wchar_t` |
| `Name.h/.cpp` | `CName` — interned string |
| `Apple/AppleStringConvertion.h/.cpp` | NSString ↔ CString conversion |

#### Process/ — Process Abstraction Hierarchy
```
CProcess (abstract)
├── CDesktopProcess (adds GetExecutablePath)
│   ├── CWindowsProcess
│   └── CMacProcess
└── CMobileProcess
    └── CAndroidProcess
```

#### Networking/ (HTTP Library)
| File | Contents |
|------|----------|
| `HttpRequest.h/.cpp` | `CHttpRequest` (inherits `TSharedFromThis`), `EHttpRequestMethod` enum |
| `HttpResponse.h/.cpp` | `CHttpResponse` — status code, response data, response string |
| `HttpRequestError.h/.cpp` | `CHttpRequestError` — error information |
| `HttpRequestManager.h/.cpp` | `CHttpRequestManager` abstract factory, global `GHttpRequestManager` |
| `Windows/WindowsHttpRequest.h/.cpp` | WinHTTP implementation |
| `Windows/WindowsHttpRequestManager.h/.cpp` | Windows HTTP manager |
| `Mac/MacHttpRequest.h/.cpp` | NSURLSession implementation |
| `Mac/MacHttpRequestManager.h/.cpp` | macOS HTTP manager with URLSession delegate wiring |

### Key Design Patterns

1. **Abstract base + platform typedef + global singleton** — Every platform service has a virtual interface, platform implementations, and a global `G*` pointer initialized at startup. The `.cpp` file uses `COMPILE_PLATFORM_HEADER` to pick the right impl at compile time.

2. **Custom smart pointers with CRTP** — `TSharedPtr`/`TWeakPtr` maintain their own `SRefCounter` block. `TSharedFromThis<T>` uses CRTP so that `AsShared()`/`AsWeak()` return properly typed pointers. `MakeShared<T>()` uses `if constexpr` to detect `TSharedFromThis` inheritance and initialize the back-link.

3. **GC-aware smart pointers** — `TObjectPtr<T>` (for CObject-derived types) inherits from `CReferencer` and registers itself with `GObjectCollector`. The collector tracks all links between objects and referencers, performs mark-and-sweep from roots, and detects cycles.

4. **Header-only template containers** — All containers are templates using `TSharedPtr` for backing storage with a custom `SArrayDeleter`. This means containers have shared ownership semantics.

5. **Type erasure for callables** — `TFunction<Signature>` uses `CCallableBase` as the virtual interface and `TCallableStorage<Callable, TReturn, Args...>` as the concrete wrapper — avoids `std::function`.

6. **SFINAE / `if constexpr`** — Used extensively in `TVector` (dimension-specific constructors), `TMap` (hash dispatch by key type), and `MakeShared` (TSharedFromThis detection).

7. **Platform source filtering** — Source files in platform-named directories (`Windows/`, `Apple/`, `Android/`, `Mac/`) are auto-included or excluded by the C# build system based on the target platform. No preprocessor `#if` chains needed for file selection.

### Namespace Usage

| Namespace | Location | Contents |
|-----------|----------|----------|
| `Core::Types` | `Defines/Types.h` | `Int8`–`Int64`, `UInt8`–`UInt64`, `SizeT`, `Byte`, `UByte`, `Float`, `Double` |
| `Core::Maths` | `Math/Math.h`, `Math/Fixed.h` | `TMath<TElement>`, `TFixed<Int64 P>`, `SFixed` |
| `Objects::Properties` | `Object/Properties/Property.h` | `TProperty<TObject>` |
| *(global)* | Most other classes | `C*` classes, `S*` structs, `T*` templates, `E*` enums, `I*` interfaces, `G*` globals |

### Naming Conventions

| Prefix | Meaning | Examples |
|--------|---------|----------|
| `C` | Class | `CObject`, `CString`, `CFile`, `CThread`, `CHttpRequest` |
| `S` | Struct / typedef alias | `SFileRef`, `SRefCounter`, `SFixed`, `SCriticalSection`, `SScopeLock` |
| `T` | Template | `TArray`, `TMap`, `TSharedPtr`, `TVector`, `TFunction`, `TFixed` |
| `E` | Enum | `EOpenMode`, `EHttpRequestMethod` |
| `I` | Interface | `IDeleter` |
| `G` | Global singleton | `GLogger`, `GMemory`, `GPath`, `GObjectCollector`, `GHttpRequestManager` |

---

## Build System & Development Workflow

### Build System Overview

The build is driven by **ProjectTools**, a custom C#/.NET 9 application stored as a git submodule (`ProjectTools/` → `github.com/andersonlucasg3/CppProjectTools`).

**Two-stage build process:**
1. **Bootstrap**: `dotnet build ProjectTools/ProjectTools.sln -c Debug -o Binaries/DotNet/ProjectTools` — compiles the C# BuildTool
2. **C++ Compilation**: `dotnet exec Binaries/DotNet/ProjectTools/BuildTool.dll Compile <args>` — discovers project definitions, generates platform-specific flags, invokes the appropriate toolchain

### Project & Module Definitions

Projects and modules are defined in C# source files that the BuildTool compiles, loads, and reflects over:

**Project files (`*.Project.cs`)** — Define a project, its modules, and cross-project dependencies:

| File | Project | Modules | Dependencies |
|------|---------|---------|-------------|
| `Core/Core.Project.cs` | Core | `Core`, `CoreTests` | AppleFrameworks (Apple only), TestSuit |
| `Networking/Networking.Project.cs` | Networking | `Http`, `NetworkingTests` | Core |
| `AppleFrameworks/AppleFrameworks.Project.cs` | AppleFrameworks | `Foundation`, Metal (commented out) | — |
| `TestSuit/TestSuit.Project.cs` | TestSuit | `TestSuitLib` | — |

**Module files (`*.Module.cs`)** — Define a module's binary type, dependencies, and platform specifics:

| Module | Binary Type | Dependencies | Platform Libraries |
|--------|-------------|-------------|-------------------|
| Core | DynamicLibrary | Foundation (macOS only) | `shell32.lib`, `ole32.lib` (Win) |
| CoreTests | Application | TestSuitLib, Core | — |
| Http | DynamicLibrary | Core, Foundation (macOS) | `winhttp.lib` (Win) |
| NetworkingTests | Application | Http, TestSuitLib | — |
| TestSuitLib | DynamicLibrary | — | — |
| Foundation | DynamicLibrary | — | `-framework Foundation` (Apple) |
| Metal | DynamicLibrary | Foundation | `-framework Metal` (Apple, commented out) |

**Binary types**: `Application` (.exe/Mach-O/.so -pie), `DynamicLibrary` (.dll/.dylib/.so), `StaticLibrary` (.lib/.a), `ShaderLibrary` (.metallib)

### Target Platforms & Toolchains

| Platform | Type | Toolchain | Compiler | Key Flags |
|----------|------|-----------|----------|-----------|
| **Windows** | Desktop | VisualStudioToolchain | clang-cl (VS 2022 bundled LLVM) | `/std:c++20 /W4 /EHsc /GR` |
| **macOS** | Desktop | XcodeToolchain | xcrun clang++ | `-std=c++23 -mmacosx-version-min=13.4 -stdlib=libc++` |
| **iOS** | Mobile | XcodeToolchain | xcrun clang++ | `-std=c++23 -miphoneos-version-min=17.0 -stdlib=libc++` |
| **tvOS** | Mobile | XcodeToolchain | xcrun clang++ | `-stdlib=libc++` |
| **visionOS** | Mobile | XcodeToolchain | xcrun clang++ | `-stdlib=libc++` |
| **Android** | Mobile | AndroidToolchain | NDK clang++ | NDK 29.0.13846066, API 21, `aarch64-linux-android21` |

**Host ↔ Target matrix**:
- Windows host → Windows, Android
- macOS host → macOS, iOS, tvOS, visionOS, Android

### Building

```powershell
# Generate Visual Studio solution
.\ProjectTools\Scripts\GenCodeProject.ps1 -Project Core -Generator VisualStudio -Platform Windows -Configuration Debug

# Generate compile_commands.json (for clangd / IDE IntelliSense)
.\ProjectTools\Scripts\GenCodeProject.ps1 -Project Core -Generator Clang -Platform Windows -Configuration Debug

# Compile a project
.\ProjectTools\Scripts\Compile.ps1 -Project Core -Platform Windows -Configuration Debug

# Compile specific modules
.\ProjectTools\Scripts\Compile.ps1 -Project Core -Modules Core,CoreTests -Platform Windows -Configuration Debug

# Clean + compile
.\ProjectTools\Scripts\Compile.ps1 -Project Core -Platform Windows -Clean

# Compile only the C# BuildTool (skip C++)
.\Compile.ps1 -ProjectToolsOnly

# Re-link (skip compilation, re-link only)
.\ProjectTools\Scripts\Compile.ps1 -Project Core -Platform Windows -Relink
```

### VS Code Integration

**tasks.json** defines:
- "Gen Project" — Generates Visual Studio project files or compile_commands.json
- "Compile" — Full compilation
- "Recompile" — Rebuild
- "Compile ProjectTools" — Build the C# build tool
- "Compile CoreTests" — Compile and run Core test executable
- "Compile NetworkingTests" — Compile and run Networking test executable
- "Clean" — Clean outputs

All tasks prompt interactively for: Generator (VisualStudio/Clang), Platform, Configuration (Debug/Release), and Project.

**launch.json** defines debug targets:
- Debug GenCodeProject (BuildTool.exe)
- Debug Compile (BuildTool.exe)
- Debug CoreTests (C++ executable via lldb)
- Debug NetworkingTests (C++ executable via lldb)

### Preprocessor System

The build system auto-generates these defines for every compilation unit:

**Platform one-hot encoding** (exactly one is `1`, rest `0`):
```
PLATFORM_WINDOWS   PLATFORM_MACOS   PLATFORM_IOS
PLATFORM_TVOS      PLATFORM_VISIONOS   PLATFORM_ANDROID
```

**Platform grouping**:
```
PLATFORM_GROUP_APPLE       PLATFORM_GROUP_GOOGLE    PLATFORM_GROUP_MICROSOFT
PLATFORM_TYPE_MOBILE       PLATFORM_TYPE_DESKTOP
```

**Platform name strings**: `PLATFORM_NAME`, `PLATFORM_GROUP_NAME`, `PLATFORM_TYPE_NAME`

**Configuration**: `DEBUG` or `NDEBUG`, `WITH_DEBUG=0/1`, `WITH_RELEASE=0/1`

**Module API macros** (Windows only): `CORE_API=__declspec(dllexport)` when building Core, `CORE_API=__declspec(dllimport)` when consuming. Empty on Apple/Android.

**Android-specific**: `ANDROID_PLATFORM=android-21`

**Per-module custom**: e.g., `METAL_ENABLED` from the Metal module definition

### Platform Source Filtering

The `CppSourceCollection` class in the build system auto-filters source files:
- When targeting **Windows**, files under `*/Windows/` are included; files under `*/Apple/`, `*/Mac/`, `*/Android/`, `*/iOS/` are excluded
- When targeting **macOS**, files under `*/Apple/` and `*/Mac/` are included; `*/Windows/`, `*/Android/` are excluded
- When targeting **Android**, files under `*/Android/` are included; everything else excluded
- Apple builds additionally collect `.m`, `.mm`, `.mi`, `.mii` (Objective-C) and `.metal` (Metal shader) files

This is how cross-platform code is selected **without** preprocessor `#ifdef` chains on `#include`.

### Incremental Build

- SHA-256 checksums stored in `Intermediate/Checksums/<Platform>/<Configuration>/Cached.checksums`
- Tracks: source file content, compilation command line, and all included header files
- A file is recompiled only if any of these checksums changed
- Failed compilations are tracked separately (not cached as success)

### Output Structure

```
Binaries/<Platform>/<Configuration>/     # Final linked binaries
Intermediate/<Platform>/<Configuration>/<Module>/Objects/   # .obj/.o files
Intermediate/Checksums/<Platform>/<Configuration>/           # Incremental build cache
Intermediate/Projects/                    # Generated .vcxproj/.sln files
Intermediate/CSharpBinaries/              # BuildTool compilation output
```

### Testing

Tests use the custom **TestSuit** framework:
- `CTestBase` — abstract base, `TestCase() = 0`
- `CTestCoordinator` — static class, `Run()` executes all registered tests
- `CAssert` — assertion helpers
- `REGISTER_TEST_CLASS(ClassName)` / `DEFINE_TEST_RUNNER` — macros for test registration

**Test modules**:
| Module | Tests | Sources |
|--------|-------|---------|
| CoreTests | Arrays, Types/Fixed, Strings, SharedPtrs, Filesystem, Queues, ObjectCollector | `Core/Modules/CoreTests/Sources/` |
| NetworkingTests | HttpRequest | `Networking/Modules/NetworkingTests/Sources/` |

---

## Current State

### Branch Structure

| Branch | Status | Commits | Last Commit |
|--------|--------|---------|-------------|
| `main` | Stable trunk | 42 | `2661f53` — "Fixed linkage/compile dependencies" |
| `feature/smart-garbage-collection` | **ACTIVE development** | 48 (6 ahead of main) | `805a3f2` — "Fixed crashes and misconceptions on Windows" |

No other branches, no tags. `feature/smart-garbage-collection` is a clean fast-forward from `main` — no commits exist in `main` that aren't also in the feature branch.

### Current Focus: Smart Garbage Collection

The `feature/smart-garbage-collection` branch adds a custom GC with cycle detection to the `CObject` system:

**New files** (vs main):
- `Object/Collector/ObjectCollector.h/.cpp` — `CObjectCollector`
- `Object/Collector/ObjectLink.h/.cpp` — `CObjectLink`
- `Object/Collector/Referencer.h/.cpp` — `CReferencer`
- `Object/Collector/Collector.h` — `SCollector`
- `Object/Collector/CollectorScope.h` — `SCollectorScope`
- `Object/Collector/ObjectCollectedListenerHandle.h/.cpp`
- `Object/ObjectPtr.h` — `TObjectPtr<T>`
- `Object/WeakObjectPtr.h` — `TWeakObjectPtr<T>`
- `Object/Properties/Property.h` — `TProperty<T>`
- `Object/Properties/ArrayProperty.h/.cpp`
- `Threading/Atomic.h` — `TAtomic<T>`
- `Core/Modules/CoreTests/Sources/ObjectCollector/ObjectCollectorTests.h/.cpp`

**Scope**: 44 files changed, ~1604 insertions, ~102 deletions.

**Known issues** (from commit messages):
- **Thread safety**: "Crashing using the GC on a thread" — GC operations across threads cause crashes
- **Windows memory**: "Making compile on Windows, but crashing due to deleted memory access" — use-after-free
- `805a3f2` "Fixed crashes and misconceptions on Windows" is the most recent fix attempt — stability unverified

### What's Stable (on `main`)

| System | Status | Notes |
|--------|--------|-------|
| Platform abstraction | ✅ Stable | All 8 services have platform impls for Windows, macOS, Android |
| Smart pointers | ✅ Stable | TSharedPtr / TWeakPtr / TSharedFromThis / MakeShared |
| Template containers | ✅ Stable | TArray, TMap, TSet, TVector, TLinkedList, TQueue, TFunction |
| String system | ✅ Stable | CString with format, Split, Replace; CName |
| Math | ✅ Stable | TMath (trig), TFixed (fixed-point) |
| Threading basics | ✅ Stable | CThread, SCriticalSection, SScopeLock, TSemaphore |
| HTTP (Windows) | ✅ Stable | WinHTTP-based CHttpRequest |
| HTTP (macOS) | ⚠️ Partial | NSURLSession delegate wiring was problematic during development |
| Apple Frameworks | ✅ Stable | Foundation, Metal, MetalFX, AppKit, QuartzCore C++ wrappers |
| Test framework | ✅ Stable | TestSuitLib |
| Build system | ✅ Stable | ProjectTools C# build system |

### What's Missing / TODO

| Area | Status |
|------|--------|
| Android HTTP | Not implemented |
| iOS/tvOS/visionOS HTTP | Not implemented |
| Android logger | Stub: `// TODO: write log to logcat` |
| Android `CThread::Join()` | Unimplemented |
| Metal module | Commented out in `AppleFrameworks.Project.cs` |
| Cross-platform UI | No UI abstraction layer exists |
| JNI / Java bridge | No Java/Kotlin code or JNI bindings anywhere |
| README | Contains only "CppCore" |
| Stable GC | Thread-safety fixes in progress on feature branch |

### Development History

- **Started**: July 31, 2025
- **Active span**: ~40 days (July–September 2025)
- **Contributor**: Anderson Lucas C. Ramos (sole author, 48 commits, 100%)
- **Commit cadence**: Highly active, averaging >1 commit/day during development windows

**Development phases**:
1. **Foundation** (July 31 – Aug 6) — Cross-platform Core, Windows compilation, module system
2. **Android support** (Aug 9–15) — NDK toolchain, Android core classes, full linking
3. **HTTP/Networking** (Aug 15–27) — CHttpRequest API, WinHTTP + NSURLSession backends, test infrastructure
4. **Smart GC** (Sep 7–9, **current**) — Custom garbage collector with cycle detection, being stabilized

---

## Rules for Working on This Project

### Code
1. **No external C++ dependencies** — All data structures and abstractions must be hand-rolled or use the platform SDK + C++ standard library
2. **Use project smart pointers** — `TSharedPtr`/`TWeakPtr`, not `std::shared_ptr`. Use `MakeShared<T>()` for allocation.
3. **GC-tracked objects** — Inherit from `CObject`, allocate with `New<T>()`, reference with `TObjectPtr<T>`
4. **Platform-specific code** — Goes in platform-named subdirectories (`Windows/`, `Apple/`, `Android/`, `Mac/`). The build system handles inclusion/exclusion automatically.
5. **Platform abstractions** — Follow the abstract-base + global-extern pattern. Add the abstract class to the Core subsystem, implement in each platform directory.

### Build System
6. **New modules** — Need a `.Module.cs` definition file specifying: binary type, dependencies, platform libraries
7. **New projects** — Need a `.Project.cs` definition file registering its modules and cross-project dependencies
8. **Build system changes** — Go in the ProjectTools submodule repo (`github.com/andersonlucasg3/CppProjectTools`), not here. Update the submodule pointer after merging there.

### Testing
9. **Use TestSuit** — All tests use the custom framework: inherit `CTestBase`, implement `TestCase()`, register with `REGISTER_TEST_CLASS`
10. **Test modules** — Tests are separate Application modules that link against tested libraries

---

## Quick Reference

### Common Commands

```powershell
# Generate VS solution for IDE work
.\ProjectTools\Scripts\GenCodeProject.ps1 -Project Core -Generator VisualStudio -Platform Windows -Configuration Debug

# Full Windows Debug build
.\ProjectTools\Scripts\Compile.ps1 -Project Core -Platform Windows -Configuration Debug

# Build and run Core tests
.\ProjectTools\Scripts\Compile.ps1 -Project Core -Modules CoreTests -Platform Windows -Configuration Debug

# Build only the C# tool
.\Compile.ps1 -ProjectToolsOnly

# Clean everything
.\ProjectTools\Scripts\Compile.ps1 -Project Core -Platform Windows -Clean
```

### Key Files to Know

| File | Why |
|------|-----|
| `Core/Core.Project.cs` | Registers Core + CoreTests modules, declares dependencies |
| `Core/Modules/Core/Core.Module.cs` | Core library build config (binary type, platform libraries) |
| `Networking/Networking.Project.cs` | Registers Http + NetworkingTests modules |
| `AppleFrameworks/AppleFrameworks.Project.cs` | Registers Foundation module (Apple only) |
| `Core/Modules/Core/Sources/Defines/Preprocessors.h` | Platform dispatch macros used everywhere |
| `Core/Modules/Core/Sources/Object/Object.h` | CObject — base of the GC-tracked object hierarchy |
| `Core/Modules/Core/Sources/Object/Collector/ObjectCollector.h` | GObjectCollector — the garbage collector |
| `Core/Modules/Core/Sources/SmartPointer/SharedPointer.h` | TSharedPtr — the framework's shared pointer |
| `Core/Modules/Core/Sources/Templates/Array.h` | TArray — primary container type |
| `ProjectTools/BuildTool/Program.cs` | Entry point for the C# build tool |
| `ProjectTools/Shared/Platforms/TargetPlatform.cs` | ETargetPlatform enum (iOS, tvOS, visionOS, macOS, Android, Windows) |
| `.vscode/tasks.json` | VS Code build task definitions |
| `.vscode/launch.json` | VS Code debug configurations |

### Git

```bash
# The active development branch
git checkout feature/smart-garbage-collection

# Update the build system submodule
git submodule update --init --recursive

# See what the GC branch adds vs main
git diff main...feature/smart-garbage-collection --stat
```
