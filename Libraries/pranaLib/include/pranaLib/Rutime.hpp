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
