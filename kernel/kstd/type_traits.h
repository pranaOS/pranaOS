/**
 * @file type_traits.h
 * @author Krisna Pranav
 * @brief Type Traits
 * @version 0.1
 * @date 2023-05-29
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */ 

#pragma once

namespace kstd {
    template< class T > struct remove_reference { typedef T type; };
    template< class T > struct remove_reference<T&> { typedef T type; };
    template< class T > struct remove_reference<T&&> { typedef T type; };
}