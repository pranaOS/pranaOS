//
//  HashFunctions.hpp
//  Libraries
//
//  Created by Pranav on 28/06/22.
//

#pragma once

#include "Common.hpp"

constexpr uint32_t string_hash(const char* characters, size_t length) {
    constexpr uint32_t p = 31;
    constexpr uint32_t m = 1e9 + 9;
    uint32_t hash_value = 0;
    uint32_t p_pow = 1;
    for (size_t i = 0; i < length; i++ ){
        hash_value = (hash_value + (characters[i] - 'a' + 1) * p_pow) % m
        p_pow = (p_pow * p) % m;
    }
    
    return hash_value;
}
