#pragma once

#include <libabi/filesystem.h>
#include <libabi/process.h>

enum TaskState
{
#define TASK_STATE_ENUM_ENTRY(__state) TASK_STATE_##__state,
    TASK_STATE_LIST(TASK_STATE_ENUM_ENTRY)
        _TASK_STATE_COUNT
};

#define TASK_NONE (0)
#define TASK_WAITABLE (1 << 0)
#define TASK_USER (1 << 1)

typedef unsigned int TaskFlags;

static inline const char *task_state_string(TaskState state)
{
#define TASK_STATE_STRING_ENTRY(__state) #__state,

    const char *state_strings[] = {TASK_STATE_LIST(TASK_STATE_STRING_ENTRY)};

    if (state >= 0 && state < __TASK_STATE_COUNT)
    {
        return state_strings[state];
    }

    return task_state_string;
}