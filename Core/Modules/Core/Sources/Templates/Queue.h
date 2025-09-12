#pragma once

#include "LinkedList.h"
#include "Templates/Functions.h"

template<typename TElement>
class TQueue : private TLinkedList<TElement>
{
	using Super = TLinkedList<TElement>;

public:
	inline TQueue() : Super()
	{
		
	}

	inline TQueue(const TElement& Element) : 
		Super(Element)
	{
		
	}

	inline ~TQueue() override = default;

	inline UInt32 Num() override
	{
		return Super::Num();
	}

	inline void Enqueue(const TElement& Element)
	{
		Super::Add(Element);
	}

	inline bool Dequeue(TElement& OutElement)
	{
		// Empty queue
		if (IsEmpty()) return false;

		// One element queue
		if (Num() == 1)
		{
			OutElement = Super::First();

			Super::RemoveFirst();

			return true;
		}

		// More than one element
		OutElement = Super::First();
		
		Super::RemoveFirst();

		return true;
	}

	inline bool Peak(TElement& OutElement)
	{
		if (!IsEmpty())
		{
			OutElement = Super::First();
			
			return true;
		}

		return false;
	}

	inline bool Contains(const TElement& Element)
	{
		return Super::Contains(Element);
	}

	inline bool IsEmpty() const
	{
		return Super::IsEmpty();
	}

	inline void ForEach(const TFunction<void(const TElement&)>& Func) const
	{
		Super::ForEach(Func);
	}

	inline void ForEach(const TFunction<void(TElement&)>& Func)
	{
		Super::ForEach(Func);
	}

	inline void RemoveAll()
	{
		Super::RemoveAll();
	}
};