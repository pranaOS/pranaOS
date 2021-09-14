#pragma once

#include <libutils/prelude.h>

struct printf_info;

typedef void (*printf_append_t)(struct printf_info *info, char c);

typedef enum
{
    PFSTATE_ESC,
    PFSTATE_PARSE,
    PFSTATE_FORMAT_LENGTH,
    PFSTATE_FINALIZE
} printf_state_t;

typedef struct printf_info
{
    char c;
    printf_state_t state;
    printf_append_t append;

    const char *format;
    int format_offset;

    char padding;
    printf_align_t align;
    size_t length;

    void *output;
    int written;
    int allocated;
} printf_info_t;

typedef struct
{
    char c;
    printf_formatter_impl_t impl;
} printf_formatter_t;

#define PRINTF_PEEK()
{                                                       \
    info->c = info->format[info->format_offset++];      \
    if (info->c == '\0')                                \
        return info->written;                           \
}                                                       \

int __printf(printf_info_t *info, va_list va);