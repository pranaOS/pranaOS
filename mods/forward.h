/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief Forward
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Mods
{

    namespace Detail 
    {
        /**
         * @tparam inline_capacity 
         */
        template<size_t inline_capacity>
        class ByteBuffer;
    } // namespace Detail
 
    class Bitmap;
    
    using ByteBuffer = Mods::Detail::ByteBuffer<32>;

    class Error;
    class GenericLexer;
    class IPv4Address;
    class JsonArray;
    class JsonObject;
    class JsonValue;
    class StackInfo;
    class String;
    class StringBuilder;
    class StringImpl;
    class StringView;
    class Time;
    class URL;
    class FlyString;
    class Utf16View;
    class Utf32View;
    class Utf8CodePointIterator;
    class Utf8View;
    class InputStream;
    class InputMemoryStream;
    class DuplexMemoryStream;
    class OutputStream;
    class InputBitStream;
    class OutputBitStream;
    class OutputMemoryStream;

    /**
     * @tparam Capacity 
     */
    template<size_t Capacity>
    class CircularDuplexStream;

    /**
     * @tparam T 
     */
    template<typename T>
    class Span;

    /**
     * @tparam T 
     * @tparam Size 
     */
    template<typename T, size_t Size>
    struct Array;

    /**
     * @tparam Container 
     * @tparam ValueType 
     */
    template<typename Container, typename ValueType>
    class SimpleIterator;

    using ReadonlyBytes = Span<const u8>;
    using Bytes = Span<u8>;

    /**
     * @tparam T 
     * @tparam DefaultMemoryOrder 
     */
    template<typename T, Mods::MemoryOrder DefaultMemoryOrder>
    class Atomic;

    /**
     * @tparam T 
     */
    template<typename T>
    class SinglyLinkedList;

    /**
     * @tparam T 
     */
    template<typename T>
    class DoublyLinkedList;

    /**
     * @tparam T 
     * @tparam capacity 
     */
    template<typename T, size_t capacity>
    class CircularQueue;

    /**
     * @tparam T 
     */
    template<typename T>
    struct Traits;

    /**
     * @tparam T 
     * @tparam TraitsForT 
     * @tparam IsOrdered 
     */
    template<typename T, typename TraitsForT = Traits<T>, bool IsOrdered = false>
    class HashTable;

    /**
     * @tparam T 
     * @tparam TraitsForT 
     */
    template<typename T, typename TraitsForT = Traits<T>>
    using OrderedHashTable = HashTable<T, TraitsForT, true>;

    /**
     * @tparam K 
     * @tparam V 
     * @tparam KeyTraits 
     * @tparam IsOrdered 
     */
    template<typename K, typename V, typename KeyTraits = Traits<K>, bool IsOrdered = false>
    class HashMap;

    /**
     * @tparam K 
     * @tparam V 
     * @tparam KeyTraits 
     */
    template<typename K, typename V, typename KeyTraits = Traits<K>>
    using OrderedHashMap = HashMap<K, V, KeyTraits, true>;

    /**
     * @tparam T 
     */
    template<typename T>
    class Badge;

    /**
     * @tparam T 
     */
    template<typename T>
    class FixedArray;
    
    /**
     * @tparam precision 
     * @tparam Underlying 
     */
    template<size_t precision, typename Underlying = i32>
    class FixedPoint;

    /**
     * @tparam ypename 
     */
    template<typename>
    class Function;

    /**
     * @tparam Out 
     * @tparam In 
     */
    template<typename Out, typename... In>
    class Function<Out(In...)>;

    /**
     * @tparam T 
     */
    template<typename T>
    class NonnullRefPtr;

    /**
     * @tparam T 
     */
    template<typename T>
    class NonnullOwnPtr;

    /**
     * @tparam T 
     * @tparam inline_capacity 
     */
    template<typename T, size_t inline_capacity = 0>
    class NonnullRefPtrVector;

    /**
     * @tparam T 
     * @tparam inline_capacity 
     */
    template<typename T, size_t inline_capacity = 0>
    class NonnullOwnPtrVector;

    /**
     * @tparam T 
     */
    template<typename T>
    class Optional;

    /**
     * @tparam T 
     */
    template<typename T>
    struct RefPtrTraits;

    /**
     * @tparam T 
     * @tparam PtrTraits 
     */
    template<typename T, typename PtrTraits = RefPtrTraits<T>>
    class RefPtr;

    /**
     * @tparam T 
     */
    template<typename T>
    class OwnPtr;

    /**
     * @tparam T 
     */
    template<typename T>
    class WeakPtr;

    template<typename T, size_t inline_capacity = 0>
    requires(!IsRvalueReference<T>) class Vector;

    /**
     * @tparam T 
     * @tparam ErrorType 
     */
    template<typename T, typename ErrorType = Error>
    class [[nodiscard]] ErrorOr;
}

using Mods::Array;
using Mods::Atomic;
using Mods::Badge;
using Mods::Bitmap;
using Mods::ByteBuffer;
using Mods::Bytes;
using Mods::CircularDuplexStream;
using Mods::CircularQueue;
using Mods::DoublyLinkedList;
using Mods::DuplexMemoryStream;
using Mods::Error;
using Mods::ErrorOr;
using Mods::FixedArray;
using Mods::FixedPoint;
using Mods::FlyString;
using Mods::Function;
using Mods::GenericLexer;
using Mods::HashMap;
using Mods::HashTable;
using Mods::InputBitStream;
using Mods::InputMemoryStream;
using Mods::InputStream;
using Mods::IPv4Address;
using Mods::JsonArray;
using Mods::JsonObject;
using Mods::JsonValue;
using Mods::NonnullOwnPtr;
using Mods::NonnullOwnPtrVector;
using Mods::NonnullRefPtr;
using Mods::NonnullRefPtrVector;
using Mods::Optional;
using Mods::OutputBitStream;
using Mods::OutputMemoryStream;
using Mods::OutputStream;
using Mods::OwnPtr;
using Mods::ReadonlyBytes;
using Mods::RefPtr;
using Mods::SinglyLinkedList;
using Mods::Span;
using Mods::StackInfo;
using Mods::String;
using Mods::StringBuilder;
using Mods::StringImpl;
using Mods::StringView;
using Mods::Time;
using Mods::Traits;
using Mods::URL;
using Mods::Utf16View;
using Mods::Utf32View;
using Mods::Utf8CodePointIterator;
using Mods::Utf8View;
using Mods::Vector;