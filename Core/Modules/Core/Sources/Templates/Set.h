#pragma once

#include "Templates/Array.h"

#include "SmartPointer/SharedPointer.h"

// TODO: use TMap logic without the value to make this Set fast
template<typename TElement>
class TSet
{
	using SetType = TSharedPtr<TElement>;
	using SSetDestructor = SArrayDeleter<TElement>;

	SetType DataPtr;
	UInt64 ItemNum;
	UInt64 SetSize;

	inline void SetAt(const UInt64& Index, const TElement& Item)
	{
		TElement& Element = *(DataPtr + Index);

		Element = Item;
	}

	inline void IncreaseSet()
	{
		UInt64 NewSetSize = SetSize + 4;
		SetType NewDataPtr = SetType(MakeShareable<TElement, SSetDestructor>(new TElement[NewSetSize]));
		GMemory.Copy(DataPtr.Raw(), NewDataPtr.Raw(), sizeof(TElement) * ItemNum);
		DataPtr = NewDataPtr;
		SetSize = NewSetSize;
	}

	inline void DecreaseSet(bool bKeepSetSize)
	{
		if (!bKeepSetSize && (SetSize - ItemNum > 4))
		{
			UInt64 NewSetSize = ItemNum + 4;
			SetType NewDataPtr = SetType(MakeShareable<TElement, SSetDestructor>(new TElement[NewSetSize]));
			GMemory.Copy(DataPtr.Raw(), NewDataPtr.Raw(), sizeof(TElement) * ItemNum);
			DataPtr = NewDataPtr;
			SetSize = NewSetSize;
		}
	}

	template<typename TEquatable>
	inline bool IsEqual(TEquatable LHS, TEquatable RHS) const
	{
		return LHS == RHS;
	}

	template<>
	inline bool IsEqual(const char* LHS, const char* RHS) const
	{
		return std::strcmp(LHS, RHS) == 0;
	}

	template<>
	inline bool IsEqual(UByte LHS, UByte RHS) const
	{
		return LHS == RHS;
	}

public:
	inline TSet(SizeT Size = 4) :
		DataPtr(MakeShareable<TElement, SSetDestructor>(new TElement[Size])),
		ItemNum(0),
		SetSize(Size)
	{
		assert(Size > 0);
	}

	inline TSet(const TElement* Data, const UInt64& Size) :
		DataPtr(MakeShareable<TElement, SSetDestructor>(new TElement[Size])),
		ItemNum(Size),
		SetSize(Size)
	{
		GMemory.Copy(Data, DataPtr.Raw(), sizeof(TElement) * ItemNum);
	}

	inline TSet(const TSet& OtherSet) : TSet(OtherSet.DataPtr.Raw(), OtherSet.ItemNum)
	{

	}

	// other initializer lists constructor
	inline TSet(std::initializer_list<TElement> InitList) noexcept :
		DataPtr(MakeShareable<TElement, SSetDestructor>(new TElement[InitList.size()])),
		ItemNum(0),
		SetSize(InitList.size())
	{
		for (const TElement& Element : InitList)
		{
			Insert(Element);
		}
	}

	inline virtual ~TSet() = default;

	inline bool Insert(const TElement& NewItem)
	{
		// TODO: maybe there is a better performance wise way to do this?
		if (Contains(NewItem)) return false;

		if (ItemNum == SetSize)
		{
			IncreaseSet();
		}

		SetAt(ItemNum++, NewItem);

		return true;
	}

	inline bool Remove(const TElement& Element)
	{
		int32_t IndexToRemove = -1;
		for (UInt32 CurrentIndex = 0; CurrentIndex < ItemNum; CurrentIndex++)
		{
			const TElement& Other = *(DataPtr + CurrentIndex);
			if (IsEqual<TElement>(Element, Other))
			{
				IndexToRemove = CurrentIndex;

				break;
			}
		}

		if (IndexToRemove != -1)
		{
			RemoveAt(IndexToRemove);

			return true;
		}

		return false;
	}

	inline void RemoveAt(const UInt64& Index, bool bKeepSetSize = true)
	{
		assert(Index <= ItemNum - 1);

		ItemNum--;

		for (UInt64 CurrentIndex = Index; CurrentIndex < ItemNum; ++CurrentIndex)
		{
			SetAt(CurrentIndex, DataPtr[CurrentIndex + 1]);
		}

		DecreaseSet(bKeepSetSize);
	}

	inline void RemoveAll(bool bKeepSetSize = true)
	{
		for (UInt32 Index = 0; Index < ItemNum; ++Index)
		{
			TElement tempItem;
			SetAt(Index, tempItem);
		}

		ItemNum = 0;

		DecreaseSet(bKeepSetSize);
	}

	inline bool Contains(const TElement& Element) const
	{
		for (UInt64 CurrentIndex = 0; CurrentIndex < ItemNum; ++CurrentIndex)
		{
			const TElement& Other = *(DataPtr + CurrentIndex);
			if (IsEqual<TElement>(Element, Other))
			{
				return true;
			}
		}

		return false;
	}

	inline UInt32 Num() const
	{
		return (UInt32)ItemNum;
	}

	inline bool IsEmpty() const
	{
		return ItemNum == 0;
	}

	inline TElement* GetData(bool bApplySetSizeToNumOfItems)
	{
		if (bApplySetSizeToNumOfItems)
		{
			ItemNum = SetSize;
		}

		return DataPtr.Get();
	}

	inline const TElement* GetData() const
	{
		return DataPtr.Get();
	}

	inline TElement& First()
	{
		return *DataPtr.Get();
	}

	inline const TElement& First() const
	{
		return *DataPtr.Get();
	}

	inline TElement& operator[](UInt32 Index)
	{
		TElement* Element = DataPtr.Get() + Index;

		return *Element;
	}

	inline TElement* begin()
	{
		return DataPtr.Raw();
	}

	inline TElement* end()
	{
		return DataPtr + ItemNum;
	}
};
