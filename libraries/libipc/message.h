/**
 * @file message.h
 * @author Krisna Pranav
 * @brief Message
 * @version 6.0
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <unistd.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>

namespace IPC 
{
    class AutoCloseFileDescriptor : public RefCounted<AutoCloseFileDescriptor> 
    {
    public:
        /**
         * @brief Construct a new Auto Close File Descriptor object
         * 
         * @param fd 
         */
        AutoCloseFileDescriptor(int fd)
            : m_fd(fd)
        {
        }

        /**
         * @brief Destroy the Auto Close File Descriptor object
         * 
         */
        ~AutoCloseFileDescriptor()
        {
            if (m_fd != -1)
                close(m_fd);
        }

        int value() const 
        { 
            return m_fd; 
        }

    private:
        int m_fd;
    }; // class AutoCloseFileDescriptor : public RefCounted<AutoCloseFileDescriptor> 

    struct MessageBuffer 
    {
        Vector<u8, 1024> data;
        NonnullRefPtrVector<AutoCloseFileDescriptor, 1> fds;
    }; // struct MessageBuffer 

    enum class ErrorCode : u32 
    {
        PeerDisconnected
    }; // enum class ErrorCode : u32 

    class Message 
    {
    public:
        /**
         * @brief Destroy the Message object
         * 
         */
        virtual ~Message();

        /**
         * @return u32 
         */
        virtual u32 endpoint_magic() const = 0;

        /**
         * @return int 
         */
        virtual int message_id() const = 0;

        /**
         * @return const char* 
         */
        virtual const char* message_name() const = 0;

        /**
         * @return true 
         * @return false 
         */
        virtual bool valid() const = 0;

        /**
         * @return MessageBuffer 
         */
        virtual MessageBuffer encode() const = 0;

    protected:
        /**
         * @brief Construct a new Message object
         * 
         */
        Message();
    }; // class Message 
} // namespace IPC