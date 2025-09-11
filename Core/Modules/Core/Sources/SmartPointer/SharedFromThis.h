#pragma once

#include <type_traits>

struct IDeleter;
struct SRefCounter;

struct SSharedFromThis
{
	virtual ~SSharedFromThis() = default;
};

template<typename TPtr>
struct TWeakPtr;

template<typename TPtr>
struct TSharedPtr;

template<typename TThis>
struct TSharedFromThis : public SSharedFromThis
{
	TSharedFromThis() = default;
	~TSharedFromThis() override = default;

	template<typename TOtherPtr = TThis>
	inline TSharedPtr<TOtherPtr> AsShared()
	{
		if constexpr (std::is_same_v<TThis, TOtherPtr>)
		{
			return _weakPtr;
		}

		return TSharedPtr<TOtherPtr>(_weakPtr);
	}

	template<typename TOtherPtr = TThis>
	inline TWeakPtr<TOtherPtr> AsWeak()
	{
		if constexpr (std::is_same_v<TThis, TOtherPtr>)
		{
			return _weakPtr;
		}

		return TWeakPtr<TOtherPtr>(_weakPtr);
	}

protected:
	virtual void OnInitializeShared() { }

private:
	TWeakPtr<TThis> _weakPtr;

	inline void UpdateWeakPtr(IDeleter* InDeleter, SRefCounter* InRefCounter)
	{
		_weakPtr = TWeakPtr<TThis>(static_cast<TThis*>(this), InDeleter, InRefCounter);

		OnInitializeShared();
	}

	template<typename TPointer>
	friend struct TWeakPtr;

	template<typename TPointer>
	friend struct TSharedPtr;

	template<typename TPointer, typename ... TArgs>
	friend TSharedPtr<TPointer> MakeShared(TArgs...);

	template<typename TPointer, typename DeleterT>
	friend TSharedPtr<TPointer> MakeShareable(TPointer*);
};

#define FORWARD_DECLARE_SHARED(ClassName)										\
class C##ClassName;                                                             \
using C##ClassName##Ptr = TSharedPtr<C##ClassName>;                       		\
using C##ClassName##WeakPtr = TWeakPtr<C##ClassName>