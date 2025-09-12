#pragma once

#include "Defines/Types.h"
#include "Templates/Functions.h"

#include "SmartPointer/MakeAndCasts.h"
#include "SmartPointer/Deleter.h"

template<typename TElement>
struct SArrayDeleter : public TDeleter<TElement>
{
	using Super = TDeleter<TElement>;

	SArrayDeleter(TElement* InPtr) : Super(InPtr)
	{
		//
	}

	void Delete() override
	{
		if (Super::_ptr == nullptr) return;

		delete[] Super::_ptr;
	}
};

template<typename TElement>
class TArray
{
	using ArrayType = TSharedPtr<TElement>;
	using ArrayDeleter = SArrayDeleter<TElement>;

	ArrayType DataPtr;
	SizeT ItemNum;
	SizeT ArraySize;
	
	TElement& SetAt(UInt64 Index, const TElement& Item)
	{
		TElement& Element = *(DataPtr + Index);

		return Element = Item;
	}

	void IncreaseArray(SizeT InSize = 4)
	{
		Resize(ArraySize + InSize);
	}

	void DecreaseArray(bool bKeepArraySize)
	{
		if (bKeepArraySize) return;

		if (ArraySize - ItemNum > 4)
		{
			Resize(ItemNum + 4);
		}
	}

	template<typename TEquatable>
	bool IsEqual(TEquatable LHS, TEquatable RHS) const
	{
		return LHS == RHS;
	}

	template<>
	bool IsEqual(const char* LHS, const char* RHS) const
	{
		return std::strcmp(LHS, RHS) == 0;
	}

public:
	TArray(const TArray& Other) :
		ItemNum(Other.ItemNum),
		ArraySize(Other.ItemNum)
	{
		DataPtr = MakeShareable<TElement, ArrayDeleter>(new TElement[Other.ItemNum]);

		for (UInt32 Index = 0; Index < Other.ItemNum; ++Index)
		{
			SetAt(Index, Other.DataPtr[Index]);
		}
	}

	TArray(SizeT Size = 4, bool bSetItemNumFromSize = false) :
		ItemNum(0),
		ArraySize(Size)
	{
		if (Size > 0)
		{
			DataPtr = MakeShareable<TElement, ArrayDeleter>(new TElement[Size]);
		}

		if (bSetItemNumFromSize)
		{
			ItemNum = Size;
		}
	}

	TArray(const TElement* Data, SizeT Size) :
		DataPtr(MakeShareable<TElement, ArrayDeleter>(new TElement[Size])),
		ItemNum(Size),
		ArraySize(Size)
	{
		for (UInt32 Index = 0; Index < Size; ++Index)
		{
			SetAt(Index, Data[Index]);
		}
	}

	// initializer lists constructor
	TArray(std::initializer_list<TElement> InitList) noexcept :
		ItemNum(InitList.size()),
		ArraySize(InitList.size())
	{
		DataPtr = MakeShareable<TElement, ArrayDeleter>(new TElement[InitList.size()]);

		for (UInt32 Index = 0; Index < ItemNum; ++Index)
		{
			SetAt(Index, InitList.begin()[Index]);
		}
	}

	virtual ~TArray()
	{
		DataPtr = nullptr;
	}

	TElement& Add(const TElement& NewItem)
	{
		if (ItemNum == ArraySize)
		{
			IncreaseArray();
		}

		return SetAt(ItemNum++, NewItem);
	}

	void Add(const TArray<TElement>& InElements)
	{
		if (ArraySize - ItemNum <= InElements.Num())
		{
			IncreaseArray(InElements.Num() + 4);
		}

		for (UInt64 Index = ItemNum; Index < ItemNum + InElements.ItemNum; ++Index)
		{
			SetAt(Index, InElements[Index - ItemNum]);
		}

		ItemNum += InElements.ItemNum;
	}

	TElement& Insert(UInt64 Index, const TElement& NewItem)
	{
		if ((static_cast<Int64>(ArraySize) - static_cast<Int64>(ItemNum + 1)) <= 0)
		{
			IncreaseArray();
		}

		for (UInt64 CurrentIndex = ItemNum++; CurrentIndex > Index; --CurrentIndex)
		{
			// setting the previous to the next
			SetAt(CurrentIndex, DataPtr[CurrentIndex - 1]);
		}

		return SetAt(Index, NewItem);
	}

	void Fill(const TElement& Element)
	{
		ItemNum = ArraySize;
		for (UInt64 Index = 0; Index < ItemNum; ++Index)
		{
			SetAt(Index, Element);
		}
	}

	bool Remove(const TElement& Element, bool bKeepArraySize = true)
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
			RemoveAt(IndexToRemove, bKeepArraySize);

