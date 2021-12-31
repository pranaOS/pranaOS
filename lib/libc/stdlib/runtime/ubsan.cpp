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