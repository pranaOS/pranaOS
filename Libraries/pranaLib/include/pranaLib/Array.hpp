//
//  Array.hpp
//  Libraries
//
//  Created by Pranav on 28/06/22.
//

#pragma once

#include "Common.hpp"

template <typename T, size_t S>
class Array {
public:
    constexpr size_t size() const { return S; }

    T& operator[](size_t index)
    {
        return m_data[index];
    }

    const T& operator[](size_t index) const
    {
        return m_data[index];
    }

private:
    T m_data[S];
};
