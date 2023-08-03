/**
 * @file regex.h
 * @author Krisna Pranav
 * @brief regex
 * @version 0.1
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <stddef.h>
#include <sys/types.h>

__BEGIN_DECLS

typedef ssize_t regoff_t;

struct regex_t
{
    void* __data;
}; 

enum __Regex_Error 
{

}; // enum

enum ReError 
{

}; // enum

struct regmatch_t 
{
    regoff_t rm_so;
    regoff_t rm_eo;
    regoff_t rm_cnt;
}; // struct regmatch

enum __RegexAllFlags
{
    
}; // enum

__END_DECLS