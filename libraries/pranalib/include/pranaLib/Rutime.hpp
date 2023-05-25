//
//  Rutime.hpp
//  Libraries
//
//  Created by Pranav on 28/06/22.
//

#pragma once

template <typename T>
inline T&& move(T& arg) {
    return static_cast<T&&>(arg);
}

template <typename T>
struct __RemoveReference {
    using Type = T;
};

template <class T>
struct __RemoveReference<T&> {
    using Type = T;
};

template <class T>
struct __RemoveReference<T&&> {
    using Type = T;
};

template <typename T>
using RemoveReference = typename __RemoveReference<T>::Type;

template <typename T>
struct __MakeUnsigned {
    using Type = void;
};

template <>
struct __MakeUnsigned<signed char> {
    using Type = unsigned char;
};

template <>
struct __MakeUnsigned<short> {
    using Type = unsigned short;
};

template <>
struct __MakeUnsigned<int> {
    using Type = unsigned int;
};

template <>
struct __MakeUnsigned<long> {
    using Type = unsigned long;
};

template <>
struct __MakeUnsigned<long long> {
    using Type = unsigned long long;
};

template <>
struct __MakeUnsigned<unsigned char> {
    using Type = unsigned char;
};

template <>
struct __MakeUnsigned<unsigned short> {
    using Type = unsigned short;
};

template <>
struct __MakeUnsigned<unsigned int> {
    using Type = unsigned int;
};

template <>
struct __MakeUnsigned<unsigned long> {
    using Type = unsigned long;
};

template <>
struct __MakeUnsigned<unsigned long long> {
    using Type = unsigned long long;
};

template <>
struct __MakeUnsigned<char> {
    using Type = unsigned char;
};

template <>
struct __MakeUnsigned<bool> {
    using Type = bool;
};

template <typename T>
using MakeUnsigned = typename __MakeUnsigned<T>::Type;

template <class T>
struct __RemoveConst {
    using Type = T;
};

template <class T>
struct __RemoveConst<const T> {
    using Type = T;
};

template <class T>
using RemoveConst = typename __RemoveConst<T>::Type;

template <class T>
struct __RemoveVolatile {
    using Type = T;
};

template <class T>
struct __RemoveVolatile<volatile T> {
    using Type = T;
};

template <typename T>
using RemoveVolatile = typename __RemoveVolatile<T>::Type;

template <typename T>
inline constexpr bool __IsIntegral = false;

template <>
inline constexpr bool __IsIntegral<bool> = true;
template <>
inline constexpr bool __IsIntegral<unsigned char> = true;
template <>
inline constexpr bool __IsIntegral<unsigned short> = true;
template <>
inline constexpr bool __IsIntegral<unsigned int> = true;
template <>
inline constexpr bool __IsIntegral<unsigned long> = true;
template <>
inline constexpr bool __IsIntegral<unsigned long long> = true;

template <typename T>
inline constexpr bool IsIntegral = __IsIntegral<MakeUnsigned<RemoveVolatile<RemoveConst<T>>>>;

template <class T>
inline constexpr bool __IsPointer = false;

template <class T>
inline constexpr bool __IsPointer<T*> = true;

template <class T>
inline constexpr bool IsPointer = __IsPointer<RemoveVolatile<RemoveConst<T>>>;

template <class T>
static constexpr T&& forward(RemoveReference<T>& t) { return static_cast<T&&>(t); }

template <class T>
static constexpr T&& forward(RemoveReference<T>&& t) { return static_cast<T&&>(t); }

template <typename T, typename U>
inline constexpr bool IsSame = false;

template <typename T>
inline constexpr bool IsSame<T, T> = true;

extern "C" {

typedef void (*AtExitFunction)(void*);

int __cxa_atexit(AtExitFunction exit_function, void* parameter, void* dso_handle);
void __cxa_finalize(void* dso_handle);

int atexit(void (*handler)());
}
