/**
 * @file socket.h
 * @author Krisna Pranav
 * @brief socket
 * @version 6.0
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "notifier.h"
#include "socketaddress.h"
#include <libcore/iodevice.h>
#include <libcore/socketaddress.h>
#include <mods/function.h>
#include <mods/span.h>

namespace Core 
{
    class Socket : public IODevice 
    {
        C_OBJECT(Socket)
    public:
        enum class Type 
        {
            Invalid,
            TCP,
            UDP,
            Local,
        }; // enum class Type

        /// @brief Destroy the Socket object
        virtual ~Socket() override;

        /**
         * @return Type 
         */
        Type type() const 
        { 
            return m_type; 
        }

        /**
         * @param hostname 
         * @param port 
         * @return true 
         * @return false 
         */
        virtual bool connect(const String& hostname, int port);

        /**
         * @param port 
         * @return true 
         * @return false 
         */
        bool connect(const SocketAddress&, int port);

        /**
         * @return true 
         * @return false 
         */
        bool connect(const SocketAddress&);

        /**
         * @param max_size 
         * @return ByteBuffer 
         */
        ByteBuffer receive(int max_size);
        
        /**
         * @return true 
         * @return false 
         */
        bool send(ReadonlyBytes);

        bool is_connected() const 
        { 
            return m_connected; 
        }

        /**
         * @param blocking 
         */
        void set_blocking(bool blocking);

        /**
         * @return SocketAddress 
         */
        SocketAddress source_address() const 
        { 
            return m_source_address; 
        }

        /**
         * @return int 
         */
        int source_port() const 
        { 
            return m_source_port; 
        }

        /**
         * @return SocketAddress 
         */
        SocketAddress destination_address() const 
        { 
            return m_destination_address; 
        }

        /**
         * @return int 
         */
        int destination_port() const 
        { 
            return m_destination_port; 
        }

        Function<void()> on_connected;
        Function<void()> on_ready_to_read;

    protected:

        /**
         * @param parent 
         */
        Socket(Type, Object* parent);

        SocketAddress m_source_address;
        SocketAddress m_destination_address;

        int m_source_port { -1 };
        int m_destination_port { -1 };

        bool m_connected { false };

        virtual void did_update_fd(int) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool common_connect(const struct sockaddr*, socklen_t);

    private:
    
        /**
         * @return true 
         * @return false 
         */
        virtual bool open(IODevice::OpenMode) override 
        { 
            ASSERT_NOT_REACHED(); 
        }

        void ensure_read_notifier();

        Type m_type { Type::Invalid };
        RefPtr<Notifier> m_notifier;
        RefPtr<Notifier> m_read_notifier;
    }; // class Socket
} // namespace Core