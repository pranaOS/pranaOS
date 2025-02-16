/**
 * @file ub_sanitizer.h
 * @author Krisna Pranav
 * @brief UB Sanitizer
 * @version 6.0
 * @date 2024-10-09
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/atomic.h>
#include <mods/noncopyable.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>

namespace Mods::UBSanitizer
{
    extern Atomic<bool> g_ubsan_is_deadly;

    typedef void* ValueHandle;

    class SourceLocation
    {
        MOD_MAKE_NONCOPYABLE(SourceLocation);

    public:
        /**
         * @return const char* 
         */
        const char* filename() const
        {
            return m_filename;
        }

        /**
         * @return u32 
         */
        u32 line() const
        {
            return m_line;
        }

        /**
         * @return u32 
         */
        u32 column() const
        {
            return m_column;
        }

        /**
         * @return SourceLocation 
         */
        SourceLocation permanently_clear()
        {
            return move(*this);
        }

        bool needs_logging() const
        {
            return !(m_filename == nullptr);
        }

        /**
         * @brief Construct a new Source Location object
         * 
         */
        SourceLocation() = default;

        /**
         * @brief Construct a new Source Location object
         * 
         * @param other 
         */
        SourceLocation(SourceLocation&& other)
            : m_filename(other.m_filename), m_line(other.m_line), m_column(other.m_column)
        {
            other = {};
        }

        /**
         * @param other 
         * @return SourceLocation& 
         */
        SourceLocation& operator=(SourceLocation&& other)
        {
            if(this != &other)
            {
                m_filename = exchange(other.m_filename, nullptr);
                m_line = exchange(other.m_line, 0);
                m_column = exchange(other.m_column, 0);
            }

            return *this;
        }

    private:
        const char* m_filename{nullptr};
        u32 m_line{0};
        u32 m_column{0};
    }; // class SourceLocation

    enum TypeKind : u16
    {
        Integer = 0,
        Float = 1,
        Unknown = 0xffff,
    }; // enum TypeKind : u16

    class TypeDescriptor
    {
    public:
        /**
         * @return const char* 
         */
        const char* name() const
        {
            return m_name;
        }

        /**
         * @return TypeKind 
         */
        TypeKind kind() const
        {
            return (TypeKind)m_kind;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_integer() const
        {
            return kind() == TypeKind::Integer;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_signed() const
        {
            return m_info & 1;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_unsigned() const
        {
            return !is_signed();
        }

        /**
         * @return size_t 
         */
        size_t bit_width() const
        {
            return 1 << (m_info >> 1);
        }

    private:
        u16 m_kind;
        u16 m_info;
        char m_name[1];
    }; // class TypeDescriptor

    struct InvalidValueData
    {
        SourceLocation location;
        const TypeDescriptor& type;
    }; // struct InvalidValueData

    struct NonnullArgData
    {
        SourceLocation location;
        SourceLocation attribute_location;
        int argument_index;
    }; // struct NonnullArgData

    struct NonnullReturnData
    {
        SourceLocation attribute_location;
    }; // struct NonnullReturnData

    struct OverflowData
    {
        SourceLocation location;
        const TypeDescriptor& type;
    }; // struct OverflowData

    struct VLABoundData
    {
        SourceLocation location;
        const TypeDescriptor& type;
    }; // struct VLABoundData

    struct ShiftOutOfBoundsData
    {
        SourceLocation location;
        const TypeDescriptor& lhs_type;
        const TypeDescriptor& rhs_type;
    }; // struct ShiftOutOfBoundsData

    struct OutOfBoundsData
    {
        SourceLocation location;
        const TypeDescriptor& array_type;
        const TypeDescriptor& index_type;
    }; // struct OutofBoundsData

    struct TypeMismatchData
    {
        SourceLocation location;
        const TypeDescriptor& type;
        u8 log_alignment;
        u8 type_check_kind;
    }; // struct TypeMismatchData

    struct AlignmentAssumptionData
    {
        SourceLocation location;
        SourceLocation assumption_location;
        const TypeDescriptor& type;
    }; // struct AlignmentAssumptionData

    struct UnreachableData
    {
        SourceLocation location;
    }; // struct UnreachableData

    struct ImplicitConversionData
    {
        SourceLocation location;
        const TypeDescriptor& from_type;
        const TypeDescriptor& to_type;
        unsigned char kind;
    }; // struct ImplicitConversionData

    struct InvalidBuiltinData
    {
        SourceLocation location;
        unsigned char kind;
    }; // struct InvalidBuiltinData

    struct PointerOverflowData
    {
        SourceLocation location;
    }; // struct PointerOverflowData

    struct FloatCastOverflowData
    {
        SourceLocation location;
        TypeDescriptor const& from_type;
        TypeDescriptor const& to_type;
    }; // struct FloatCastOverflowData

} // namespace Mods::UBSanitizer
