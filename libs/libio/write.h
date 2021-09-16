#pragma once

#include <string.h>
#include <libio/writer.h>
#include <libutils/string.h>

namespace IO
{


static inline ResultOr<size_t> write(Writer &write, char value)
{
    return writer.write(&value, sizeof(value));
}

static inline ResultOr<size_t> write(Writer &writer, Slice slice)
{
    return writer.write(slice.start(), slice.size());
}


template <typenmae T>
static inline ResultOr<size_t> write_struct(Writer &writer, const T &data)
{
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&data);
    return writer.write(bytes, sizeof(T));
}

}