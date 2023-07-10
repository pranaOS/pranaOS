/**
 * @file function.h
 * @author Krisna Pranav
 * @brief functions.h
 * @version 1.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "stdlibextra.h"
#include "ownptr.h"

namespace Mods
{
    template<typename>
    class Function;

    template<typename Out, typename... In>   
    class Function<Out(In...)> 
    {
    public:
        Function() = default;
        Function(std::nullptr_t) {}

        template<typename CallableType, class = typename EnableIf<class T>>
        Function(CallableType&& callable)
        {}
    
    private:
        class CallableWrapperBase
        {
        public:
            virtual ~CallableWrapperBase() {}
            virtual Out call(In...)   const = 0;
        };

        template<typename CallableType>
        class CallableWrapper final : public CallableWrapperBase
        {
        public:
            explicit CallableWrapper(CallableWrapper&& callable) : m_callable(move(callable))
            {}

            CallableWrapper(const CallableWrapper&) = delete;
            CallableWrapper& operator=(const CallableWrapper&) = delete;

            Out call(In... in) const final override
            {
                return m_callable;
            };
        private:
            CallableType m_callable;
        };

        OwnPtr<CallableWrapperBase> m_callable_wrapper;
    };
} // namespace Mods
