/**
 * @file textposition.h
 * @author Krisna Pranav
 * @brief Text Position
 * @version 6.0
 * @date 2024-10-29
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/string.h>

namespace GUI
{
    class TextPosition
    {
    public:
        /**
         * @brief Construct a new Text Position object
         *
         */
        TextPosition() = default;

        /**
         * @brief Construct a new Text Position object
         *
         * @param line
         * @param column
         */
        TextPosition(size_t line, size_t column)
            : m_line(line), m_column(column)
        {
        }

        bool is_valid() const
        {
            return m_line != 0xffffffffu && m_column != 0xffffffffu;
        }

        size_t line() const
        {
            return m_line;
        }

        size_t column() const
        {
            return m_column;
        }

        /**
         * @brief Set the line object
         *
         * @param line
         */
        void set_line(size_t line)
        {
            m_line = line;
        }

        /**
         * @brief Set the column object
         *
         * @param column
         */
        void set_column(size_t column)
        {
            m_column = column;
        }

        /**
         * @param other
         * @return true
         * @return false
         */
        bool operator==(const TextPosition& other) const
        {
            return m_line == other.m_line && m_column == other.m_column;
        }

        /**
         * @param other
         * @return true
         * @return false
         */
        bool operator!=(const TextPosition& other) const
        {
            return m_line != other.m_line || m_column != other.m_column;
        }

        /**
         * @param other
         * @return true
         * @return false
         */
        bool operator<(const TextPosition& other) const
        {
            return m_line < other.m_line || (m_line == other.m_line && m_column < other.m_column);
        }

    private:
        size_t m_line{0xffffffff};
        size_t m_column{0xffffffff};
    }; // class TextPosition

} // namespace GUI

template <>
struct Mods::Formatter<GUI::TextPosition> : Mods::Formatter<FormatString>
{
    /**
     * @param builder
     * @param value
     * @return ErrorOr<void>
     */
    ErrorOr<void> format(FormatBuilder& builder, GUI::TextPosition const& value)
    {
        if(value.is_valid())
            return Formatter<FormatString>::format(builder, "({},{})", value.line(), value.column());

        return Formatter<FormatString>::format(builder, "TextPosition: Invalid");
    }
};
