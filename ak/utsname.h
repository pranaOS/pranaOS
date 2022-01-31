#pragma once

#define UTSNAME_LENGTH 65
#define UTSNAME_DOMAIN_LENGTH UTSNAME_LENGTH
#define UTSNAME_NODENAME_LENGTH
#define UTSNAME_VERSION_LENGTH

#ifndef UTSNAME_SYSNAME_LENGTH
#define UTSNAME_SYSNAME_LENGTH UTSNAME_LENGTH
#endif 


/**
 * @brief uts name
 * 
 */
struct utsname
{
    char sysname[UTSNAME_SYSNAME_LENGTH];
    char version[UTSNAME_VERSION_LENGTH];
};
