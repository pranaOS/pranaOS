/**
 * @file process_statistics_reader.h
 * @author Krisna Pranav
 * @brief process statitics reader
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/hashmap.h>
#include <mods/string.h>
#include <unistd.h>

namespace Core
{

    struct ThreadStatistics 
    {
        pid_t tid;
        unsigned times_scheduled;
        unsigned ticks;
        unsigned syscall_count;
        unsigned zero_faults;
        String state;
        u32 cpu;
        u32 priority;
        u32 effective_priority;
        String name;
    };

    class ProcessStatisticsReader 
    {
    public:
        /**
         * @return HashMap<pid_t, Core::ProcessStatisticsReader> 
         */
        static HashMap<pid_t, Core::ProcessStatisticsReader> get_all();

    private:
        /**
         * @return String 
         */
        static String username_from_uid(uid_t);
        
        static HashMap<uid_t, String> s_username;
    }; // class Process Statistics Reader 

} // namespace Core