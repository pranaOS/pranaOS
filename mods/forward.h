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

#include <mods/defaultdelete.h>
#include <mods/singlylinkedlistsizepolicy.h>
#include <mods/stdlibextras.h>
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

    enum class TrailingCodePointTransformation : u8;

    class BigEndianInputBitStream;
    class BigEndianOutputBitStream;
    class Bitmap;
    using ByteBuffer = Detail::ByteBuffer<32>;
    class CircularBuffer;
    class ConstrainedStream;
    class CountingStream;
    class DeprecatedFlyString;
    class ByteString;
    class DeprecatedStringCodePointIterator;
    class Duration;
    class Error;
    class FlyString;
    class GenericLexer;
    class IPv4Address;
    class JsonArray;
    class JsonObject;
    class JsonValue;
    class LexicalPath;
    class LittleEndianInputBitStream;
    class LittleEndianOutputBitStream;
    class SearchableCircularBuffer;
    class SeekableStream;
    class StackInfo;
    class Stream;
    class String;
    class StringBuilder;
    class StringImpl;
    class StringView;
    class UnixDateTime;
    class Utf16View;
    class Utf32CodePointIterator;
    class Utf32View;
    class Utf8CodePointIterator;
    class Utf8View;

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

    /**
     * @tparam T 
     */
    template<typename T>
    using ReadonlySpan = Span<T const>;

    using ReadonlyBytes = ReadonlySpan<u8>;
    using Bytes = Span<u8>;

    /**
     * @tparam T
     * @tparam DefaultMemoryOrder
     */
    template<typename T, Mods::MemoryOrder DefaultMemoryOrder>
    class Atomic;

    /**
     * @tparam T
     * @tparam TSizeCalcuationPolicy
     */
    template<typename T, typename TSizeCalculationPolicy = DefaultSizeCalculationPolicy>
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
     * @tparam IsOrdered
     */
    template<typename T, typename TraitsForT = Traits<T>, bool IsOrdered = false>
    class HashTable;

    /**
     * @tparam T
     */
    template<typename T, typename TraitsForT = Traits<T>>
    using OrderedHashTable = HashTable<T, TraitsForT, true>;

    /**
     * @tparam K
     * @tparam V
     * @tparam KeyTraits
     * @tparam ValueTraits
     */
    template<typename K, typename V, typename KeyTraits = Traits<K>, typename ValueTraits = Traits<V>, bool IsOrdered = false>
    class HashMap;

    /**
     * @tparam K
     * @tparam V
     * @tparam KeyTraits
     */
    template<typename K, typename V, typename KeyTraits = Traits<K>, typename ValueTraits = Traits<V>>
    using OrderedHashMap = HashMap<K, V, KeyTraits, ValueTraits, true>;

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
     * @tparam
     */
    template<typename>
    class Function;

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
     */
    template<typename T>
    class Optional;

    #ifdef KERNEL
    /**
     * @tparam T
     */
    template<typename T>
    class NonnullLockRefPtr;

    /**
     * @tparam T
     */
    template<typename T>
    struct LockRefPtrTraits;

    /**
     * @tparam T
     */
    template<typename T, typename PtrTraits = LockRefPtrTraits<T>>
    class LockRefPtr;
    #endif

    /**
     * @tparam T
     */
    template<typename T>
    class RefPtr;

    /**
     * @tparam T
     */
    template<typename T, typename TDeleter = DefaultDelete<T>>
    class OwnPtr;

    /**
     * @tparam T
     */
    template<typename T>
    class WeakPtr;

    /**
     * @tparam T
     * @tparam inline_capacity
     */
    template<typename T, size_t inline_capacity = 0>
    requires(!IsRvalueReference<T>) class Vector;

    /**
     * @tparam T
     */
    template<typename T, typename ErrorType = Error>
    class [[nodiscard]] ErrorOr;

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::Array;
using Mods::Atomic;
using Mods::Badge;
using Mods::BigEndianInputBitStream;
using Mods::BigEndianOutputBitStream;
using Mods::Bitmap;
using Mods::ByteBuffer;
using Mods::Bytes;
using Mods::ByteString;
using Mods::CircularBuffer;
using Mods::CircularQueue;
using Mods::ConstrainedStream;
using Mods::CountingStream;
using Mods::DeprecatedFlyString;
using Mods::DeprecatedStringCodePointIterator;
using Mods::DoublyLinkedList;
using Mods::Duration;
using Mods::Error;
using Mods::ErrorOr;
using Mods::FixedArray;
using Mods::FixedPoint;
using Mods::FlyString;
using Mods::Function;
using Mods::GenericLexer;
using Mods::HashMap;
using Mods::HashTable;
using Mods::IPv4Address;
using Mods::JsonArray;
using Mods::JsonObject;
using Mods::JsonValue;
using Mods::LexicalPath;
using Mods::LittleEndianInputBitStream;
using Mods::LittleEndianOutputBitStream;
using Mods::NonnullOwnPtr;
using Mods::NonnullRefPtr;
using Mods::Optional;
using Mods::OwnPtr;
using Mods::ReadonlyBytes;
using Mods::RefPtr;
using Mods::SearchableCircularBuffer;
using Mods::SeekableStream;
using Mods::SinglyLinkedList;
using Mods::Span;
using Mods::StackInfo;
using Mods::Stream;
using Mods::String;
using Mods::StringBuilder;
using Mods::StringImpl;
using Mods::StringView;
using Mods::TrailingCodePointTransformation;
using Mods::Traits;
using Mods::UnixDateTime;
using Mods::Utf16View;
using Mods::Utf32CodePointIterator;
using Mods::Utf32View;
using Mods::Utf8CodePointIterator;
using Mods::Utf8View;
using Mods::Vector;

#    ifdef KERNEL
using Mods::LockRefPtr;
using Mods::LockRefPtrTraits;
using Mods::NonnullLockRefPtr;
#    endif

#endif
