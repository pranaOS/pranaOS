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
    class Bitmap;
    class ByteBuffer;
    class DebugLogStream;
    class IPv4Address;
    class JsonArray;
    class JsonObject;
    class JsonValue;
    class LogStream;

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
}

using Mods::Array;