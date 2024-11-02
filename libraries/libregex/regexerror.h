/**
 * @file regexerror.h
 * @author Krisna Pranav
 * @brief Regex Error
 * @version 6.0
 * @date 2024-11-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>
#include <mods/types.h>
#ifdef __pranaos__
#    include <regex.h>
#else
#    include <libc/regex.h>
#endif

namespace regex 
{
    enum class Error : u8 
    {
        NoError = __Regex_NoError,
        InvalidPattern = __Regex_InvalidPattern,                         
        InvalidCollationElement = __Regex_InvalidCollationElement,       
        InvalidCharacterClass = __Regex_InvalidCharacterClass,           
        InvalidTrailingEscape = __Regex_InvalidTrailingEscape,           
        InvalidNumber = __Regex_InvalidNumber,                           
        MismatchingBracket = __Regex_MismatchingBracket,                 
        MismatchingParen = __Regex_MismatchingParen,                     
        MismatchingBrace = __Regex_MismatchingBrace,                     
        InvalidBraceContent = __Regex_InvalidBraceContent,               
        InvalidBracketContent = __Regex_InvalidBracketContent,           
        InvalidRange = __Regex_InvalidRange,                             
        InvalidRepetitionMarker = __Regex_InvalidRepetitionMarker,       
        ReachedMaxRecursion = __Regex_ReachedMaxRecursion,               
        EmptySubExpression = __Regex_EmptySubExpression,                 
        InvalidCaptureGroup = __Regex_InvalidCaptureGroup,               
        InvalidNameForCaptureGroup = __Regex_InvalidNameForCaptureGroup, 
        InvalidNameForProperty = __Regex_InvalidNameForProperty,         
        DuplicateNamedCapture = __Regex_DuplicateNamedCapture,           
    }; // enum class Error : u8

    /**
     * @brief Get the error string object
     * 
     * @param error 
     * @return String 
     */
    inline String get_error_string(Error error)
    {
        switch (error) {
        case Error::NoError:
            return "No error";
        case Error::InvalidPattern:
            return "Invalid regular expression.";
        case Error::InvalidCollationElement:
            return "Invalid collating element referenced.";
        case Error::InvalidCharacterClass:
            return "Invalid character class type referenced.";
        case Error::InvalidTrailingEscape:
            return "Trailing \\ in pattern.";
        case Error::InvalidNumber:
            return "Number in \\digit invalid or in error.";
        case Error::MismatchingBracket:
            return "[ ] imbalance.";
        case Error::MismatchingParen:
            return "( ) imbalance.";
        case Error::MismatchingBrace:
            return "{ } imbalance.";
        case Error::InvalidBraceContent:
            return "Content of {} invalid: not a number, number too large, more than two numbers, first larger than second.";
        case Error::InvalidBracketContent:
            return "Content of [] invalid.";
        case Error::InvalidRange:
            return "Invalid endpoint in range expression.";
        case Error::InvalidRepetitionMarker:
            return "?, * or + not preceded by valid regular expression.";
        case Error::ReachedMaxRecursion:
            return "Maximum recursion has been reached.";
        case Error::EmptySubExpression:
            return "Sub expression has empty content.";
        case Error::InvalidCaptureGroup:
            return "Content of capture group is invalid.";
        case Error::InvalidNameForCaptureGroup:
            return "Name of capture group is invalid.";
        case Error::InvalidNameForProperty:
            return "Name of property is invalid.";
        case Error::DuplicateNamedCapture:
            return "Duplicate capture group name";
        }
        return "Undefined error.";
    }
}

using regex::get_error_string;
