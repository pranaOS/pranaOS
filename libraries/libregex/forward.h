/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief Forward
 * @version 6.0
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <mods/types.h>

namespace regex 
{
    struct CompareTypeAndValuePair;

    enum class Error : u8;
    class Lexer;
    class PosixExtendedParser;
    class ECMA262Parser;

    class ByteCode;
    class OpCode;
    class OpCode_Exit;
    class OpCode_Jump;
    class OpCode_ForkJump;
    class OpCode_ForkStay;
    class OpCode_CheckBegin;
    class OpCode_CheckEnd;
    class OpCode_SaveLeftCaptureGroup;
    class OpCode_SaveRightCaptureGroup;
    class OpCode_SaveLeftNamedCaptureGroup;
    class OpCode_SaveNamedLeftCaptureGroup;
    class OpCode_SaveRightNamedCaptureGroup;
    class OpCode_Compare;
    class RegexStringView;
} // namespace regex

using regex::ECMA262Parser;
using regex::Lexer;
using regex::PosixExtendedParser;
using regex::RegexStringView;
