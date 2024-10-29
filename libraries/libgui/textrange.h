/**
 * @file textrange.h
 * @author Krisna Pranav
 * @brief Text Range
 * @version 6.0
 * @date 2024-10-29
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <libgui/textposition.h>

namespace GUI
{
    class TextRange
    {
    public:
        /**
         * @brief Construct a new Text Range object
         *
         */
        TextRange()
        {
        }

        /**
         * @brief Construct a new Text Range object
         *
         * @param start
         * @param end
         */
        TextRange(const TextPosition& start, const TextPosition& end)
            : m_start(start), m_end(end)
        {
        }

        /**
         * @return true
         * @return false
         */
        bool is_valid() const
        {
            return m_start.is_valid() && m_end.is_valid() && m_start != m_end;
        }

        void clear()
        {
            m_start = {};
            m_end = {};
        }

        TextPosition& start()
        {
            return m_start;
        }

        TextPosition& end()
        {
            return m_end;
        }

        const TextPosition& start() const
        {
            return m_start;
        }

        const TextPosition& end() const
        {
            return m_end;
        }

        TextRange normalized() const
        {
            return TextRange(normalized_start(), normalized_end());
        }

        /**
         * @brief Set the start object
         *
         * @param position
         */
        void set_start(const TextPosition& position)
        {
            m_start = position;
        }

        /**
         * @brief Set the end object
         *
         * @param position
         */
        void set_end(const TextPosition& position)
        {
            m_end = position;
        }

        /**
         * @param start
         * @param end
         */
        void set(const TextPosition& start, const TextPosition& end)
        {
            m_start = start;
            m_end = end;
        }

        /**
         * @param other
         * @return true
         * @return false
         */
        bool operator==(const TextRange& other) const
        {
            return m_start == other.m_start && m_end == other.m_end;
        }

        /**
         * @param position
         * @return true
         * @return false
         */
        bool contains(const TextPosition& position) const
        {
            if(!(position.line() > m_start.line() || (position.line() == m_start.line() && position.column() >= m_start.column())))
                return false;
            if(!(position.line() < m_end.line() || (position.line() == m_end.line() && position.column() <= m_end.column())))
                return false;
            return true;
        }

    private:
        TextPosition normalized_start() const
        {
            return m_start < m_end ? m_start : m_end;
        }

        TextPosition normalized_end() const
        {
            return m_start < m_end ? m_end : m_start;
        }

        TextPosition m_start;
        TextPosition m_end;
    }; // class TextRange
} // namespace GUI

template <>
struct Mods::Formatter<GUI::TextRange> : Mods::Formatter<FormatString>
{
    /**
     * @param builder
     * @param value
     * @return ErrorOr<void>
     */
    ErrorOr<void> format(FormatBuilder& builder, GUI::TextRange const& value)
    {
        if(value.is_valid())
            return Formatter<FormatString>::format(builder, "{}-{}", value.start(), value.end());
        return Formatter<FormatString>::format(builder, "TextRange: Invalid");
    }
};
