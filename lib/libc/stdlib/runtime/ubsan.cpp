#include <stdlib/runtime/ubsan.h>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>

static FILE* _ubsan_output = stderr;

struct ubsan_source_location
{
    const char* filename;
    uint32_t line;
    uint32_t column;
};

enum ubsan_kind
{
    UBSAN_KIND_INTERGER,
    UBSAN_KIND_FLOAT,
    UBSAN_KIND_UNKNOWN = 0xffff
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

static void ubsan_handle_int_overflow(ubsan_overflow_info* info, uintptr_t lhs, char op, uintptr_t rhs)
{
    ubsan_log_info(&info->location,
                   "%s interger overflow: %d %c %d cannot be represented as type %s\n",
                   (info->type->is_signed_int() ? "signed" : "unsigned"),
                   lhs,
                   op,
                   rhs,
                   info->type->name);
}

static void ubsan_handle_divrem_overflow(ubsan_overflow_info* info, uintptr_t lhs, uintptr_t rhs)
{
    if(info->type->is_signed_int() && rhs == -1)
    {
        ubsan_log_info(&info->location, "division of %d by -1 cannot be represented as type %s\n", lhs, info->type->name);
    }
    else if(info->type->kind == UBSAN_KIND_INTERGER || info->type->kind == UBSAN_KIND_FLOAT)
    {
        ubsan_log_info(&info->location, "division by zero\n");
    }
}

static void ubsan_handle_negate_overflow(ubsan_overflow_info* info, uintptr_t old_data)
{
    if(info->type->is_signed_int())
    {
        ubsan_log_info(&info->location,
                       "negation of %d cannot be represented as type %s;"
                       "cast to an unsigned type to negate this value to itself\n",
                       old_data,
                       info->type->name);
    }
    else
    {
        ubsan_log_info(&info->location, "negation of %d cannot be represented as type %s\n", old_data, info->type->name);
    }
}

void ubsan_handle_load_invalid_value(ubsan_overflow_info* info, uintptr_t data)
{
    ubsan_log_info(&info->location, "loading value %d which is not a valid value for type %s\n", data, info->type->name);
}

static void ubsan_handle_missing_return(ubsan_unreachable_info* info)
{
    ubsan_log_info(&info->location, "no return statement in a function that returns a non-void type\n");
}

__BEGIN_DECLS
void __ubsan_handle_type_mismatch(ubsan_type_mismatch_info* info, uintptr_t pointer)
{
    ubsan_handle_type_mismatch(info, pointer);
}

void __ubsan_handle_type_mismatch_v1(ubsan_type_mismatch_info* info, uintptr_t pointer)
{
    ubsan_handle_type_mismatch(info, pointer);
}

void __ubsan_handle_pointer_overflow(ubsan_overflow_info* info, uintptr_t base, uintptr_t result)
{
    ubsan_handle_pointer_overflow(info, base, result);
}

void __ubsan_handle_out_of_bounds(ubsan_out_of_bounds_info* info, uintptr_t index)
{
    ubsan_handle_out_of_bounds(info, index);
}

void __ubsan_handle_sub_overflow(ubsan_overflow_info* info, uintptr_t lhs, uintptr_t rhs)
{
    ubsan_handle_int_overflow(info, lhs, '-', rhs);
}

void __ubsan_handle_add_overflow(ubsan_overflow_info* info, uintptr_t lhs, uintptr_t rhs)
{
    ubsan_handle_int_overflow(info, lhs, '+', rhs);
}

void __ubsan_handle_mul_overflow(ubsan_overflow_info* info, uintptr_t lhs, uintptr_t rhs)
{
    ubsan_handle_int_overflow(info, lhs, '*', rhs);
}

void __ubsan_handle_divrem_overflow(ubsan_overflow_info* info, uintptr_t lhs, uintptr_t rhs)
{
    ubsan_handle_divrem_overflow(info, lhs, rhs);
}

void __ubsan_handle_negate_overflow(ubsan_overflow_info* info, uintptr_t old_data)
{
    ubsan_handle_negate_overflow(info, old_data);
}

void __ubsan_handle_load_invalid_value(ubsan_overflow_info* info, uintptr_t data)
{
    ubsan_handle_load_invalid_value(info, data);
}

void __ubsan_handle_missing_return(ubsan_unreachable_info* info)
{
    ubsan_handle_missing_return(info);
}

void __ubsan_handle_shift_out_of_bounds(ubsan_shift_out_of_bounds_info* info, uintptr_t lhs, uintptr_t rhs)
{
}

void __ubsan_handle_vla_bound_not_positive(ubsan_vla_bound_not_positive_info* info, uintptr_t bound)
{
}

void __ubsan_set_output_file(FILE* file)
{
    _ubsan_output = file;
}

__END_DECLS