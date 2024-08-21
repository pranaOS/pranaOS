/**
 * @file process_statistics_reader.h
 * @author Krisna Pranav
 * @brief process statitics reader
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        unsigned inode_faults;
        unsigned zero_faults;
        unsigned cow_faults;
        unsigned unix_socket_read_bytes;
        unsigned unix_socket_write_bytes;
        unsigned ipv4_socket_read_bytes;
        unsigned ipv4_socket_write_bytes;
        unsigned file_read_bytes;
        unsigned file_write_bytes;

        String state;
        String name;

        u32 cpu;
        u32 priority;
        u32 effective_priority;        
    }; // struct ThreadStatistics

    struct ProcessStatistics 
    {
        pid_t pid;
        pid_t pgid;
        pid_t pgp;
        pid_t sid;
        uid_t uid;
        gid_t gid;
        pid_t ppid;

        unsigned nfds;

        String name;
        String tty;
        String pledge;
        String veil;

        size_t amount_virtual;
        size_t amount_resident;
        size_t amount_shared;
        size_t amount_dirty_private;
        size_t amount_clean_inode;
        size_t amount_purgeable_volatile;
        size_t amount_purgeable_nonvolatile;

        int icon_id;

        Vector<Core::ThreadStatistics> threads;

        String username;
    }; // struct ProcessStatistics

    class ProcessStatisticsReader 
    {
    public:
        /**
         * @return HashMap<pid_t, Core::ProcessStatistics> 
         */
        static HashMap<pid_t, Core::ProcessStatistics> get_all();

    private:
        /**
         * @return String 
         */
        static String username_from_uid(uid_t);
        static HashMap<uid_t, String> s_usernames;
    }; // class ProcessStatisticsReader

} // namespace Core