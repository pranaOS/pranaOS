/**
 * @file position.h
 * @author Krisna Pranav
 * @brief position
 * @version 6.0
 * @date 2023-09-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

namespace VT 
{
    class Position 
    {
    public:
        /// @brief Construct a new Position object
        Position() { }

        /**
         * @param row 
         * @param column 
         */
        Position(int row, int column)
            : m_row(row)
            , m_column(column)
        {
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return m_row >= 0 && m_column >= 0; 
        }

        /**
         * @return int 
         */
        int row() const 
        { 
            return m_row; 
        }

        /**
         * @return int 
         */
        int column() const 
        { 
            return m_column;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<(const Position& other) const
        {
            return m_row < other.m_row || (m_row == other.m_row && m_column < other.m_column);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(const Position& other) const
        {
            return *this < other || *this == other;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(const Position& other) const
        {
            return !(*this < other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const Position& other) const
        {
            return m_row == other.m_row && m_column == other.m_column;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const Position& other) const
        {
            return !(*this == other);
        }

    private:
        int m_row { -1 };
        int m_column { -1 };
    }; // class Position
} // namesapce VT