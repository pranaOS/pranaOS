/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <base/NonnullRefPtrVector.h>
#include <base/RefCounted.h>
#include <base/String.h>
#include <base/Vector.h>

namespace Web::CSS {

using SelectorList = NonnullRefPtrVector<class Selector>;

class Selector : public RefCounted<Selector> {
public:
    struct SimpleSelector {
        enum class Type {
            Invalid,
            Universal,
            TagName,
            Id,
            Class,
            Attribute,
            PseudoClass,
            PseudoElement,
        };
        Type type { Type::Invalid };

        struct ANPlusBPattern {
            int step_size { 0 }; 
            int offset = { 0 };  

            static ANPlusBPattern parse(StringView const& args);
            String to_string() const
            {
                return String::formatted("{}n{:+}", step_size, offset);
            }
        };

        struct PseudoClass {
            enum class Type {
                None,
                Link,
                Visited,
                Hover,
                Focus,
                FirstChild,
                LastChild,
                OnlyChild,
                Empty,
                Root,
                FirstOfType,
                LastOfType,
                NthChild,
                NthLastChild,
                Disabled,
                Enabled,
                Checked,
                Not,
                Active,
            };
            Type type { Type::None };

            ANPlusBPattern nth_child_pattern;

            SelectorList not_selector {};
        };
        PseudoClass pseudo_class {};

        enum class PseudoElement {
            None,
            Before,
            After,
            FirstLine,
            FirstLetter,
        };
        PseudoElement pseudo_element { PseudoElement::None };

        FlyString value {};

        struct Attribute {
            enum class MatchType {
                None,
                HasAttribute,
                ExactValueMatch,
                ContainsWord,      
                ContainsString,    
                StartsWithSegment, 
                StartsWithString,  
                EndsWithString,    
            };
            MatchType match_type { MatchType::None };
            FlyString name {};
            String value {};
        };
        Attribute attribute {};
    };

    enum class Combinator {
        None,
        ImmediateChild,    
        Descendant,        
        NextSibling,       
        SubsequentSibling, 
        Column,            
    };

    struct CompoundSelector {

        Combinator combinator { Combinator::None };
        Vector<SimpleSelector> simple_selectors;
    };

    static NonnullRefPtr<Selector> create(Vector<CompoundSelector>&& compound_selectors)
    {
        return adopt_ref(*new Selector(move(compound_selectors)));
    }

    ~Selector();

    Vector<CompoundSelector> const& compound_selectors() const { return m_compound_selectors; }

    u32 specificity() const;

private:
    explicit Selector(Vector<CompoundSelector>&&);

    Vector<CompoundSelector> m_compound_selectors;
};

}
