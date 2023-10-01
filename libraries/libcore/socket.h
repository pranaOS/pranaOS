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
    public:
        virtual ~Socket() override;

        Type type() const
        {
            return m_type;
        }
    
    protected:
        Socket(Type, Object* parent);

        SocketAddress m_source_addresss;
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
        virtual bool open(IODevice::OpenMode) override { ASSERT_NOT_REACHED(); }
        void ensure_read_notifier();

        Type m_type { Type::Invalid };
        RefPtr<Notifier> m_notifier;
        RefPtr<Notifier> m_read_notifier;

    }; // class Socket

} // namespace Core