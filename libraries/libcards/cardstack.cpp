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

    /**
     * @param card 
     * @param stack_size 
     * @param movement_rule 
     * @return true 
     * @return false 
     */
    bool CardStack::is_allowed_to_push(Card const& card, size_t stack_size, MovementRule movement_rule) const
    {
        if (m_type == Type::Stock || m_type == Type::Waste || m_type == Type::Play)
            return false;

        if (m_type == Type::Normal && is_empty()) {
            if (movement_rule == MovementRule::Alternating) {
                return card.value() == 12;
            }
            return true;
        }

        if (m_type == Type::Foundation && is_empty())
            return card.value() == 0;

        if (!is_empty()) {
            auto& top_card = peek();
            if (top_card.is_upside_down())
                return false;

            if (m_type == Type::Foundation) {
                if (stack_size > 1)
                    return false;
                return top_card.suit() == card.suit() && m_stack.size() == card.value();
            } else if (m_type == Type::Normal) {
                bool color_match;
                switch (movement_rule) {
                case MovementRule::Alternating:
                    color_match = card.color() != top_card.color();
                    break;
                case MovementRule::Same:
                    color_match = card.color() == top_card.color();
                    break;
                case MovementRule::Any:
                    color_match = true;
                    break;
                }

                return color_match && top_card.value() == card.value() + 1;
            }

            VERIFY_NOT_REACHED();
        }

        return true;
    }

    /**
     * @param card 
     */
    void CardStack::push(NonnullRefPtr<Card> card)
    {
        auto size = m_stack.size();
        auto top_most_position = m_stack_positions.is_empty() ? m_position : m_stack_positions.last();

        if (size && size % m_rules.step == 0) {
            if (peek().is_upside_down())
                top_most_position.translate_by(m_rules.shift_x, m_rules.shift_y_upside_down);
            else
                top_most_position.translate_by(m_rules.shift_x, m_rules.shift_y);
        }

        if (m_type == Type::Stock)
            card->set_upside_down(true);

        card->set_position(top_most_position);

        m_stack.append(card);
        m_stack_positions.append(top_most_position);
        calculate_bounding_box();
    }

} // namespace Cards