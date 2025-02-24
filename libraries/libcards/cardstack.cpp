/**
 * @file cardstack.cpp
 * @author Krisna Pranav
 * @brief card stack
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "cardstack.h"

namespace Cards
{
    /**
     * @brief Construct a new CardStack::CardStack object
     * 
     */
    CardStack::CardStack()
        : m_position ({ 0, 0 })
        , m_type(Type::Invalid)
        , m_base(m_position, { Card::width, Card::height })
    {}

    /**
     * @brief Construct a new CardStack::CardStack object
     * 
     * @param position 
     * @param type 
     */
    CardStack::CardStack(Gfx::IntPoint const& position, Type type)
    : m_position(position)
    , m_type(type)
    , m_rules(rules_for_type(type))
    , m_base(m_position, { Card::width, Card::height })
    {
        VERIFY(type != Type::Invalid);
        calculate_bounding_box();
    }
} // namespace Cards