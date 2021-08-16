/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/Types.h>

typedef struct
{
    long a_type; 
    union {
        long a_val;
        void* a_ptr;
        void (*a_fnc)(); 
    } a_un;
} auxv_t;

static_assert(sizeof(auxv_t) % sizeof(FlatPtr) == 0);

#define AT_NULL 0           
#define AT_IGNORE 1         
#define AT_EXECFD 2         
#define AT_PHDR 3           
#define AT_PHENT 4          
#define AT_PHNUM 5          
#define AT_PAGESZ 6         
#define AT_BASE 7           
#define AT_FLAGS 8          
#define AT_ENTRY 9          
#define AT_NOTELF 10        
#define AT_UID 11           
#define AT_EUID 12          
#define AT_GID 13           
#define AT_EGID 14          
#define AT_PLATFORM 15      
#define AT_HWCAP 16         
#define AT_CLKTCK 17        
#define AT_SECURE 23        
#define AT_BASE_PLATFORM 24 
#define AT_RANDOM 25        
#define AT_HWCAP2 26        
#define AT_EXECFN 31        
#define AT_EXE_BASE 32       
#define AT_EXE_SIZE 33      

namespace ELF {

struct AuxiliaryValue {
    enum Type {
        Null = AT_NULL,
        Ignore = AT_IGNORE,
        ExecFileDescriptor = AT_EXECFD,
        Phdr = AT_PHDR,
        Phent = AT_PHENT,
        Phnum = AT_PHNUM,
        PageSize = AT_PAGESZ,
        BaseAddress = AT_BASE,
        Flags = AT_FLAGS,
        Entry = AT_ENTRY,
        NotELF = AT_NOTELF,
        Uid = AT_UID,
        EUid = AT_EUID,
        Gid = AT_GID,
        EGid = AT_EGID,
        Platform = AT_PLATFORM,
        HwCap = AT_HWCAP,
        ClockTick = AT_CLKTCK,
        Secure = AT_SECURE,
        BasePlatform = AT_BASE_PLATFORM,
        Random = AT_RANDOM,
        HwCap2 = AT_HWCAP2,
        ExecFilename = AT_EXECFN,
        ExeBaseAddress = AT_EXE_BASE,
        ExeSize = AT_EXE_SIZE
    };

    AuxiliaryValue(Type type, long val)
    {
        auxv.a_type = type;
        auxv.a_un.a_val = val;
    }
    AuxiliaryValue(Type type, void* ptr)
    {
        auxv.a_type = type;
        auxv.a_un.a_ptr = (void*)ptr;
    }
    AuxiliaryValue(Type type, String string)
    {
        auxv.a_type = type;
        auxv.a_un.a_ptr = nullptr;
        optional_string = string;
    }

    auxv_t auxv {};
    String optional_string;
};

}
