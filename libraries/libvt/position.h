/**
 * @file position.h
 * @author Krisna Pranav
 * @brief position
 * @version 6.0
 * @date 2023-09-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

namespace VT
{
    class Position
    {
    public:
        Position() {}
        
        Position(int row, int column)
            : m_row(row)
            , m_column(column)
        {}
    
    private:
        int m_row { -1 };
        int m_column { -1 };

    }; // class Position
}