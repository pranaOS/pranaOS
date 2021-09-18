/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/types.h>

namespace Utils {

namespace Detail {
template <size_t inline_capacity>
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

template <size_t Capacity>
class CircularDuplexStream;

template <typename T>
class Span;

template <typename T, size_t Size>
struct Array;

using ReadonlyBytes = Span<const u8>;
using Bytes = Span<u8>;

template <typename T, Utils::MemoryOrder DefaultMemoryOrder>
class Atomic;

template <typename T, size_t inline_capacity = 0>
requires(!IsRvalueReference<T>) class Vector;

}

using Utils::Array;
using Utils::Atomic;