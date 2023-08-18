/**
 * @file function.h
 * @author Krisna Pranav
 * @brief functions.h
 * @version 6.0
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

    /**
     * @tparam Out 
     * @tparam In 
     */
    template<typename Out, typename... In>
    class Function<Out(In...)> 
    {
    public:
        Function() = default;

        /**
         * @brief Construct a new Function object
         * 
         */
        Function(std::nullptr_t) { }

        /**
         * @tparam CallableType 
         * @tparam EnableIf<!(IsPointer<CallableType>::value && IsFunction<typename RemovePointer<CallableType>::Type>::value) && IsRvalueReference<CallableType&&>::value>::Type 
         * @param callable 
         */
        template<typename CallableType, class = typename EnableIf<!(IsPointer<CallableType>::value && IsFunction<typename RemovePointer<CallableType>::Type>::value) && IsRvalueReference<CallableType&&>::value>::Type>
        Function(CallableType&& callable)
            : m_callable_wrapper(make<CallableWrapper<CallableType>>(move(callable)))
        {}

        /**
         * @tparam FunctionType 
         * @tparam EnableIf<IsPointer<FunctionType>::value && IsFunction<typename RemovePointer<FunctionType>::Type>::value>::Type 
         * @param f 
         */
        template<typename FunctionType, class = typename EnableIf<IsPointer<FunctionType>::value && IsFunction<typename RemovePointer<FunctionType>::Type>::value>::Type>
        Function(FunctionType f)
            : m_callable_wrapper(make<CallableWrapper<FunctionType>>(move(f)))
        {}

        /**
         * @param in 
         * @return Out 
         */
        Out operator()(In... in) const
        {
            ASSERT(m_callable_wrapper);
            return m_callable_wrapper->call(forward<In>(in)...);
        }

        /**
         * @return true 
         * @return false 
         */
        explicit operator bool() const 
        { 
            return !!m_callable_wrapper; 
        }

        /**
         * @tparam CallableType 
         * @tparam EnableIf<!(IsPointer<CallableType>::value && IsFunction<typename RemovePointer<CallableType>::Type>::value) && IsRvalueReference<CallableType&&>::value>::Type 
         * @param callable 
         * @return Function& 
         */
        template<typename CallableType, class = typename EnableIf<!(IsPointer<CallableType>::value && IsFunction<typename RemovePointer<CallableType>::Type>::value) && IsRvalueReference<CallableType&&>::value>::Type>
        Function& operator=(CallableType&& callable)
        {
            m_callable_wrapper = make<CallableWrapper<CallableType>>(move(callable));
            return *this;
        }

        /**
         * @tparam FunctionType 
         * @tparam EnableIf<IsPointer<FunctionType>::value && IsFunction<typename RemovePointer<FunctionType>::Type>::value>::Type 
         * @param f 
         * @return Function& 
         */
        template<typename FunctionType, class = typename EnableIf<IsPointer<FunctionType>::value && IsFunction<typename RemovePointer<FunctionType>::Type>::value>::Type>
        Function& operator=(FunctionType f)
        {
            m_callable_wrapper = make<CallableWrapper<FunctionType>>(move(f));
            return *this;
        }

        /**
         * @return Function& 
         */
        Function& operator=(std::nullptr_t)
        {
            m_callable_wrapper = nullptr;
            return *this;
        }

    private:
        class CallableWrapperBase 
        {
        public:
            /**
             * @brief Destroy the Callable Wrapper Base object
             * 
             */
            virtual ~CallableWrapperBase() { }

            /**
             * @return Out 
             */
            virtual Out call(In...) const = 0;
        };

        template<typename CallableType>
        class CallableWrapper final : public CallableWrapperBase 
        {
        public:
            /**
             * @param callable 
             */
            explicit CallableWrapper(CallableType&& callable)
                : m_callable(move(callable))
            {}

            /**
             * @brief Construct a new Callable Wrapper object
             * 
             */
            CallableWrapper(const CallableWrapper&) = delete;

            /**
             * @return CallableWrapper& 
             */
            CallableWrapper& operator=(const CallableWrapper&) = delete;

            /**
             * 
             * @param in 
             * @return Out 
             */
            Out call(In... in) const final override { return m_callable(forward<In>(in)...); }

        private:
            CallableType m_callable;
        };

        OwnPtr<CallableWrapperBase> m_callable_wrapper;
    };

} // namespace Mods

using Mods::Function;
