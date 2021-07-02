#pragma once

#include <libutils/Prelude.h>

#define FILE_NAME_LENGHT (64)
#define PATH_LENGHT (512)
#define PATH_DEPTH (16)
#define PATH_SEPARATOR '/'

enum JWhence
{
    J_WHENCE_START,
    J_WHENCE_CURRENT,
    J_WHENCE_END,
};