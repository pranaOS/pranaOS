/**
 * @file card.h
 * @author Krisna Pranav
 * @brief card
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Cards 
{
    class Card final : public Core::Object 
    {
        C_OBJECT(Card)
    private:
        Card(Suit suit, uint8_t value);

        Suit m_suit;
        uint8_t m_value;
        bool m_old_position_valid { false };
        bool m_moving { false };
        bool m_upside_down { false };
        bool m_inverted { false };
    }
} // namespace Cards

template<>
struct Mods::Formatter<Cards::Card> : Formatter<FormatString> {
    ErrorOr<void> format(FormatBuilder& builder, Cards::Card const& card)
    {
        StringView suit;

        switch (card.suit()) {
        case Cards::Card::Suit::Clubs:
            suit = "C"sv;
            break;
        case Cards::Card::Suit::Diamonds:
            suit = "D"sv;
            break;
        case Cards::Card::Suit::Hearts:
            suit = "H"sv;
            break;
        case Cards::Card::Suit::Spades:
            suit = "S"sv;
            break;
        default:
            VERIFY_NOT_REACHED();
        }

        return Formatter<FormatString>::format(builder, "{:>2}{}", Cards::Card::labels[card.value()], suit);
    }
};
