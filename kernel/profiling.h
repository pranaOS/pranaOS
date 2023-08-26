/**
 * @file profiling.h
 * @author Krisna Pranav
 * @brief profiling
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <mods/string.h>
#include <mods/types.h>

namespace Kernel
{
    class Process;

    namespace Profiling
    {
        constexpr size_t max_stack_frame_count = 50;

        struct Sample
        {
            ProcessID pid;
            ThreadID tid;
            u64 timestamp;
            u32 frames[max_stacks_frame_count];
        }; 

        extern ProcessID pid();
        extern String& executable_path();

        Sample& next_sample_slot();

        void for_each_sample(Function<void(Sample&)>);

    } // namespace Profiling

} // namespace Kernel