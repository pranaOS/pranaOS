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

typedef Result<void, DlErrorMessage> (*DlCloseFunction)(void*);
typedef Result<void*, DlErrorMessage> (*DlOpenFunction)(const char*, int);
typedef Result<void*, DlErrorMessage> (*DlSymFunction)(void*, const char*);
typedef Result<void, DlErrorMessage> (*DlAddrFunction)(void*, Dl_info*);

