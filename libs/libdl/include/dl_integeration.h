/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libutils/result.h>
#include <libc/include/string.h>

struct DlErrorMessage {
   DlErrorMessage(String&& other)
       : text(move(other))
   {
   }

   virtual ~DlErrorMessage() { }

   String text;
};

struct __Dl_info;
typedef struct __Dl_info Dl_info;

/* Error Message */
typedef Result<void, DlErrorMessage> (*DlCloseFunction)(void*);
typedef Result<void*, DlErrorMessage> (*DlOpenFunction)(const char*, int);
typedef Result<void*, DlErrorMessage> (*DlSymFunction)(void*, const char*);
typedef Result<void, DlErrorMessage> (*DlAddrFunction)(void*, Dl_info*);

/* call */
extern "C" {
extern DlCloseFunction __dlclose;
extern DlOpenFunction __dlopen;
extern DlSymFunction __dlsym;
extern DlAddrFunction __dladdr;
}