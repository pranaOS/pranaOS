/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>

namespace Base {

namespace Detail {
template<size_t inline_capacity>
class ByteBuffer;
}

class Bitmap;
using ByteBuffer = Base::Detail::ByteBuffer<32>;
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

template<typename T, Base::MemoryOrder DefaultMemoryOrder>
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
class WeBasePtr;

template<typename T, size_t inline_capacity = 0>
requires(!IsRvalueReference<T>) class Vector;

}

using Base::Array;
using Base::Atomic;
using Base::Badge;
using Base::Bitmap;
using Base::ByteBuffer;
using Base::Bytes;
using Base::CircularDuplexStream;
using Base::CircularQueue;
using Base::DoublyLinkedList;
using Base::DuplexMemoryStream;
using Base::FixedArray;
using Base::FlyString;
using Base::Function;
using Base::HashMap;
using Base::HashTable;
using Base::InputBitStream;
using Base::InputMemoryStream;
using Base::InputStream;
using Base::IPv4Address;
using Base::JsonArray;
using Base::JsonObject;
using Base::JsonValue;
using Base::NonnullOwnPtr;
using Base::NonnullOwnPtrVector;
using Base::NonnullRefPtr;
using Base::NonnullRefPtrVector;
using Base::Optional;
using Base::OutputBitStream;
using Base::OutputMemoryStream;
using Base::OutputStream;
using Base::OwnPtr;
using Base::ReadonlyBytes;
using Base::RefPtr;
using Base::SinglyLinkedList;
using Base::Span;
using Base::StackInfo;
using Base::String;
using Base::StringBuilder;
using Base::StringImpl;
using Base::StringView;
using Base::Time;
using Base::Traits;
using Base::URL;
using Base::Utf32View;
using Base::Utf8View;
using Base::Vector;