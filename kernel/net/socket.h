/**
 * @file socket.h
 * @author Krisna Pranav
 * @brief socket
 * @version 1.0
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/hashtable.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/refcounted.h>
#include <kernel/lock.h>
#include <kernel/kresult.h>
#include <kernel/unixtypes.h>
#include <kernel/filesystem/file.h>
#include <kernel/net/network_adapter.h>

namespace Kernel
{

    enum class ShouldBlock
    {
        No = 0,
        Yes = 1,
    }; // enum

    class FileDescription;

    class Socket : public File
    {
    public:
        enum class SetupState
        {
            Unstarted,
            InProgress,
            Completed,
        }; 
        
        enum class Role : u8
        {
            None,
            Listener,
            Accepted,
            Connected,
            Connecting
        };

        /**
         * @param setup_state 
         * @return const char* 
         */
        static const char* to_string(SetupState setup_state)
        {
            switch (setup_state) {
            case SetupState::Unstarted:
                return "Unsarted";
            default:
                return "None";
            }
        }  
    
    private:
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_socket() const final
        {
            return true;
        }

        Lock m_lock { "Socket" };

        int m_domain { 0 };
        int m_type { 0 };
        int m_protocol { 0 };

        size_t m_backlog { 0 };
    }; // class Socket  

} // namespace Kernel