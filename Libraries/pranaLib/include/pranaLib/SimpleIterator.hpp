//
//  SimpleIterator.hpp
//  Libraries
//
//  Created by Pranav on 14/07/22.
//

#pragma once

#include "Common.hpp"

template <typename Container, typename ValueType>
class SimpleIterator {
    friend Container;
    
public:
    
private:
    SimpleIterator(Container& container, size_t index)
        : m_container(container)
        , m_index(index)
    {
    }
    
private:
    Container& m_container;
    size_t m_index;
}
