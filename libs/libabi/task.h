#pragma once 

#include <libabi/filesystem.h>
#include <libabi/process.h>

#define TASK_STATE_LIST(__ENTRY) \
    _ENTRY(NONE)                 \


enum TaskState
{
#define TASK_STATE_ENUM_ENTRY(__state) TASK_STATE_##__state,
    TASK_STATE_LIST(TASK_STATE_ENUM_ENTRY)
        __TASK_STATE_COUNT
};

typedef unsigned int TaskFlags 


static inline const char *task_state_string(TaskState state)
{
    const char *state_strings[] = {TASK_STATE_LIST(TASK_STATE_STRING_ENTRY)};

    if (state >= 0 && state < __TASK_STATE_COUNT)
    {
        return state_strings[state];
    }

    return "undefined";
}