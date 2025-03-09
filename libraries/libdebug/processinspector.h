/**
 * @file processinspector.h
 * @author Krisna Pranav
 * @brief process inspector
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Debug
{
    
    class ProcessInspector
    {
    public:
        virtual ~ProcessInspector() = default;

        /**
         * @param address 
         * @param data 
         * @return true 
         * @return false 
         */
        virtual bool poke(FlatPtr address, FlatPtr data) = 0;
    protected:
        ProcessInspector() = default;
    }; // class ProcessInspector

} // namespace Debug
