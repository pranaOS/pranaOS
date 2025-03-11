/**
 * @file dlfcn_integration.h
 * @author Krisna Pranav
 * @brief DLFCN Integration
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/result.h>
#include <mods/string.h>

struct DlErrorMessage {

    /**
     * @brief Construct a new DlErrorMessage object
     * 
     * @param other 
     */
    DlErrorMessage(String&& other)
        : text(move(other))
    {
    }

    /**
     * @brief Destroy the DlErrorMessage object
     * 
     */
    virtual ~DlErrorMessage() = default;

    String text;
}; // struct DlErrorMessage

struct __Dl_info;
typedef struct __Dl_info Dl_info;

typedef Result<void, DlErrorMessage> (*DlCloseFunction)(void*);
typedef Result<void*, DlErrorMessage> (*DlOpenFunction)(char const*, int);
typedef Result<void*, DlErrorMessage> (*DlSymFunction)(void*, char const*);
typedef Result<void, DlErrorMessage> (*DlAddrFunction)(void*, Dl_info*);

extern "C" {
extern DlCloseFunction __dlclose;
extern DlOpenFunction __dlopen;
extern DlSymFunction __dlsym;
extern DlAddrFunction __dladdr;
}
