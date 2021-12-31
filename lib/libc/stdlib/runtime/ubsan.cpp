#include <stdlib/runtime/ubsan.h>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>

static FILE* _ubsan_output = stderr;

struct ubsan_source_location {
    const char* filename;
    uint32_t line;
    uint32_t column;
};

enum ubsan_kind
{
    UBSAN_KIND_INTERGER,
    UBSAN_KIND_FLOAT,
    UNSAN_KIND_UNKNOWN = 0xffff
};

struct ubsan_type_descriptor
{
    uint16_t kind;
    uint16_t info;
    char name[1];

    bool is_signed_int() const
    {
        return kind == UBSAN_KIND_INTERGER && (info & 1);
    }

    bool is_unsigned_int() const
    {
        return kind == UBSAN_KIND_INTERGER && !(info & 1);
    }
};

struct ubsan_type_mismatch_info
{
    ubsan_source_location location;
    ubsan_type_descriptor* type;
    uintptr_t alignment;
    uint8_t type_check_kind;
};

struct ubsan_out_of_bounds_info
{
    ubsan_source_location location;
    ubsan_type_descriptor* lhs_type;
    ubsan_type_descriptor* rhs_type;
};

struct ubsan_overflow_info
{
    ubsan_source_location location;
    ubsan_type_descriptor* type;
};

struct ubsan_unreachable_info
{
    ubsan_source_location location;
};

struct ubsan_shift_out_of_bounds_info
{
    ubsan_source_location location;
    ubsan_type_descriptor* lhs_type;
    ubsan_type_descriptor* rhs_type;
};

struct ubsan_vla_bound_not_positive_info
{
    ubsan_source_location location;
    ubsan_type_descriptor* type;
};

static void ubsan_log_info(ubsan_source_location* location, const char* fmt, ...)
{
    fprintf(_ubsan_output, "ubsan! %s:%d:%d\n", location->filename, location->line, location->column);

    va_list v;
    va_start(v, fmt);

    vfprintf(_ubsan_output, fmt, v);

    fputc('\n', _ubsan_output);

    va_end(v);
}

static void ubsan_handle_type_mismatch(ubsan_type_mismatch_info* info, uintptr_t pointer)
{
    if((void*)pointer == nullptr)
    {
        ubsan_log_info(&info->location, "nullptr referenced\n");
    }
}

static void ubsan_handle_pointer_overflow(ubsan_overflow_info* info, uintptr_t base, uintptr_t result)
{
    if(base >= 0 && result >= 0)
    {
        if(base > result)
        {
            ubsan_log_info(&info->location, "addition of an unsigned offset to %d overflowed to %d\n", base, result);
        }
        else
        {
            ubsan_log_info(&info->location, "subtraction of an unsigned offset to %d overflowed to %d\n", base, result);
        }
    }
    else
    {
        ubsan_log_info(&info->location, "pointer index expression with base %d overflowed to %d\n", base, result);
    }
}

static void ubsan_handle_out_of_bounds(ubsan_out_of_bounds_info* info, uintptr_t index)
{
    ubsan_log_info(&info->location, "index %d is out of bounds for type %s\n", index, info->lhs_type->name);
}