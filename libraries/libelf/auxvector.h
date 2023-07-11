/**
 * @file auxvector.h
 * @author Krisna Pranav
 * @brief auxiliaryvector
 * @version 1.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

typedef struct 
{
    long a_type;
    union
    {
        long a_val;
        void* a_ptr;
        void (*a_fnc)();
    } a_un;
} auxv_t;

#define AT_NULL 0
#define AT_IGNORE 1
#define EXECFD 2
#define AT_PHDR = 3 

#ifdef __cplusplus
#   include <mods/string.h>
#   include <mods/types.h>

struct AuxiliaryValue
{
    enum Type
    {
        Null = AT_NULL,
        Ignore = AT_IGNORE,
        Phdr = AT_PHDR,
    };

    /**
     * @param type 
     * @param val 
     */
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


#endif 