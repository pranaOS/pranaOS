/**
 * @file stackinfo.h
 * @author Krisna Pranav
 * @brief StackInfo
 * @version 6.0
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "types.h"


namespace Mods 
{

    class StackInfo 
    {
    public:
        StackInfo();

        /**
         * @return FlatPtr 
         */
        FlatPtr base() const 
        { 
            return m_base; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr top() const 
        { 
            return m_top; 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_size; 
        }

        /**
         * @return size_t 
         */
        size_t size_free() const
        {
            FlatPtr dummy;
            return reinterpret_cast<FlatPtr>(&dummy) - m_base;
        }

    private:
        FlatPtr m_base;
        FlatPtr m_top;
        size_t m_size;
    }; // class StackInfo

} // namespace Mods

using Mods::StackInfo;