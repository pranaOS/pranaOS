/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/types.h>

namespace Utils {

namespace Detail {
template<size_t inline_capacity>
class ByteBuffer;
}

class Bitmap;
using ByteBuffer = Utils::Detail::ByteBuffer<32>;
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
class Utf8View;
class InputStream;
class InputMemoryStream;
class DuplexMemoryStream;
class OutputStream;
class InputBitStream;
class OutputBitStream;
class OutputMemoryStream;

template<size_t Capacity>
class CircularDuplexStream;

template<typename T>
class Span;

template<typename T, size_t Size>
struct Array;

template<typename Container, typename ValueType>
class SimpleIterator;

using ReadonlyBytes = Span<const u8>;
using Bytes = Span<u8>;

template<typename T, Utils::MemoryOrder DefaultMemoryOrder>
class Atomic;

template<typename T>
class SinglyLinkedList;

template<typename T>
class DoublyLinkedList;

template<typename T, size_t capacity>
class CircularQueue;

template<typename T>
struct Traits;

template<typename T, typename TraitsForT = Traits<T>, bool IsOrdered = false>
class HashTable;

template<typename T, typename TraitsForT = Traits<T>>
using OrderedHashTable = HashTable<T, TraitsForT, true>;

template<typename K, typename V, typename KeyTraits = Traits<K>, bool IsOrdered = false>
class HashMap;

template<typename K, typename V, typename KeyTraits = Traits<K>>
using OrderedHashMap = HashMap<K, V, KeyTraits, true>;

template<typename T>
class Badge;

template<typename T>
class FixedArray;

template<typename>
class Function;

template<typename Out, typename... In>
class Function<Out(In...)>;

template<typename T>
class NonnullRefPtr;

template<typename T>
class NonnullOwnPtr;

template<typename T, size_t inline_capacity = 0>
class NonnullRefPtrVector;

template<typename T, size_t inline_capacity = 0>
class NonnullOwnPtrVector;

template<typename T>
class Optional;

template<typename T>
struct RefPtrTraits;

template<typename T, typename PtrTraits = RefPtrTraits<T>>
class RefPtr;

template<typename T>
class OwnPtr;

template<typename T>
class WeakPtr;

template<typename T, size_t inline_capacity = 0>
requires(!IsRvalueReference<T>) class Vector;

}

using Utils::Array;
using Utils::Atomic;
using Utils::Badge;
using Utils::Bitmap;
using Utils::ByteBuffer;
using Utils::Bytes;
using Utils::CircularDuplexStream;
using Utils::CircularQueue;
using Utils::DoublyLinkedList;
using Utils::DuplexMemoryStream;
using Utils::FixedArray;
using Utils::FlyString;
using Utils::Function;
using Utils::GenericLexer;
using Utils::HashMap;
using Utils::HashTable;
using Utils::InputBitStream;
using Utils::InputMemoryStream;
using Utils::InputStream;
using Utils::IPv4Address;
using Utils::JsonArray;
using Utils::JsonObject;
using Utils::JsonValue;
using Utils::NonnullOwnPtr;
using Utils::NonnullOwnPtrVector;
using Utils::NonnullRefPtr;
using Utils::NonnullRefPtrVector;
using Utils::Optional;
using Utils::OutputBitStream;
using Utils::OutputMemoryStream;
using Utils::OutputStream;
using Utils::OwnPtr;
using Utils::ReadonlyBytes;
using Utils::RefPtr;
using Utils::SinglyLinkedList;
using Utils::Span;
using Utils::StackInfo;
using Utils::String;
using Utils::StringBuilder;
using Utils::StringImpl;
using Utils::StringView;
using Utils::Time;
using Utils::Traits;
using Utils::URL;
using Utils::Utf16View;
using Utils::Utf32View;
using Utils::Utf8View;
using Utils::Vector;