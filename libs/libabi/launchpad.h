#pragma once

// includes
#include <libabi/filesystem>
#include <libabi/process.h>
#include <libabi/task.h>

struct LaunchpadArgument
{
    char *buffer;
    size_t size;
};

struct Launchpad
{
    char name[PROCESS_NAME_SIZE];
    char executable[PATH_LENGTH];

    int argc;

    char *env;
    size_t env_size;

    int handles;
};