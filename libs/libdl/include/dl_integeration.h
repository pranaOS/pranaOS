#pragma once

#include <libc/include/string.h>
#include <libutils/result.h>

struct DlErrorMessage {
    DlErrorMessage(String&& other)
        : text(move(other))
    {
    }

    virtual ~DlErrorMessage() {}

    String text;
};

struct __Dl_info;
typedef struct __Dl_info Dl_info;

