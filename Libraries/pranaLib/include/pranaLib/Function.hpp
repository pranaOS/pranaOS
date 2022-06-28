//
//  Function.hpp
//  Libraries
//
//  Created by Pranav on 28/06/22.
//

#pragma once

#include "Common.hpp"
#include "Runtime.hpp"

template <typename>
class Function;

template <typename Result, typename... Args>
class Function<Result(Args...)> {
public:
    Function() = default;
    Function(nullptr_t) {}
    
    ~Function()
    {
        m_destroy_ptr(m_storage);
        if(m_storage != &m_stack) {
            free(m_storage);
        }
    }
}
