#pragma once

#include <algorithm>

template<
    typename TReturn,
    typename ... TArgs
>
class CCallableBase
{
public:
    virtual ~CCallableBase() = default;
    virtual TReturn Invoke(TArgs ... Arguments) const = 0;
};

template<
    typename TCallable,
    typename TReturn,
    typename ... TArgs
>
class TCallableStorage : public CCallableBase<TReturn, TArgs...>
{
    TCallable Callable;

public:
    TCallableStorage(TCallable Callable) :
        Callable(std::move(Callable))
    {

    }

    TReturn Invoke(TArgs ... Args) const override
    {
        return Callable(std::forward<TArgs>(Args) ...);
    }

    TCallableStorage& operator=(TCallable Callable)
    {
        this->Callable = std::move(Callable);
        return *this;
    }
};

template<typename TPtr>
struct TSharedPtr;

template<
    typename TFunc
>
class TFunction;

template<
    typename TReturn,
    typename... TArgs
>
class TFunction<TReturn(TArgs...)>
{
    using TFunc = TReturn(TArgs&...);

    TSharedPtr<CCallableBase<TReturn, TArgs...>> CallableStorage;

public:
    TFunction(TFunc Func) :
        CallableStorage(MakeShared<TCallableStorage<TFunc, TReturn, TArgs...>>(Func))
    {
    }
    
    template<
        typename TCallable
    >
    TFunction(TCallable Callable) :
        CallableStorage(MakeShared<TCallableStorage<TCallable, TReturn, TArgs...>>(Callable))
    {
    }

    ~TFunction() = default;

    TReturn operator()(TArgs ... Args) const
    {
        return CallableStorage->Invoke(std::forward<TArgs>(Args) ...);
    }

    TFunction& operator=(TFunc Func)
    {
        CallableStorage = MakeShared<TCallableStorage<TFunc, TReturn, TArgs...>>(Func);

        return *this;
    }

    template<
        typename TCallable
    >
    TFunction& operator=(TCallable Callable)
    {
        CallableStorage = MakeShared<TCallableStorage<TCallable, TReturn, TArgs...>>(Callable);

        return *this;
    }
};

template<
    typename... TArgs
>
class TFunction<void(TArgs...)>
{
    using TFunc = void(TArgs&...);

    TSharedPtr<CCallableBase<void, TArgs...>> CallableStorage;

public:
    TFunction() : CallableStorage(nullptr)
    {
        //
    }

    TFunction(TFunc Func) : 
        CallableStorage(MakeShared<TCallableStorage<TFunc, void, TArgs...>>(Func))
    {
    }
    
    template<
        typename TCallable
    >
    TFunction(TCallable Callable)
    {
        if constexpr (!std::is_same_v<TCallable, std::nullptr_t>)
        {
            CallableStorage = MakeShared<TCallableStorage<TCallable, void, TArgs...>>(Callable);
        }
    }

    ~TFunction() = default;

    void operator()(TArgs ... Arguments) const
    {
        CallableStorage->Invoke(std::forward<TArgs>(Arguments) ...);
    }

    TFunction& operator=(TFunc Func)
	{
        CallableStorage = MakeShared<TCallableStorage<TFunc, void, TArgs...>>(Func);

		return *this;
	}

    template<
        typename TCallable
    >
    TFunction& operator=(TCallable Callable)
    {
        CallableStorage = MakeShared<TCallableStorage<TCallable, void, TArgs...>>(Callable);

        return *this;
    }

    operator bool() const
    {
        return CallableStorage;
    }
};
