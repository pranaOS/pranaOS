/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Forward.h>
#include <base/HashMap.h>
#include <base/OwnPtr.h>
#include <base/Types.h>

namespace Base {

namespace Detail {

template<typename TypeA, typename Default>
struct SubstituteIfVoid {
    using Type = TypeA;
};

template<typename Default>
struct SubstituteIfVoid<void, Default> {
    using Type = Default;
};

template<typename DeclaredBaseType, typename DefaultBaseType, typename ValueType, typename MetadataT, typename ValueTraits>
class Trie {
    using BaseType = typename SubstituteIfVoid<DeclaredBaseType, DefaultBaseType>::Type;

    class ConstIterator {
    
    public:
        static ConstIterator end() { return {}; }

        bool operator==(const ConstIterator& other) const { return m_current_node == other.m_current_node; }

        const BaseType& operator*() const { return static_cast<const BaseType&>(*m_current_node); }
        const BaseType* operator->() const { return static_cast<const BaseType*>(m_current_node); }
        void operator++() { skip_to_next(); }

        explicit ConstIterator(const Trie& node)
        {
            m_current_node = &node;
            m_state.empend(false, node.m_children.begin(), node.m_children.end());
        }


    }    
}

}

}