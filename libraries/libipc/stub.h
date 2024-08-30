/**
 * @file stub.h
 * @author Krisna Pranav
 * @brief STUB
 * @version 6.0
 * @date 2024-08-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownptr.h>
#include <mods/string.h>

namespace Mods
{
    class BufferStream;
} // namespace Mods

namespace IPC
{
    class Message;
    struct MessageBuffer;

    class Stub
    {
    public:
        virtual ~Stub();

        /**
         * @return u32 
         */
        virtual u32 magic() const = 0;

        /**
         * @return String 
         */
        virtual String name() const = 0;

        /**
         * @return OwnPtr<MessageBuffer> 
         */
        virtual OwnPtr<MessageBuffer> handle(const Message&) = 0;

    private:   
        String m_name;

    protected:
        Stub();
    }; // class Stub 
} // namespace IPC