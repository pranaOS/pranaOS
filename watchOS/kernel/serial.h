/**
 * @file serial.h
 * @author Krisna Pranav
 * @brief serial
 * @version 6.0
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include "defines.h"

namespace Kernel
{
    class serial_t
    {   
    public:
        serial_t();
        ~serial_t() {};

        std::list<std::stringstream> buffer;

        /**
         * @brief Set the Buffered object
         * 
         * @param buffered 
         */
        void setBuffered(bool buffered);

        bool isBuffered() const;

        /**
         * @param b 
         */
        void begin(int b);
    }; // class serial_t
} // namespace Kernel