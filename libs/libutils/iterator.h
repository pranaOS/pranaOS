#pragma once

#include <libutils/forward.h>

namespace Utils {

template <typename Container, typename ValueType>
class SimpleIterator {
public:
    friend Container;

    constexpr bool is_end() const { return m_index == SimpleIterator::end(m_container).m_index; }
    constexpr size_t index() const { return m_index; }

private:
    static constexpr SimpleIterator begin(Container& container) { return {container, 0}; }

    Container& m_container;
    size_t m_index;

};

}