			return true;
		}

		return false;
	}

	void RemoveAt(UInt64 Index, bool bKeepArraySize = true)
	{
		if (Index < 0 || Index >= ItemNum) throw "Index out of bounds";
		
		ItemNum--;

		for (UInt64 CurrentIndex = Index; CurrentIndex < ItemNum; ++CurrentIndex)
		{
			SetAt(CurrentIndex, DataPtr[CurrentIndex + 1]);
		}

		DecreaseArray(bKeepArraySize);
	}

	void RemoveWhere(const TFunction<bool(const TElement&)>& Predicate, bool bKeepArraySize = true)
	{
		for (UInt64 CurrentIndex = 0; CurrentIndex < ItemNum; ++CurrentIndex)
		{
			const TElement& Element = *(DataPtr.Get() + CurrentIndex);
			if (Predicate(Element))
			{
				RemoveAt(CurrentIndex, bKeepArraySize);

				--CurrentIndex;
			}
		}
	}

	void RemoveAll(bool bKeepArraySize = true)
	{
		for (UInt32 Index = 0; Index < ItemNum; ++Index)
		{
			TElement tempItem = TElement();
			SetAt(Index, tempItem);
		}

		ItemNum = 0;

		DecreaseArray(bKeepArraySize);
	}

	bool Contains(const TElement& Element) const
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

	template<typename TSize = SizeT>
	const TSize Num() const
	{
		return static_cast<TSize>(ItemNum);
	}

	bool IsEmpty() const
	{
		return ItemNum == 0;
	}

	TElement* GetData()
	{
		return DataPtr.Raw();
	}

	template<typename TReinterpretedElement>
	TReinterpretedElement* GetData()
	{
		TElement* Data = GetData();
		return reinterpret_cast<TReinterpretedElement*>(Data);
	}

	const TElement* GetData() const
	{
		return DataPtr.Raw();
	}

	template<typename TReinterpretedElement>
	const TReinterpretedElement* GetData() const
	{
		const TElement* Data = GetData();
		return reinterpret_cast<TReinterpretedElement*>(Data);
	}

	TElement& First()
	{
		return DataPtr[0];
	}

	const TElement& First() const
	{
		return DataPtr[0];
	}

	void Resize(SizeT NewSize, bool bApplyNewSizeToItemNum = false)
	{
		if (ArraySize != NewSize)
		{
			if (NewSize == 0)
			{
				DataPtr = nullptr;
				ItemNum = 0;
				ArraySize = 0;
				return;
			}

			ArrayType NewDataPtr = MakeShareable<TElement, ArrayDeleter>(new TElement[NewSize]);
			
			// must use assign so copy operators will be used
			for (SizeT Index = 0; Index < ItemNum; ++Index)
			{
				NewDataPtr[Index] = DataPtr[Index];
			}

			DataPtr = NewDataPtr;
			ArraySize = NewSize;
		}

		if (bApplyNewSizeToItemNum)
		{
			ItemNum = ArraySize;
		}
	}

	template<typename TCast>
	TArray<TCast> Cast(const TFunction<TCast(const TElement& Element)>& CastFunc) const
	{
		TArray<TCast> Casted(ItemNum, true);
		for (UInt32 Index = 0; Index < ItemNum; ++Index)
		{
			Casted.SetAt(Index, CastFunc(DataPtr[Index]));
		}
		return Casted;
	}

	template<typename TCast>
	TArray<TCast> Cast() const
	{
		TArray<TCast> Casted(ItemNum * sizeof(TElement), true);
		for (UInt32 Index = 0; Index < ItemNum; ++Index)
		{
			Casted.SetAt(Index, DataPtr[Index]);
		}
		return Casted;
	}

	void ForEach(const TFunction<void(TElement&)>& EachFunc)
	{
		for (UInt32 Index = 0; Index < ItemNum; ++Index)
		{
			EachFunc(DataPtr[Index]);
		}
	}

	void ForEach(const TFunction<void(const TElement&)>& EachFunc) const
	{
		for (UInt32 Index = 0; Index < ItemNum; ++Index)
		{
			EachFunc(DataPtr[Index]);
		}
	}

	void Reverse()
	{
		ArrayType NewDataPtr = MakeShareable<TElement, ArrayDeleter>(new TElement[ArraySize]);

		for (UInt32 IndexReversed = ItemNum - 1, Index = 0; Index < ItemNum; --IndexReversed, ++Index)
		{
			TElement& NewElement = NewDataPtr[IndexReversed];
			TElement& CurrentElement = DataPtr[Index];

			NewElement = CurrentElement;
		}

		DataPtr = NewDataPtr;
	}

	TElement& operator[](SizeT Index)
	{
		TElement& Element = *(DataPtr + Index);

		return Element;
	}

	const TElement& operator[](SizeT Index) const
	{
		const TElement& Element = *(DataPtr + Index);

		return Element;
	}

	TArray& operator=(const TArray& Other)
	{
		DataPtr = MakeShareable<TElement, ArrayDeleter>(new TElement[Other.ItemNum]);
		ItemNum = Other.ItemNum;
		ArraySize = Other.ItemNum;

		for (UInt64 Index = 0; Index < ItemNum; ++Index) 
		{
			SetAt(Index, Other.DataPtr[Index]);
		}

		return *this;
	}

	TElement* begin() const
	{
		return (TElement*)DataPtr;
	}

	TElement* end() const
	{
		return DataPtr + ItemNum;
	}
};
