/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libsql/ast/Lexer.h>
#include <libsql/ast/Parser.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto parser = SQL::AST::Parser(SQL::AST::Lexer({ data, size }));
    [[maybe_unused]] auto statement = parser.next_statement();
    return 0;
}
