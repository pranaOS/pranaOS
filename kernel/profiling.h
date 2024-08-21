/**
 * @file profiling.h
 * @author Krisna Pranav
 * @brief profiling
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
            u32 frames[max_stack_frame_count];
        }; // struct 

        extern ProcessID pid();
        extern String& executable_path();

        /// @breif: next slot
        Sample& next_sample_slot();
        
        /// @brief: start & stop
        void start(Process&);
        void stop();

        /**
         * @brief did exec?
         * 
         * @param new_executable_path 
         */
        void did_exec(const String& new_executable_path);

        /// @broef: each sample for
        void for_each_sample(Function<void(Sample&)>);

    } // namespace Profiling

} // namespace Kernel