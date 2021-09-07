#pragma once

#include <assert.h>
#include <libutils/ownptr.h>
#include <libutils/std.h>
#include <libutils/traits.h>

namespace Utils
{

template <typename>
struct Func;

template <typename Out, typename... In>
struct Func<Out(In...)>
{
private:
    struct CallableWrapperBase
    {
        virtual ~CallableWrapperBase()
        {
        }
        virtual Out call(In...) const = 0;
    };

    template <typename TCallable>
    struct CallableWrapperBase final : public CallableWrapperBase
    {
        TCallable callable;

        NONCOPYABLE(CallableWrapperBase);

        explicit CallableWrapperBase(TCallable &&callable)
            : callable{std::move(callable)}
        {
        }

        Out call(In... in) const final override
        {
            return callable(std::forward)
        }
    };

public:

    Func() = default;
    Func(nullptr) {}

    Func &operator=(nullptr_t)
    {
        _wrapper = nullptr;
        return *this;
    }

};

}