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

    void CardStack::clear()
    {
        m_stack.clear();
        m_stack_positions.clear();
    }

    void CardStack::rebound_cards()
    {
        VERIFY(m_stack_positions.size() == m_stack.size());

        size_t card_index = 0;
        for (auto& card : m_stack)
            card.set_position(m_stack_positions.at(card_index++));
    }

} // namespace Cards