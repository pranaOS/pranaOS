/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <EP/Types.h>

namespace EP {

namespace Detail {
template<size_t inline_capacity>
class ByteBuffer;
}

class Bitmap;
using ByteBuffer = EP::Detail::ByteBuffer<32>;
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

template<typename T, EP::MemoryOrder DefaultMemoryOrder>
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

using EP::Array;
using EP::Atomic;
using EP::Badge;
using EP::Bitmap;
using EP::ByteBuffer;
using EP::Bytes;
using EP::CircularDuplexStream;
using EP::CircularQueue;
using EP::DoublyLinkedList;
using EP::DuplexMemoryStream;
using EP::FixedArray;
using EP::FlyString;
using EP::Function;
using EP::HashMap;
using EP::HashTable;
using EP::InputBitStream;
using EP::InputMemoryStream;
using EP::InputStream;
using EP::IPv4Address;
using EP::JsonArray;
using EP::JsonObject;
using EP::JsonValue;
using EP::NonnullOwnPtr;
using EP::NonnullOwnPtrVector;
using EP::NonnullRefPtr;
using EP::NonnullRefPtrVector;
using EP::Optional;
using EP::OutputBitStream;
using EP::OutputMemoryStream;
using EP::OutputStream;
using EP::OwnPtr;
using EP::ReadonlyBytes;
using EP::RefPtr;
using EP::SinglyLinkedList;
using EP::Span;
using EP::StackInfo;
using EP::String;
using EP::StringBuilder;
using EP::StringImpl;
using EP::StringView;
using EP::Time;
using EP::Traits;
using EP::URL;
using EP::Utf16View;
using EP::Utf32View;
using EP::Utf8View;
using EP::Vector;
