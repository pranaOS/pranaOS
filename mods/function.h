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
#include <mods/span.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>

namespace Mods 
{
    #if defined(MOD_COMPILER_CLANG)
    #    define ESCAPING [[clang::annotate("pranaos::escaping")]]
    
    #    define IGNORE_USE_IN_ESCAPING_LAMBDA [[clang::annotate("pranaos::ignore_use_in_escaping_lambda")]]
    #else
    #    define ESCAPING
    #    define IGNORE_USE_IN_ESCAPING_LAMBDA
    #endif

    template<typename>
    class Function;

    /**
     * @tparam F 
     */
    template<typename F>
    inline constexpr bool IsFunctionPointer = (IsPointer<F> && IsFunction<RemovePointer<F>>);

    /**
     * @tparam F 
     */
    template<typename F>
    inline constexpr bool IsFunctionObject = (!IsFunctionPointer<F> && IsRvalueReference<F&&>);

    /**
     * @tparam Out 
     * @tparam In 
     */
    template<typename Out, typename... In>
    class Function<Out(In...)> 
    {
        MOD_MAKE_NONCOPYABLE(Function);

    public:
        using FunctionType = Out(In...);
        using ReturnType = Out;

    #ifdef KERNEL
        constexpr static auto AccommodateExcessiveAlignmentRequirements = false;
        constexpr static size_t ExcessiveAlignmentThreshold = alignof(void*);
    #else
        constexpr static auto AccommodateExcessiveAlignmentRequirements = true;
        constexpr static size_t ExcessiveAlignmentThreshold = 16;
    #endif

        /**
         * @brief Construct a new Function object
         * 
         */
        Function() = default;

        /**
         * @brief Construct a new Function object
         * 
         */
        Function(nullptr_t)
        {
        }

        ~Function()
        {
            clear(false);
        }

        /**
         * @return ReadonlyBytes 
         */
        [[nodiscard]] ReadonlyBytes raw_capture_range() const
        {
            if (!m_size)
                return {};
            if (auto* wrapper = callable_wrapper())
                return ReadonlyBytes { wrapper, m_size };
            return {};
        }

        template<typename CallableType>
        Function(CallableType&& callable)
        requires((IsFunctionObject<CallableType> && IsCallableWithArguments<CallableType, Out, In...> && !IsSame<RemoveCVReference<CallableType>, Function>))
        {
            init_with_callable(forward<CallableType>(callable), CallableKind::FunctionObject);
        }

        template<typename FunctionType>
        Function(FunctionType f)
        requires((IsFunctionPointer<FunctionType> && IsCallableWithArguments<RemovePointer<FunctionType>, Out, In...> && !IsSame<RemoveCVReference<FunctionType>, Function>))
        {
            init_with_callable(move(f), CallableKind::FunctionPointer);
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

            ScopeGuard guard([this] {
                if (--m_call_nesting_level == 0 && m_deferred_clear)
                    const_cast<Function*>(this)->clear(false);
            });

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
        template<typename CallableType>
        Function& operator=(CallableType&& callable)
        requires((IsFunctionObject<CallableType> && IsCallableWithArguments<CallableType, Out, In...>))
        {
            clear();
            init_with_callable(forward<CallableType>(callable), CallableKind::FunctionObject);
            return *this;
        }

        /**
         * @tparam FunctionType 
         */
        template<typename FunctionType>
        Function& operator=(FunctionType f)
        requires((IsFunctionPointer<FunctionType> && IsCallableWithArguments<RemovePointer<FunctionType>, Out, In...>))
        {
            clear();
            if (f)
                init_with_callable(move(f), CallableKind::FunctionPointer);
            return *this;
        }

        /**
         * @return Function& 
         */
        Function& operator=(nullptr_t)
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
            if (this != &other) {
                clear();
                move_from(move(other));
            }
            return *this;
        }

    private:
        enum class CallableKind 
        {
            FunctionPointer,
            FunctionObject,
        }; // enum class CallableKind 

        class CallableWrapperBase 
        {
        public:
            virtual ~CallableWrapperBase() = default;
            
            virtual Out call(In...) = 0;
            virtual void destroy() = 0;
            virtual void init_and_swap(u8*, size_t) = 0;
        }; // class CallableWrapperBase 

        /**
         * @tparam CallableType 
         */
        template<typename CallableType>
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
                new (destination) CallableWrapper { move(m_callable) };
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
            switch (m_kind) {
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

            if (called_from_inside_function && may_defer) {
                m_deferred_clear = true;
                return;
            }

            m_deferred_clear = false;
            auto* wrapper = callable_wrapper();

            if (m_kind == FunctionKind::Inline) {
                VERIFY(wrapper);
                wrapper->~CallableWrapperBase();
            } else if (m_kind == FunctionKind::Outline) {
                VERIFY(wrapper);
                wrapper->destroy();
            }
            m_kind = FunctionKind::NullPointer;
        }

        /**
         * @tparam Callable 
         * @param callable 
         * @param callable_kind 
         */
        template<typename Callable>
        void init_with_callable(Callable&& callable, CallableKind callable_kind)
        {
            if constexpr (alignof(Callable) > ExcessiveAlignmentThreshold && !AccommodateExcessiveAlignmentRequirements) {
                static_assert(
                    alignof(Callable) <= ExcessiveAlignmentThreshold,
                    "This callable object has a very large alignment requirement, "
                    "check your capture list if it is a lambda expression, "
                    "and make sure your callable object is not excessively aligned.");
            }
            VERIFY(m_call_nesting_level == 0);
            using WrapperType = CallableWrapper<Callable>;
    #ifndef KERNEL
            if constexpr (alignof(Callable) > inline_alignment || sizeof(WrapperType) > inline_capacity) {
                *bit_cast<CallableWrapperBase**>(&m_storage) = new WrapperType(forward<Callable>(callable));
                m_kind = FunctionKind::Outline;
            } else {
    #endif
                static_assert(sizeof(WrapperType) <= inline_capacity);
                new (m_storage) WrapperType(forward<Callable>(callable));
                m_kind = FunctionKind::Inline;
    #ifndef KERNEL
            }
    #endif
            if (callable_kind == CallableKind::FunctionObject)
                m_size = sizeof(WrapperType);
            else
                m_size = 0;
        }

        /**
         * @param other 
         */
        void move_from(Function&& other)
        {
            VERIFY(m_call_nesting_level == 0 && other.m_call_nesting_level == 0);
            auto* other_wrapper = other.callable_wrapper();
            m_size = other.m_size;
            switch (other.m_kind) {
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

        size_t m_size { 0 };
        FunctionKind m_kind { FunctionKind::NullPointer };
        bool m_deferred_clear { false };
        mutable Atomic<u16> m_call_nesting_level { 0 };

        static constexpr size_t inline_alignment = max(alignof(CallableWrapperBase), alignof(CallableWrapperBase*));
    #ifndef KERNEL
        static constexpr size_t inline_capacity = 4 * sizeof(void*);
    #else
        static constexpr size_t inline_capacity = 6 * sizeof(void*);
    #endif

        alignas(inline_alignment) u8 m_storage[inline_capacity];
    }; // class Function<Out(In..)>

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::Function;
using Mods::IsCallableWithArguments;
#endif
