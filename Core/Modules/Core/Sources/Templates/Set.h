#pragma once

#include "Defines/Types.h"

#include "Templates/Array.h"
#include "Templates/Hashable.h"
#include "Templates/LinkedList.h"

#include "Logger/Logger.h"

template<typename TElement>
class TSet
{
    using TBucket = TLinkedList<TElement>;

public:
    TSet() : _buckets(16, true)
	{

	}

    ~TSet() = default;

    bool Insert(const TElement& Element)
	{
		ResizeIfNeeded();

		SizeT Index = Hash(Element);

		TBucket& Bucket = _buckets[Index];

		if (Bucket.Contains(Element)) return false;

		_size++;

		Bucket.Add(Element);

		return true;
	}

	bool Remove(const TElement& Element)
	{
		SizeT Index = Hash(Element);

		TBucket& Bucket = _buckets[Index];

		TNode<TElement>* Current = Bucket.Head();
		while (Current != nullptr)
		{
			if (Element == Current->Value)
			{
				Bucket.Remove(Current);
				
				_size--;
				
				return true;
			}

			Current = Current->Next;
		}

		return false;
	}

	bool Contains(const TElement& Element)
	{
		SizeT Index = Hash(Element);

		TBucket& Bucket = _buckets[Index];

		return Bucket.Contains(Element);
	}

	void RemoveAll()
	{
		if (_size == 0) return;

		_buckets.ForEach([](TBucket& Bucket)
		{
			Bucket.RemoveAll();
		});

		_size = 0;
	}

    template<typename TNum = SizeT>
	TNum Num() const
	{
		return static_cast<TNum>(_size);
	}

	void ForEach(const TFunction<void(const TElement&)>& Func)
	{
		for (const TBucket& Bucket : _buckets)
		{
			Bucket.ForEach([Func](const TElement& Element)
			{
				Func(Element);
			});
		}
	}

protected:
    void ResizeIfNeeded()
	{
		if (_size > _buckets.Num() * 0.75f)
		{
			TArray<TBucket> OldBuckets = _buckets;
			_buckets = TArray<TBucket>(OldBuckets.Num() * 2, true);
			
			for (const TBucket& Bucket : OldBuckets)
			{
				Bucket.ForEach([this](const TElement& Element)
				{
					SizeT NewIndex = Hash(Element); // doing against new bucket
					_buckets[NewIndex].Add(Element);
				});
			}
		}
	}

    template<typename THashable, std::enable_if_t<std::is_integral_v<THashable> || std::is_enum_v<THashable>, bool> = true>
    SizeT Hash(const THashable& Hashable) 
	{
		return static_cast<SizeT>(Hashable) % _buckets.Num();
    }

	template<typename THashable, std::enable_if_t<std::is_pointer_v<THashable>, bool> = true>
	SizeT Hash(THashable Hashable)
	{
		return reinterpret_cast<SizeT>(Hashable) % _buckets.Num();
	}

    template<typename THashable, std::enable_if_t<std::is_base_of_v<CHashable, THashable>, bool> = true>
    SizeT Hash(const THashable& Hashable) 
	{
        static_assert(std::is_base_of_v<CHashable, THashable>, "THashable type must implement CHashable");

        return Hashable.Hash() % _buckets.Num();
    }

private:
    TArray<TBucket> _buckets;
    SizeT _size = 0;
};  