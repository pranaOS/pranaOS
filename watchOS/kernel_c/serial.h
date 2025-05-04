/**
 * @file serial.h
 * @author Krisna Pranav
 * @brief serial
 * @version 6.0
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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

        /**
         * @tparam T 
         * @param smth 
         */
        template<typename T>
        void print(T smth)
        {
            if (this->bauds < 1)
                return;
            
            if (this->buffered) {
                if (this->addBufferedNewline) {
                    this->buffer.push_back({});
                    this->addBufferNewline = false;
                }
                this->buffer.back() << smth;
            } else {
                std::cout << smth << std::flush;
            }
        }

        /**
         * @tparam T 
         * @param smth 
         */
        template<typename T>
        void println(T smth) 
        {
            if (this->bauds < 1)
                return;

            this->print(smth);
            this->println();
        }

    private:
        std::list<char> inputBuffer;
        int bauds = 0;
        bool buffered = false;
        bool addBufferNewline = true;
    }; // class serial_t

    extern serial_t serial;
} // namespace Kernel

using Kernel::serial;