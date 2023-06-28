/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief Forward
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Mods {
    /**
     * @brief Class()
     * 
     */
    class Bitmap;
    class ByteBuffer;
    class DebugLogStream;
    class IPv4Address;
    class JsonArray;
    class JsonObject;
    class JsonValue;
    class LogStream;
    class SharedBuffer;
    class StackInfo;
    class String;
    class StringBuilder;
    class StringImpl;
    class StringView;
    class URL;
    class FlyString;
    class Utf32View;
    class Utf8View;
    class InputStream;
    class InputMemoryStream;
    class DuplexMemoryStream;
    class OutputStream;
    class InputBitStream;
    class OutputMemoryStream;

    /**
     * @brief CircularDuplexStream
     * 
     * @tparam Capacity 
     */
    template<size_t Capacity>
    class CircularDuplexStream;

    /**
     * @brief Span
     * 
     * @tparam T 
     */
    template<typename T>
    class Span;

    /**
     * @brief Array
     * 
     * @tparam T 
     * @tparam Size 
     */
    template<typename T, size_t Size>
    struct Array;

    /**
     * @brief SimpleIterator
     * 
     * @tparam Container 
     * @tparam ValueType 
     */
    template<typename Container, typename ValueType>
    class SimpleIterator;

    /**
     * @brief ReadonlyBytes + Bytes
     * 
     */
    using ReadonlyBytes = Span<const u8>;
    using Bytes = Span<u8>;

    /**
     * @brief Atomic
     * 
     * @tparam T 
     */
    template<typename T>
    class Atomic;

    /**
     * @brief SinglyLinkedList 
     * 
     * @tparam T 
     */
    template<typename T>
    class SinglyLinkedList;

    /**
     * @brief DoublyLinkedList
     * 
     * @tparam T 
     */
    template<typename T>
    class DoublyLinkedList;

    /**
     * @brief InlineLinkedList
     * 
     * @tparam T 
     */
    template<typename T>
    class InlineLinkedList;

    /**
     * @brief CircularQueue
     * 
     * @tparam T 
     * @tparam capacity 
     */
    template<typename T, size_t capacity>
    class CircularQueue;

    /**
     * @brief Traits
     * 
     * @tparam T 
     */
    template<typename T>
    struct Traits;

    /**
     * @brief HashTable
     * 
     * @tparam T 
     * @tparam typename 
     */
    template<typename T, typename = Traits<T>>
    class HashTable;

    /**
     * @brief Hashmap
     * 
     * @tparam K 
     * @tparam V 
     * @tparam typename 
     */
    template<typename K, typename V, typename = Traits<K>>
    class HashMap;

    /**
     * @brief Badge
     * 
     * @tparam T 
     */
    template<typename T>
    class Badge;

    /**
     * @brief Function
     * 
     * @tparam ypename 
     */
    template<typename>
    class Function;

    /**
     * @brief Function
     * 
     * @tparam Out 
     * @tparam In 
     */
    template<typename Out, typename... In>
    class Function<Out(In...)>;

    /**
     * @brief NonnullRefPtr
     * 
     * @tparam T 
     */
    template<typename T>
    class NonnullRefPtr;

    /**
     * @brief NonnullOwnPtr
     * 
     * @tparam T 
     */
    template<typename T>
    class NonnullOwnPtr;

    /**
     * @brief NonnullOwnPtr
     * 
     * @tparam T 
     */
    template<typename T>
    class NonnullOwnPtr;

    /**
     * @brief NonnullRefPtrVector
     * 
     * @tparam T 
     * @tparam inline_capacity 
     */
    template<typename T, int inline_capacity = 0>
    class NonnullRefPtrVector;

    /**
     * @brief NonullOwnPtrVector
     * 
     * @tparam T 
     * @tparam inline_capacity 
     */
    template<typename T, int inline_capacity = 0>
    class NonnullOwnPtrVector;

    /**
     * @brief optional
     * 
     * @tparam T 
     */
    template<typename T>
    class Optional;

    /**
     * @brief  refptrtraits
     * 
     * @tparam T 
     */
    template<typename T>
    struct RefPtrTraits;

    /**
     * @brief refptr
     * 
     * @tparam T 
     * @tparam PtrTraits 
     */
    template<typename T, typename PtrTraits = RefPtrTraits<T>>
    class RefPtr;

    /**
     * @brief ownptr
     * 
     * @tparam T 
     */
    template<typename T>
    class OwnPtr;

    /**
     * @brief weakptr
     * 
     * @tparam T 
     */
    template<typename T>
    class WeakPtr;

    /**
     * @brief vector
     * 
     * @tparam T 
     * @tparam inline_capacity 
     */
    template<typename T, size_t inline_capacity = 0>
    class Vector;

    /**
     * @brief dbgln
     * 
     * @tparam Parameters 
     * @param fmtstr 
     */
    template<typename... Parameters>
    void dbgln(const char* fmtstr, const Parameters&...);

    /**
     * @brief warnln
     * 
     * @tparam Parameters 
     * @param fmtstr 
     */
    template<typename... Parameters>
    void warnln(const char* fmtstr, const Parameters&...);

    /**
     * @brief outln
     * 
     * @tparam Parameters 
     * @param fmtstr 
     */
    template<typename... Parameters>
    void outln(const char* fmtstr, const Parameters&...);

}

/**
 * @brief Accessing mods.
 * 
 */
using Mods::Array;
using Mods::Atomic;
using Mods::Badge;
using Mods::Bitmap;
using Mods::ByteBuffer;
using Mods::Bytes;
using Mods::CircularDuplexStream;
using Mods::CircularQueue;
using Mods::DebugLogStream;
using Mods::DoublyLinkedList;
using Mods::DuplexMemoryStream;
using Mods::FlyString;
using Mods::Function;
using Mods::HashMap;
using Mods::HashTable;
using Mods::InlineLinkedList;
using Mods::InputBitStream;
using Mods::InputMemoryStream;
using Mods::InputStream;
using Mods::IPv4Address;
using Mods::JsonArray;
using Mods::JsonObject;
using Mods::JsonValue;
using Mods::LogStream;
using Mods::NonnullOwnPtr;
using Mods::NonnullOwnPtrVector;
using Mods::NonnullRefPtr;
using Mods::NonnullRefPtrVector;
using Mods::Optional;
using Mods::OutputMemoryStream;
using Mods::OutputStream;
using Mods::OwnPtr;
using Mods::ReadonlyBytes;
using Mods::RefPtr;
using Mods::SharedBuffer;
using Mods::SinglyLinkedList;
using Mods::Span;
using Mods::StackInfo;
using Mods::String;
using Mods::StringBuilder;
using Mods::StringImpl;
using Mods::StringView;
using Mods::Traits;
using Mods::URL;
using Mods::Utf32View;
using Mods::Utf8View;
using Mods::Vector;
