#pragma once

#include "Array.h"
#include "LinkedList.h"
#include "Functions.h"
#include "Hashable.h"

#include "Logger/Logger.h"

#include <type_traits>

template<typename TKey, typename TValue>
struct TKeyValuePair
{
	TKey Key;
	TValue Value;

	bool operator ==(const TKey& OtherKey) const
	{
		return Key == OtherKey;
	}

	bool operator ==(const TKeyValuePair& Other) const
	{
		return Key == Other.Key;
	}
};

template<typename TKey, typename TValue>
class TMap
{
	using TKeyValuePair = TKeyValuePair<TKey, TValue>;
	using TBucket = TLinkedList<TKeyValuePair>;

	TArray<TBucket> _buckets;
	SizeT _size = 0;

    template<typename THashableKey, std::enable_if_t<std::is_integral_v<THashableKey> || std::is_enum_v<THashableKey>, bool> = true>
    SizeT Hash(const THashableKey& Key) 
	{
		return static_cast<SizeT>(Key) % _buckets.Num();
    }

	template<typename THashableKey, std::enable_if_t<std::is_pointer_v<THashableKey>, bool> = true>
	SizeT Hash(THashableKey Key)
	{
		return reinterpret_cast<SizeT>(Key) % _buckets.Num();
	}

    template<typename THashableKey, std::enable_if_t<std::is_base_of_v<CHashable, THashableKey>, bool> = true>
    SizeT Hash(const THashableKey& Key) 
	{
        static_assert(std::is_base_of_v<CHashable, THashableKey>, "For being a Key to TMap, TKey type must implement CHashable");

        return Key.Hash() % _buckets.Num();
    }

	void ResizeIfNeeded()
	{
		if (_size > _buckets.Num() * 0.75f)
		{
			TArray<TBucket> OldBuckets = _buckets;
			_buckets = TArray<TBucket>(OldBuckets.Num() * 2, true);
			
			for (const TBucket& Bucket : OldBuckets)
			{
				Bucket.ForEach([this](const TKeyValuePair& Pair)
				{
					SizeT NewIndex = Hash(Pair.Key); // doing against new bucket
					_buckets[NewIndex].Add(Pair);
				});
			}
		}
	}
	
public:
	TMap() : _buckets(16, true)
	{
		//
	}

	~TMap() = default;

    TKeyValuePair& Add(const TKey& Key, const TValue& Value)
    {
		return Add(TKeyValuePair{ Key, Value });
    }

	TKeyValuePair& Add(const TKeyValuePair& Pair)
	{
		ResizeIfNeeded();

		SizeT Index = Hash(Pair.Key);

		TBucket& Bucket = _buckets[Index];

		if (Bucket.Contains(Pair))
		{
			GLogger.Fatal("TMap: Trying to Add a key that is already present");
		}

		_size++;
		return Bucket.Add(Pair)->Value;
	}

	TValue& Set(const TKey& Key, const TValue& Value)
	{
		return Set(TKeyValuePair{ Key, Value });
	}

	TValue& Set(const TKeyValuePair& Pair)
	{
		ResizeIfNeeded();

		SizeT Index = Hash(Pair.Key);

		TBucket& Bucket = _buckets[Index];

		TNode<TKeyValuePair>* Current = Bucket.Head();
		while (Current != nullptr)
		{
			if (Pair.Key == Current->Value.Key)
			{
				return Current->Value.Value = Pair.Value;
			}

			Current = Current->Next;
		}

		_size++;

		return Bucket.Add(Pair)->Value.Value;
	}

	bool Remove(const TKey& Key)
	{
		SizeT Index = Hash(Key);

		TBucket& Bucket = _buckets[Index];

		TNode<TKeyValuePair>* Current = Bucket.Head();
		while (Current != nullptr)
		{
			if (Key == Current->Value.Key)
			{
				Bucket.Remove(Current);
				
				_size--;
				
				return true;
			}

			Current = Current->Next;
		}

		return false;
	}

	bool RemoveAndCopyValue(const TKey& InKey, TValue& OutValue)
	{
		bool bFound = false;

		if (TValue* Value = Find(InKey))
		{
			bFound = true;
			OutValue = *Value;

			Remove(InKey);
		}
		
		return bFound;
	}

	void RemoveAll()
	{
		_buckets.ForEach([](TBucket& Bucket)
		{
			Bucket.RemoveAll();
		});

		_size = 0;
	}

	TValue* Find(const TKey& Key)
	{
		SizeT Index = Hash(Key);

		TBucket& Bucket = _buckets[Index];

		TNode<TKeyValuePair>* Current = Bucket.Head();
		while (Current != nullptr)
		{
			if (Key == Current->Value.Key)
			{
				return &Current->Value.Value;
			}

			Current = Current->Next;
		}

		return nullptr;
	}

	TValue& operator[](const TKey& Key)
	{
		TValue* Value = Find(Key);

		if (Value == nullptr)
		{
			GLogger.Fatal("TMap: There is no value for the given key");
		}

		return *Value;
	}

	void ForEach(const TFunction<void(const TKeyValuePair&)>& Function) const
	{
		for (const TBucket& Bucket : _buckets)
		{
			Bucket.ForEach(Function);
		}
	}

	template<typename TNum = SizeT>
	TNum Num() const
	{
		return static_cast<TNum>(_size);
	}
};
