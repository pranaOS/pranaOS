/**
 * @file stdlibextradetail.h
 * @author Krisna Pranav
 * @brief std lib extra
 * @version 6.0
 * @date 2024-08-21
 *
 * @copyright Copyright (c) 2021 - 2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

namespace Mods::Detail
{
    /**
     * @tparam T 
     * @tparam v 
     */
    template <class T, T v>
    struct IntegralConstant
    {
        static constexpr T value = v;
        using ValueType = T;
        using Type = IntegralConstant;
        
        /**
         * @return ValueType 
         */
        constexpr operator ValueType() const
        {
            return value;
        }
        constexpr ValueType operator()() const
        {
            return value;
        }
    }; // struct IntegralConstant

    using FalseType = IntegralConstant<bool, false>;
    using TrueType = IntegralConstant<bool, true>;

    /**
     * @tparam T 
     */
    template <class T>
    using AddConst = const T;

    /**
     * @tparam T 
     */
    template <class T>
    struct __AddConstToReferencedType
    {
        using Type = T;
    }; // struct __AddConstToReferencedType
 
    /**
     * @tparam T 
     */
    template <class T>
    struct __AddConstToReferencedType<T&>
    {
        using Type = AddConst<T>&;
    }; // struct __AddConstToReferencedType<T&>

    /**
     * @tparam T 
     */
    template <class T>
    struct __AddConstToReferencedType<T&&>
    {
        using Type = AddConst<T>&&;
    }; // struct __AddConstToReferencedType<T&&>

    /**
     * @tparam T 
     */
    template <class T>
    using AddConstToReferencedType = typename __AddConstToReferencedType<T>::Type;

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemoveConst
    {
        using Type = T;
    }; // struct __RemoveConst

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemoveConst<const T>
    {
        using Type = T;
    }; // struct __RemoveConst<const T>

    /**
     * @tparam T 
     */
    template <class T>
    using RemoveConst = typename __RemoveConst<T>::Type;

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemoveVolatile
    {
        using Type = T;
    }; // struct __RemoveVolatile

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemoveVolatile<volatile T>
    {
        using Type = T;
    };

    /**
     * @tparam T 
     */
    template <typename T>
    using RemoveVolatile = typename __RemoveVolatile<T>::Type;

    /**
     * @tparam T 
     */
    template <class T>
    using RemoveCV = RemoveVolatile<RemoveConst<T>>;

    /**
     * @tparam  
     */
    template <typename...>
    using VoidType = void;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool IsLvalueReference = false;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool IsLvalueReference<T&> = true;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool __IsPointerHelper = false;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool __IsPointerHelper<T*> = true;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool IsPointer = __IsPointerHelper<RemoveCV<T>>;

    template <class>
    inline constexpr bool IsFunction = false;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...)> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...)> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) const> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) const> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) volatile> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) volatile> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) const volatile> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) const volatile> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...)&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...)&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) const&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) const&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) volatile&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) volatile&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) const volatile&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) const volatile&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) &&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) &&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) const&&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) const&&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) volatile&&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) volatile&&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args...) const volatile&&> = true;
    template <class Ret, class... Args>
    inline constexpr bool IsFunction<Ret(Args..., ...) const volatile&&> = true;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool IsRvalueReference = false;
    template <class T>
    inline constexpr bool IsRvalueReference<T&&> = true;

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemovePointer
    {
        using Type = T;
    }; // struct __RemovePointer

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemovePointer<T*>
    {
        using Type = T;
    }; // struct __RemovePointer<T*>

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemovePointer<T* const>
    {
        using Type = T;
    }; // struct __RemovePointer<T* const>

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemovePointer<T* volatile>
    {
        using Type = T;
    }; // struct __RemovePointer<T* volatile>

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemovePointer<T* const volatile>
    {
        using Type = T;
    }; // struct __RemovePointer<T* const volatile>

    /**
     * @tparam T 
     */
    template <typename T>
    using RemovePointer = typename __RemovePointer<T>::Type;

    /**
     * @tparam T 
     * @tparam U 
     */
    template <typename T, typename U>
    inline constexpr bool IsSame = false;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsSame<T, T> = true;

    /**
     * @tparam condition 
     * @tparam TrueType 
     * @tparam FalseType 
     */
    template <bool condition, class TrueType, class FalseType>
    struct __Conditional
    {
        using Type = TrueType;
    }; // struct __Conditional

    /**
     * @tparam TrueType 
     * @tparam FalseType 
     */
    template <class TrueType, class FalseType>
    struct __Conditional<false, TrueType, FalseType>
    {
        using Type = FalseType;
    };

    /**
     * @tparam condition 
     * @tparam TrueType 
     * @tparam FalseType 
     */
    template <bool condition, class TrueType, class FalseType>
    using Conditional = typename __Conditional<condition, TrueType, FalseType>::Type;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsNullPointer = IsSame<decltype(nullptr), RemoveCV<T>>;

    /**
     * @tparam T 
     */
    template <typename T>
    struct __RemoveReference
    {
        using Type = T;
    }; // struct __RemoveReference

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemoveReference<T&>
    {
        using Type = T;
    }; // struct __RemoveReference<T&>

    /**
     * @tparam T 
     */
    template <class T>
    struct __RemoveReference<T&&>
    {
        using Type = T;
    }; // struct __RemoveReference<T&&>

    /**
     * @tparam T 
     */
    template <typename T>
    using RemoveReference = typename __RemoveReference<T>::Type;

    /**
     * @tparam T 
     */
    template <typename T>
    using RemoveCVReference = RemoveCV<RemoveReference<T>>;
    
    /**
     * @tparam T 
     */
    template <typename T>
    struct __MakeUnsigned
    {
        using Type = void;
    }; // struct __MakeUnsigned

    /**
     * @tparam  
     */
    template <>
    struct __MakeUnsigned<signed char>
    {
        using Type = unsigned char;
    }; // struct __MakeUnsigned<signed char>
    
    template <>
    struct __MakeUnsigned<short>
    {
        using Type = unsigned short;
    }; // struct __MakeUnsigned<short>

    template <>
    struct __MakeUnsigned<int>
    {
        using Type = unsigned int;
    }; // struct __MakeUnsigned<int>

    template <>
    struct __MakeUnsigned<long>
    {
        using Type = unsigned long;
    }; // struct __MakeUnsigned<long>

    template <>
    struct __MakeUnsigned<long long>
    {
        using Type = unsigned long long;
    }; // struct __MakeUnsigned<long long>

    template <>
    struct __MakeUnsigned<unsigned char>
    {
        using Type = unsigned char;
    }; // struct __MakeUnsigned<unsigned char>

    template <>
    struct __MakeUnsigned<unsigned short>
    {
        using Type = unsigned short;
    }; // struct __MakeUnsigned<unsigned short>

    template <>
    struct __MakeUnsigned<unsigned int>
    {
        using Type = unsigned int;
    }; // struct __MakeUnsigned<unsigned int>

    template <>
    struct __MakeUnsigned<unsigned long>
    {
        using Type = unsigned long;
    }; // struct __MakeUnsigned<unsigned long>

    template <>
    struct __MakeUnsigned<unsigned long long>
    {
        using Type = unsigned long long;
    }; // struct __MakeUnsigned<unsigned long long>

    template <>
    struct __MakeUnsigned<char>
    {
        using Type = unsigned char;
    }; // struct __MakeUnsigned<char>

    /**
     * @tparam  
     */
    template <>
    struct __MakeUnsigned<char8_t>
    {
        using Type = char8_t;
    }; // struct __MakeUnsigned<char8_t>

    
    template <>
    struct __MakeUnsigned<char16_t>
    {
        using Type = char16_t;
    }; // struct __MakeUnsigned<char16_t>

    template <>
    struct __MakeUnsigned<char32_t>
    {
        using Type = char32_t;
    }; // struct __MakeUnsigned<char32_t>

    template <>
    struct __MakeUnsigned<bool>
    {
        using Type = bool;
    }; // struct __MakeUnsigned<bool>

    /**
     * @tparam T 
     */
    template <typename T>
    using MakeUnsigned = typename __MakeUnsigned<T>::Type;

    /**
     * @tparam T 
     */
    template <typename T>
    struct __MakeSigned
    {
        using Type = void;
    };

    template <>
    struct __MakeSigned<signed char>
    {
        using Type = signed char;
    }; // struct __MakeSigned<signed char>

    template <>
    struct __MakeSigned<short>
    {
        using Type = short;
    }; // struct __MakeSigned<short>

    template <>
    struct __MakeSigned<int>
    {
        using Type = int;
    }; // struct __MakeSigned<int>

    template <>
    struct __MakeSigned<long>
    {
        using Type = long;
    }; // struct __MakeSigned<long>

    template <>
    struct __MakeSigned<long long>
    {
        using Type = long long;
    }; // struct __MakeSigned<long long>

    template <>
    struct __MakeSigned<unsigned char>
    {
        using Type = char;
    }; // struct __MakeSigned<unsigned char>
    
    template <>
    struct __MakeSigned<unsigned short>
    {
        using Type = short;
    }; // struct __MakeSigned<unsigned short>

    template <>
    struct __MakeSigned<unsigned int>
    {
        using Type = int;
    }; // struct __MakeSigned<unsigned int>

    template <>
    struct __MakeSigned<unsigned long>
    {
        using Type = long; 
    }; // struct __MakeSigned<unsigned long>

    template <>
    struct __MakeSigned<unsigned long long>
    {
        using Type = long long;
    }; // struct __MakeSigned<unsigned long long>

    template <>
    struct __MakeSigned<char>
    {
        using Type = char;
    }; // struct __MakeSigned<char>

    /**
     * @tparam T 
     */
    template <typename T>
    using MakeSigned = typename __MakeSigned<T>::Type;

    /**
     * @tparam T 
     * @return T 
     */
    template <typename T>
    auto declval() -> T;

    /**
     * @tparam  
     */
    template <typename...>
    struct __CommonType;

    /**
     * @tparam T 
     */
    template <typename T>
    struct __CommonType<T>
    {
        using Type = T;
    }; // struct __CommonType<T>

    /**
     * @tparam T1 
     * @tparam T2 
     */
    template <typename T1, typename T2>
    struct __CommonType<T1, T2>
    {
        using Type = decltype(true ? declval<T1>() : declval<T2>());
    }; // struct __CommonType<T1, T2>

    /**
     * @tparam T1 
     * @tparam T2 
     * @tparam Ts 
     */
    template <typename T1, typename T2, typename... Ts>
    struct __CommonType<T1, T2, Ts...>
    {
        using Type = typename __CommonType<typename __CommonType<T1, T2>::Type, Ts...>::Type;
    };

    /**
     * @tparam Ts 
     */
    template <typename... Ts>
    using CommonType = typename __CommonType<Ts...>::Type;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool IsVoid = IsSame<void, RemoveCV<T>>;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool IsConst = false;

    /**
     * @tparam T 
     */
    template <class T>
    inline constexpr bool IsConst<const T> = true;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsEnum = __is_enum(T);

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsUnion = __is_union(T);

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsClass = __is_class(T);

    /**
     * @tparam Base 
     * @tparam Derived 
     */
    template <typename Base, typename Derived>
    inline constexpr bool IsBaseOf = __is_base_of(Base, Derived);

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool __IsIntegral = false;

    template <>
    inline constexpr bool __IsIntegral<bool> = true;
    template <>
    inline constexpr bool __IsIntegral<unsigned char> = true;
    template <>
    inline constexpr bool __IsIntegral<char8_t> = true;
    template <>
    inline constexpr bool __IsIntegral<char16_t> = true;
    template <>
    inline constexpr bool __IsIntegral<char32_t> = true;
    template <>
    inline constexpr bool __IsIntegral<unsigned short> = true;
    template <>
    inline constexpr bool __IsIntegral<unsigned int> = true;
    template <>
    inline constexpr bool __IsIntegral<unsigned long> = true;
    template <>
    inline constexpr bool __IsIntegral<unsigned long long> = true;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsIntegral = __IsIntegral<MakeUnsigned<RemoveCV<T>>>;

    template <typename T>
    inline constexpr bool __IsFloatingPoint = false;
    template <>
    inline constexpr bool __IsFloatingPoint<float> = true;
    template <>
    inline constexpr bool __IsFloatingPoint<double> = true;
    template <>
    inline constexpr bool __IsFloatingPoint<long double> = true;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsFloatingPoint = __IsFloatingPoint<RemoveCV<T>>;

    /**
     * @tparam ReferenceType 
     * @tparam T 
     */
    template <typename ReferenceType, typename T>
    using CopyConst = Conditional<IsConst<ReferenceType>, AddConst<T>, RemoveConst<T>>;

    /**
     * @tparam Ts 
     */
    template <typename... Ts>
    using Void = void;

    /**
     * @tparam _Ignored 
     */
    template <typename... _Ignored>
    constexpr auto DependentFalse = false;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsSigned = IsSame<T, MakeSigned<T>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsUnsigned = IsSame<T, MakeUnsigned<T>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsArithmetic = IsIntegral<T> || IsFloatingPoint<T>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsFundamental = IsArithmetic<T> || IsVoid<T> || IsNullPointer<T>;

    /**
     * @tparam T 
     * @tparam Ts 
     */
    template <typename T, T... Ts>
    struct IntegerSequence
    {
        using Type = T;
        static constexpr unsigned size() noexcept
        {
            return sizeof...(Ts);
        };
    }; // struct IntegerSequence

    /**
     * @tparam Indices 
     */
    template <unsigned... Indices>
    using IndexSequence = IntegerSequence<unsigned, Indices...>;

    /**
     * @tparam T 
     * @tparam N 
     * @tparam Ts 
     * @return auto 
     */
    template <typename T, T N, T... Ts>
    auto make_integer_sequence_impl()
    {
        if constexpr(N == 0)
            return IntegerSequence<T, Ts...>{};
        else
            return make_integer_sequence_impl<T, N - 1, N - 1, Ts...>();
    }

    /**
     * @tparam T 
     * @tparam N 
     */
    template <typename T, T N>
    using MakeIntegerSequence = decltype(make_integer_sequence_impl<T, N>());

    /**
     * @tparam N 
     */
    template <unsigned N>
    using MakeIndexSequence = MakeIntegerSequence<unsigned, N>;

    /**
     * @tparam T 
     */
    template <typename T>
    struct __IdentityType
    {
        using Type = T;
    }; // struct __IdentityType

    /**
     * @tparam T 
     */
    template <typename T>
    using IdentityType = typename __IdentityType<T>::Type;

    /**
     * @tparam T 
     * @tparam typename 
     */
    template <typename T, typename = void>
    struct __AddReference
    {
        using LvalueType = T;
        using TvalueType = T;
    }; // struct __AddReference

    /**
     * @tparam T 
     */
    template <typename T>
    struct __AddReference<T, VoidType<T&>>
    {
        using LvalueType = T&;
        using RvalueType = T&&;
    }; // struct __AddReference<T, VoidType<T&>>

    /**
     * @tparam T 
     */
    template <typename T>
    using AddLvalueReference = typename __AddReference<T>::LvalueType;

    /**
     * @tparam T 
     */
    template <typename T>
    using AddRvalueReference = typename __AddReference<T>::RvalueType;

    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     */
    template <class T>
        requires(IsEnum<T>)
    using UnderlyingType = __underlying_type(T);

    /**
     * @tparam T 
     * @tparam ExpectedSize 
     * @tparam ActualSize 
     */
    template <typename T, unsigned ExpectedSize, unsigned ActualSize>
    struct __AssertSize : TrueType
    {
        static_assert(ActualSize == ExpectedSize,
                    "actual size does not match expected size");

        consteval explicit operator bool() const
        {
            return value;
        }
    };

    /**
     * @tparam T 
     * @tparam ExpectedSize 
     */
    template <typename T, unsigned ExpectedSize>
    using AssertSize = __AssertSize<T, ExpectedSize, sizeof(T)>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsPOD = __is_pod(T);

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsTrivial = __is_trivial(T);

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsTriviallyCopyable = __is_trivially_copyable(T);

    /**
     * @tparam T 
     * @tparam Args 
     */
    template <typename T, typename... Args>
    inline constexpr bool IsCallableWithArguments = requires(T t) { t(declval<Args>()...); };

    /**
     * @tparam T 
     * @tparam Args 
     */
    template <typename T, typename... Args>
    inline constexpr bool IsConstructible = requires { ::new T(declval<Args>()...); };

    /**
     * @tparam T 
     * @tparam Args 
     */
    template <typename T, typename... Args>
    inline constexpr bool IsTriviallyConstructible = __is_trivially_constructible(T, Args...);

    /**
     * @tparam From 
     * @tparam To 
     */
    template <typename From, typename To>
    inline constexpr bool IsConvertible = requires { declval<void (*)(To)>()(declval<From>()); };

    /**
     * @tparam T 
     * @tparam U 
     */
    template <typename T, typename U>
    inline constexpr bool IsAssignable = requires { declval<T>() = declval<U>(); };

    /**
     * @tparam T 
     * @tparam U 
     */
    template <typename T, typename U>
    inline constexpr bool IsTriviallyAssignable = __is_trivially_assignable(T, U);

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsDestructible = requires { declval<T>().~T(); };

    template <typename T>
    #if defined(__clang__)
    inline constexpr bool IsTriviallyDestructible = __is_trivially_destructible(T);
    #else
    inline constexpr bool IsTriviallyDestructible = __has_trivial_destructor(T) && IsDestructible<T>;
    #endif

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsCopyConstructible = IsConstructible<T, AddLvalueReference<AddConst<T>>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsTriviallyCopyConstructible = IsTriviallyConstructible<T, AddLvalueReference<AddConst<T>>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsCopyAssignable = IsAssignable<AddLvalueReference<T>, AddLvalueReference<AddConst<T>>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsTriviallyCopyAssignable = IsTriviallyAssignable<AddLvalueReference<T>, AddLvalueReference<AddConst<T>>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsMoveConstructible = IsConstructible<T, AddRvalueReference<T>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsTriviallyMoveConstructible = IsTriviallyConstructible<T, AddRvalueReference<T>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsMoveAssignable = IsAssignable<AddLvalueReference<T>, AddRvalueReference<T>>;

    /**
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool IsTriviallyMoveAssignable = IsTriviallyAssignable<AddLvalueReference<T>, AddRvalueReference<T>>;

    /** 
     * @tparam T 
     * @tparam U 
     */
    template <typename T, template <typename...> typename U>
    inline constexpr bool IsSpecializationOf = false;

    /**
     * @tparam U 
     * @tparam Us 
     */
    template <template <typename...> typename U, typename... Us>
    inline constexpr bool IsSpecializationOf<U<Us...>, U> = true;

    /**
     * @tparam T 
     */
    template <typename T>
    struct __decay
    {
        typedef Detail::RemoveCVReference<T> type;
    }; // struct __decay

    /**
     * @tparam T 
     */
    template <typename T>
    struct __decay<T[]>
    {
        typedef T* type;
    }; // struct __decay<T[]>

    /**
     * @tparam T 
     * @tparam N 
     */
    template <typename T, decltype(sizeof(T)) N>
    struct __decay<T[N]>
    {
        typedef T* type;
    }; // struct __decay<T[N]>
    
    template <typename T>
    using Decay = typename __decay<T>::type;

    /**
     * @tparam T 
     * @tparam U 
     */
    template <typename T, typename U>
    inline constexpr bool IsPointerOfType = IsPointer<Decay<U>> && IsSame<T, RemoveCV<RemovePointer<Decay<U>>>>;

    /**
     * @tparam T 
     * @tparam U 
     */
    template <typename T, typename U>
    inline constexpr bool IsHashCompatible = false;
    template <typename T>
    inline constexpr bool IsHashCompatible<T, T> = true;

    /**
     * @tparam T 
     * @tparam Ts 
     */
    template <typename T, typename... Ts>
    inline constexpr bool IsOneOf = (IsSame<T, Ts> || ...);

    /**
     * @tparam T 
     * @tparam U 
     */
    template <typename T, typename U>
    inline constexpr bool IsSameIgnoringCV = IsSame<RemoveCV<T>, RemoveCV<U>>;

    /**
     * @tparam T 
     * @tparam Ts 
     */
    template <typename T, typename... Ts>
    inline constexpr bool IsOneOfIgnoringCV = (IsSameIgnoringCV<T, Ts> || ...);

} // namespace Mods::Detail

