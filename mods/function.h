/**
 * @file function.h
 * @author Krisna Pranav
 * @brief functions.h
 * @version 6.0
 * @date 2023-07-10
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/bitcast.h>
#include <mods/noncopyable.h>
#include <mods/scopeguard.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam ypename 
     */
    template <typename>
    class Function;

    /**
     * @tparam F 
     */
    template <typename F>
    inline constexpr bool IsFunctionPointer = (IsPointer<F> && IsFunction<RemovePointer<F>>);

    /**
     * @tparam F 
     */
    template <typename F>
    inline constexpr bool IsFunctionObject = (!IsFunctionPointer<F> && IsRvalueReference<F&&>);

    /**
     * @tparam Out 
     * @tparam In 
     */
    template <typename Out, typename... In>
    class Function<Out(In...)>
    {
        MOD_MAKE_NONCOPYABLE(Function);

    public:
        /**
         * @brief Construct a new Function object
         * 
         */
        Function() = default;

        /**
         * @brief Construct a new Function object
         * 
         */
        Function(std::nullptr_t)
        {
        }

        /**
         * @brief Destroy the Function object
         * 
         */
        ~Function()
        {
            clear(false);
        }

        /**
         * @tparam CallableType 
         */
        template <typename CallableType>
        Function(CallableType&& callable)
            requires((IsFunctionObject<CallableType> && IsCallableWithArguments<CallableType, In...> && !IsSame<RemoveCVReference<CallableType>, Function>))
        {
            init_with_callable(forward<CallableType>(callable));
        }

        /**
         * @tparam FunctionType 
         */
        template <typename FunctionType>
        Function(FunctionType f)
            requires((IsFunctionPointer<FunctionType> && IsCallableWithArguments<RemovePointer<FunctionType>, In...> && !IsSame<RemoveCVReference<FunctionType>, Function>))
        {
            init_with_callable(move(f));
        }   

        /**
         * @brief Construct a new Function object
         * 
         * @param other 
         */
        Function(Function&& other)
        {
            move_from(move(other));
        }

        /**
         * @param in 
         * @return Out 
         */
        Out operator()(In... in) const
        {
            auto* wrapper = callable_wrapper();
            VERIFY(wrapper);
            ++m_call_nesting_level;

            ScopeGuard guard([this]
                            {
                if (--m_call_nesting_level == 0 && m_deferred_clear)
                    const_cast<Function*>(this)->clear(false); });

            return wrapper->call(forward<In>(in)...);
        }

        /**
         * @return true 
         * @return false 
         */
        explicit operator bool() const
        {
            return !!callable_wrapper();
        }

        /**
         * @tparam CallableType 
         */
        template <typename CallableType>
        Function& operator=(CallableType&& callable)
            requires((IsFunctionObject<CallableType> && IsCallableWithArguments<CallableType, In...>))
        {
            clear();
            init_with_callable(forward<CallableType>(callable));
            return *this;
        }

        /**
         * @tparam FunctionType 
         */
        template <typename FunctionType>
        Function& operator=(FunctionType f)
            requires((IsFunctionPointer<FunctionType> && IsCallableWithArguments<RemovePointer<FunctionType>, In...>))
        {
            clear();
            if(f)
                init_with_callable(move(f));
            return *this;
        }

        /**
         * @return Function& 
         */
        Function& operator=(std::nullptr_t)
        {
            clear();
            return *this;
        }

        /**
         * @param other 
         * @return Function& 
         */
        Function& operator=(Function&& other)
        {
            if(this != &other)
            {
                clear();
                move_from(move(other));
            }
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
            virtual ~CallableWrapperBase() = default;

            /**
             * @return Out 
             */
            virtual Out call(In...) = 0;

            virtual void destroy() = 0;

            virtual void init_and_swap(u8*, size_t) = 0;
        }; // class CallableWrapperBase

        /**
         * @tparam CallableType 
         */
        template <typename CallableType>
        class CallableWrapper final : public CallableWrapperBase
        {
            MOD_MAKE_NONMOVABLE(CallableWrapper);
            MOD_MAKE_NONCOPYABLE(CallableWrapper);

        public:
            /**
             * @brief Construct a new Callable Wrapper object
             * 
             * @param callable 
             */
            explicit CallableWrapper(CallableType&& callable)
                : m_callable(move(callable))
            {
            }

            /**
             * @param in 
             * @return Out 
             */
            Out call(In... in) final override
            {
                return m_callable(forward<In>(in)...);
            }

            void destroy() final override
            {
                delete this;
            }

            /**
             * @param destination 
             * @param size 
             */
            void init_and_swap(u8* destination, size_t size) final override
            {
                VERIFY(size >= sizeof(CallableWrapper));
                new(destination) CallableWrapper{move(m_callable)};
            }

        private:
            CallableType m_callable;
        }; // class CallableWrapper final : public CallableWrapperBase

        enum class FunctionKind
        {
            NullPointer,
            Inline,
            Outline,
        }; // enum class FunctionKind

        /**
         * @return CallableWrapperBase* 
         */
        CallableWrapperBase* callable_wrapper() const
        {
            switch(m_kind)
            {
            case FunctionKind::NullPointer:
                return nullptr;
            case FunctionKind::Inline:
                return bit_cast<CallableWrapperBase*>(&m_storage);
            case FunctionKind::Outline:
                return *bit_cast<CallableWrapperBase**>(&m_storage);
            default:
                VERIFY_NOT_REACHED();
            }
        }

        /**
         * @param may_defer 
         */
        void clear(bool may_defer = true)
        {
            bool called_from_inside_function = m_call_nesting_level > 0;
           
            VERIFY(may_defer || !called_from_inside_function);

            if(called_from_inside_function && may_defer)
            {
                m_deferred_clear = true;
                return;
            }

            m_deferred_clear = false;
            auto* wrapper = callable_wrapper();

            if(m_kind == FunctionKind::Inline)
            {
                VERIFY(wrapper);
                wrapper->~CallableWrapperBase();
            }
            else if(m_kind == FunctionKind::Outline)
            {
                VERIFY(wrapper);
                wrapper->destroy();
            }
            m_kind = FunctionKind::NullPointer;
        }   

        /**
         * @tparam Callable 
         * @param callable 
         */
        template <typename Callable>
        void init_with_callable(Callable&& callable)
        {
            VERIFY(m_call_nesting_level == 0);
            using WrapperType = CallableWrapper<Callable>;
            if constexpr(sizeof(WrapperType) > inline_capacity)
            {
                *bit_cast<CallableWrapperBase**>(&m_storage) = new WrapperType(forward<Callable>(callable));
                m_kind = FunctionKind::Outline;
            }
            else
            {
                new(m_storage) WrapperType(forward<Callable>(callable));
                m_kind = FunctionKind::Inline;
            }
        }

        /**
         * @param other 
         */
        void move_from(Function&& other)
        {
            VERIFY(m_call_nesting_level == 0 && other.m_call_nesting_level == 0);
            auto* other_wrapper = other.callable_wrapper();
            switch(other.m_kind)
            {
            case FunctionKind::NullPointer:
                break;
            case FunctionKind::Inline:
                other_wrapper->init_and_swap(m_storage, inline_capacity);
                m_kind = FunctionKind::Inline;
                break;
            case FunctionKind::Outline:
                *bit_cast<CallableWrapperBase**>(&m_storage) = other_wrapper;
                m_kind = FunctionKind::Outline;
                break;
            default:
                VERIFY_NOT_REACHED();
            }
            other.m_kind = FunctionKind::NullPointer;
        }

        FunctionKind m_kind{FunctionKind::NullPointer};
        bool m_deferred_clear{false};
        mutable Atomic<u16> m_call_nesting_level{0};
        static constexpr size_t inline_capacity = 4 * sizeof(void*);
        alignas(max(alignof(CallableWrapperBase), alignof(CallableWrapperBase*))) u8 m_storage[inline_capacity];
    }; // class Function<Out(In...)>
} // namespace Mods

using Mods::Function;
