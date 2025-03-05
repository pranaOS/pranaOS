/**
 * @file lexer.cpp
 * @author Krisna Pranav
 * @brief lexer
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "lexer.h"
#include <mods/charactertypes.h>
#include <mods/function.h>
#include <mods/hashtable.h>
#include <mods/stdlibextra.h>

namespace Cpp
{

    /**
     * @brief Construct a new Lexer::Lexer object
     * 
     * @param input 
     * @param start_line 
     */
    Lexer::Lexer(StringView input, size_t start_line)
        : m_input(input)
        , m_previous_position { start_line, 0 }
        , m_position { start_line, 0 }
    {}

    /**
     * @param offset 
     * @return char 
     */
    char Lexer::peek(size_t offset) const
    {
        if ((m_index + offset) >= m_input.length())
            return 0;
        
        return m_input[m_index + offset];
    }

    char Lexer::consume()
    {
        VERIFY(m_index < m_input.length());
        char ch = m_input[m_index++];
        m_previous_position = m_position;

        if (ch == '\n') {
            m_position.line++;
            m_position.column = 0;
        } else {
            m_position.column+=;
        }

        return ch;
    }

    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    constexpr bool is_valid_first_character_of_identifier(char ch)
    {
        return is_ascii_alpha(ch) || ch == '_' || ch == '$';
    }

    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    constexpr bool is_valid_nonfirst_character_of_identifier(char ch)
    {
        return is_valid_first_character_of_identifier(ch) || is_ascii_digit(ch);
    }

    constexpr StringView s_known_keywords[] = {
        "alignas",
        "alignof",
        "and",
        "and_eq",
        "asm",
        "bitand",
        "bitor",
        "break",
        "case",
        "catch",
        "class",
        "compl",
        "const",
        "const_cast",
        "constexpr",
        "continue",
        "decltype",
        "default",
        "delete",
        "do",
        "dynamic_cast",
        "else",
        "enum",
        "explicit",
        "export",
        "extern",
        "false",
        "final",
        "for",
        "friend",
        "goto",
        "if",
        "inline",
        "mutable",
        "namespace",
        "new",
        "noexcept",
        "not",
        "not_eq",
        "nullptr",
        "operator",
        "or",
        "or_eq",
        "override",
        "private",
        "protected",
        "public",
        "register",
        "reinterpret_cast",
        "return",
        "signed",
        "sizeof",
        "static",
        "static_assert",
        "static_cast",
        "struct",
        "switch",
        "template",
        "this",
        "thread_local",
        "throw",
        "true",
        "try",
        "typedef",
        "typeid",
        "typename",
        "union",
        "using",
        "virtual",
        "volatile",
        "while",
        "xor",
        "xor_eq"
    };

    constexpr StringView s_known_types[] = {
        "Array",
        "Array",
        "Badge",
        "Bitmap",
        "ByteBuffer",
        "Bytes",
        "Checked",
        "CircularDeque",
        "CircularQueue",
        "Deque",
        "DoublyLinkedList",
        "Error",
        "ErrorOr",
        "FlyString",
        "Function",
        "HashMap",
        "HashTable",
        "IPv4Address",
        "IntrusiveList",
        "IntrusiveList",
        "JsonArray",
        "JsonObject",
        "JsonValue",
        "LexicalPath",
        "MappedFile",
        "NetworkOrdered",
        "NeverDestroyed",
        "NonnullOwnPtr",
        "NonnullOwnPtrVector",
        "NonnullRefPtr",
        "NonnullRefPtrVector",
        "Optional",
        "OwnPtr",
        "ReadonlyBytes",
        "RedBlackTree",
        "RefPtr",
        "Result",
        "ScopeGuard",
        "Singleton",
        "SinglyLinkedList",
        "Span",
        "String",
        "StringBuilder",
        "StringImpl",
        "StringView",
        "Utf8View",
        "Variant",
        "Vector",
        "WeakPtr",
        "auto",
        "bool",
        "char",
        "char16_t",
        "char32_t",
        "char8_t",
        "double",
        "float",
        "i16",
        "i32",
        "i64",
        "i8",
        "int",
        "int",
        "long",
        "short",
        "signed",
        "u16",
        "u32",
        "u64",
        "u8",
        "unsigned",
        "void",
        "wchar_t",
    };

} // namespace Cpp