using Mods::Detail::AddConst;
using Mods::Detail::AddConstToReferencedType;
using Mods::Detail::AddLvalueReference;
using Mods::Detail::AddRvalueReference;
using Mods::Detail::AssertSize;
using Mods::Detail::CommonType;
using Mods::Detail::Conditional;
using Mods::Detail::CopyConst;
using Mods::Detail::declval;
using Mods::Detail::DependentFalse;
using Mods::Detail::FalseType;
using Mods::Detail::IdentityType;
using Mods::Detail::IndexSequence;
using Mods::Detail::IntegerSequence;
using Mods::Detail::IsArithmetic;
using Mods::Detail::IsAssignable;
using Mods::Detail::IsBaseOf;
using Mods::Detail::IsCallableWithArguments;
using Mods::Detail::IsClass;
using Mods::Detail::IsConst;
using Mods::Detail::IsConstructible;
using Mods::Detail::IsConvertible;
using Mods::Detail::IsCopyAssignable;
using Mods::Detail::IsCopyConstructible;
using Mods::Detail::IsDestructible;
using Mods::Detail::IsEnum;
using Mods::Detail::IsFloatingPoint;
using Mods::Detail::IsFunction;
using Mods::Detail::IsFundamental;
using Mods::Detail::IsHashCompatible;
using Mods::Detail::IsIntegral;
using Mods::Detail::IsLvalueReference;
using Mods::Detail::IsMoveAssignable;
using Mods::Detail::IsMoveConstructible;
using Mods::Detail::IsNullPointer;
using Mods::Detail::IsOneOf;
using Mods::Detail::IsOneOfIgnoringCV;
using Mods::Detail::IsPOD;
using Mods::Detail::IsPointer;
using Mods::Detail::IsRvalueReference;
using Mods::Detail::IsSame;
using Mods::Detail::IsSameIgnoringCV;
using Mods::Detail::IsSigned;
using Mods::Detail::IsSpecializationOf;
using Mods::Detail::IsTrivial;
using Mods::Detail::IsTriviallyAssignable;
using Mods::Detail::IsTriviallyConstructible;
using Mods::Detail::IsTriviallyCopyable;
using Mods::Detail::IsTriviallyCopyAssignable;
using Mods::Detail::IsTriviallyCopyConstructible;
using Mods::Detail::IsTriviallyDestructible;
using Mods::Detail::IsTriviallyMoveAssignable;
using Mods::Detail::IsTriviallyMoveConstructible;
using Mods::Detail::IsUnion;
using Mods::Detail::IsUnsigned;
using Mods::Detail::IsVoid;
using Mods::Detail::MakeIndexSequence;
using Mods::Detail::MakeIntegerSequence;
using Mods::Detail::MakeSigned;
using Mods::Detail::MakeUnsigned;
using Mods::Detail::RemoveConst;
using Mods::Detail::RemoveCV;
using Mods::Detail::RemoveCVReference;
using Mods::Detail::RemovePointer;
using Mods::Detail::RemoveReference;
using Mods::Detail::RemoveVolatile;
using Mods::Detail::TrueType;
using Mods::Detail::UnderlyingType;
using Mods::Detail::Void;