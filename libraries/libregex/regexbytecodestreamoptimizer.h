/**
 * @file regexbytecodestreamoptimizer.h
 * @author Krisna Pranav
 * @brief Regex Byte Code Stream Optimizer
 * @version 6.0
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/vector.h>
#include "forward.h"

namespace regex 
{
    class Optimizer 
    {
    public:
        /**
         * @param target 
         * @param left 
         * @param right 
         */
        static void append_alternation(ByteCode& target, ByteCode&& left, ByteCode&& right);

        /**
         * @param target 
         * @param alternatives 
         */
        static void append_alternation(ByteCode& target, Span<ByteCode> alternatives);

        /**
         * @param target 
         * @param pairs 
         */
        static void append_character_class(ByteCode& target, Vector<CompareTypeAndValuePair>&& pairs);
    }; // class Optimizer
} // namespace regex