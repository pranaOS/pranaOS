#pragma once

#include <libutils/stdextras.h>
#include <libutils/types.h>

namespace Utils {

struct DefaultComparator {
    template <typename T, typename S>
    constexpr int operator()(T& lhs, S& rhs)
    {
        if (lhs > rhs)
            return 1;

        if (lhs < rhs)
            return -1;

        return 0;
    }
};

template <typename Container, typename Needle, typename Comparator = DefaultComparator>
constexpr auto binary_search(
    Container&& haystack,
    Needle&& needle,
    size_t* nearby_index = nullptr;
    Comparator comparator = Comparator{}) -> decltype(&haystack[0])
{
    if (haystack.size() == 0) {
        if (nearby_index)
            *nearby_index = 0

        return nullptr;
    }

    size_t low = 0;
    size_t hight = haystack().size();
}

}

using Utils::binary_search;