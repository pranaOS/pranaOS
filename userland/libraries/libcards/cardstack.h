/**
 * @file cardstack.h
 * @author Krisna Pranav
 * @brief card stack
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "card.h"
#include <mods/format.h>
#include <mods/refcounted.h>
#include <mods/vector.h>

namespace Cards 
{
    class CardStack final : public RefCounted<CardStack> 
    {
    public:
        enum class Type 
        {
            Invalid,
            Stock,
            Normal,
            Waste,
            Play,
            Foundation
        }; // enum class Type 

        enum class MovementRule 
        {
            Alternating,
            Same,
            Any,
        }; // enum class MovementRule 

        /**
         * @brief Construct a new CardStack object
         * 
         */
        CardStack();

        /**
         * @brief Construct a new CardStack object
         * 
         * @param position 
         * @param type 
         */
        CardStack(Gfx::IntPoint const& position, Type type);

        /**
         * @brief Construct a new CardStack object
         * 
         * @param position 
         * @param type 
         * @param associated_stack 
         */
        CardStack(Gfx::IntPoint const& position, Type type, NonnullRefPtr<CardStack> associated_stack);

        bool is_empty() const 
        { 
            return m_stack.is_empty(); 
        }
        
        bool is_focused() const 
        { 
            return m_focused; 
        }

        /**
         * @return Type 
         */
        Type type() const 
        { 
            return m_type; 
        }
        
        /**
         * @return NonnullRefPtrVector<Card> const& 
         */
        NonnullRefPtrVector<Card> const& stack() const 
        { 
            return m_stack; 
        }

        /**
         * @return size_t 
         */
        size_t count() const 
        { 
            return m_stack.size(); 
        }

        /**
         * @return Card const& 
         */
        Card const& peek() const 
        { 
            return m_stack.last(); 
        }

        /**
         * @return Card& 
         */
        Card& peek() 
        { 
            return m_stack.last(); 
        }

        /**
         * @return Gfx::IntRect const& 
         */
        Gfx::IntRect const& bounding_box() const 
        { 
            return m_bounding_box; 
        }

        /**
         * @brief Set the focused object
         * 
         * @param focused 
         */
        void set_focused(bool focused) 
        { 
            m_focused = focused; 
        }

        /**
         * @param card 
         */
        void push(NonnullRefPtr<Card> card);
        NonnullRefPtr<Card> pop();
        void move_to_stack(CardStack&);
        void rebound_cards();

        /**
         * @param stack_size 
         * @param movement_rule 
         * @return true 
         * @return false 
         */
        bool is_allowed_to_push(Card const&, size_t stack_size = 1, MovementRule movement_rule = MovementRule::Alternating) const;

        /**
         * @param click_location 
         * @param grabbed 
         * @param movement_rule 
         */
        void add_all_grabbed_cards(Gfx::IntPoint const& click_location, NonnullRefPtrVector<Card>& grabbed, MovementRule movement_rule = MovementRule::Alternating);

        /**
         * @param background_color 
         */
        void draw(GUI::Painter&, Gfx::Color const& background_color);

        void clear();

    private:
        struct StackRules {
            uint8_t shift_x { 0 };
            uint8_t shift_y { 0 };
            uint8_t step { 1 };
            uint8_t shift_y_upside_down { 0 };
        }; // struct StackRules 

        /**
         * @param stack_type 
         * @return constexpr StackRules 
         */
        constexpr StackRules rules_for_type(Type stack_type)
        {
            switch (stack_type) {
            case Type::Foundation:
                return { 2, 1, 4, 1 };
            case Type::Normal:
                return { 0, 20, 1, 3 };
            case Type::Stock:
                return { 2, 1, 8, 1 };
            case Type::Waste:
                return { 0, 0, 1, 0 };
            case Type::Play:
                return { 20, 0, 1, 0 };
            default:
                return {};
            }
        }

        void calculate_bounding_box();

        RefPtr<CardStack> m_associated_stack;
        NonnullRefPtrVector<Card> m_stack;
        Vector<Gfx::IntPoint> m_stack_positions;
        Gfx::IntPoint m_position;
        Gfx::IntRect m_bounding_box;
        Type m_type { Type::Invalid };
        StackRules m_rules;
        bool m_focused { false };
        Gfx::IntRect m_base;
    }; // class CardStack final : public RefCounted<CardStack>

} // namespace Cards

/**
 * @tparam  
 */
template<>
struct Mods::Formatter<Cards::CardStack> : Formatter<FormatString> {
    /**
     * @param builder 
     * @param stack 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, Cards::CardStack const& stack)
    {
        StringView type;

        switch (stack.type()) {
        case Cards::CardStack::Type::Stock:
            type = "Stock"sv;
            break;
        case Cards::CardStack::Type::Normal:
            type = "Normal"sv;
            break;
        case Cards::CardStack::Type::Foundation:
            type = "Foundation"sv;
            break;
        case Cards::CardStack::Type::Waste:
            type = "Waste"sv;
            break;
        case Cards::CardStack::Type::Play:
            type = "Play"sv;
            break;
        default:
            VERIFY_NOT_REACHED();
        }

        StringBuilder cards;
        bool first_card = true;

        for (auto const& card : stack.stack()) {
            cards.appendff("{}{}", (first_card ? "" : " "), card);
            first_card = false;
        }

        return Formatter<FormatString>::format(builder, "{:<10} {:>16}: {}", type, stack.bounding_box(), cards.build());
    }
};
