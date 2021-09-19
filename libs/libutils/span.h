#pragma once

// includes
#include <libutils/assertions.h>
#include <libutils/iterator.h>
#include <libutils/typedtransfer.h>
#include <libutils/types.h>

namespace Utils {

namespace Detail {


template<typename T>
class Span {
public:
    ALWAYS_INLINE constexpr Span() = default;

    ALWAYS_INLINE constexpr Span(T* values, size_t size)
        : m_values(values)
        , m_size(size)
    {
    }

    template<size_t size>
    ALWAYS_INLINE constexpr Span(T (&values)[size])
        : m_values(values)
        , m_size(size)
    {
    }

protected:
    T* m_values { nullptr };
    size_t m_size { 0 };
};

template<>
class Span<u8> {
public:
    ALWAYS_INLINE constexpr Span() = default;

    ALWAYS_INLINE constexpr Span(u8* values, size_t size)
        : m_values(values)
        , m_size(size)
    {
    }
    ALWAYS_INLINE Span(void* values, size_t size)
        : m_values(reinterpret_cast<u8*>(values))
        , m_size(size)
    {
    }

protected:
    u8* m_values { nullptr };
    size_t m_size { 0 };
};

template<>
class Span<u8 const> {
public:
    ALWAYS_INLINE constexpr Span() = default;

    ALWAYS_INLINE constexpr Span(u8 const* values, size_t size)
        : m_values(values)
        , m_size(size)
    {
    }
    ALWAYS_INLINE Span(void const* values, size_t size)
        : m_values(reinterpret_cast<u8 const*>(values))
        , m_size(size)
    {
    }
    ALWAYS_INLINE Span(char const* values, size_t size)
        : m_values(reinterpret_cast<u8 const*>(values))
        , m_size(size)
    {
    }

protected:
    u8 const* m_values { nullptr };
    size_t m_size { 0 };
};

}


}

}