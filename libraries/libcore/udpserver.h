/**
 * @file udpserver.h
 * @author Krisna Pranav
 * @brief udp server
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/forward.h>
#include <mods/function.h>
#include <libcore/forward.h>
#include <libcore/object.h>
#include <libcore/socketaddress.h>

namespace Core
{

    class UDPServer : public Object
    {
    public:
        virtual ~UDPServer() override;

        bool is_bound() const 
        {
            return m_bound;
        }
    protected:
        /**
         * @brief Construct a new UDPServer object
         * 
         * @param parent 
         */
        explicit UDPServer(Object* parent = nullptr);

    private:
        int m_fd { -1 };
        bool m_bound { false };
        RefPtr<Notifier> m_notifier;
    }; // class UDPServer : public Object

} // namespace Core
