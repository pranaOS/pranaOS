/**
 * @file auxv.h
 * @author Krisna Pranav
 * @brief auxv
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

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

typedef struct
{
    long a_type; 
    union {
        long a_val;
        void* a_ptr;
        void (*a_fnc)(void); 
    } a_un;
} auxv_t;

/**
 * @param type 
 * @return long 
 */
long getauxval(long type);

__END_DECLS
