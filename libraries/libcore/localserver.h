/**
 * @file localserver.h
 * @author Krisna Pranav
 * @brief local server
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <LibCore/Notifier.h>
#include <LibCore/Object.h>
#include <LibCore/Stream.h>

namespace Core 
{

    class LocalServer : public Object 
    {
        C_OBJECT(LocalServer)
    public:
        /**
         * @brief Destroy the LocalServer object
         * 
         */
        virtual ~LocalServer() override;

        /**
         * @param path 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> take_over_from_system_server(String const& path = String());

        /**
         * @return true 
         * @return false 
         */
        bool is_listening() const 
        { 
            return m_listening; 
        }

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool listen(String const& address);

        /**
         * @return ErrorOr<NonnullOwnPtr<Stream::LocalSocket>> 
         */
        ErrorOr<NonnullOwnPtr<Stream::LocalSocket>> accept();

        Function<void(NonnullOwnPtr<Stream::LocalSocket>)> on_accept;

    private:
        /**
         * @brief Construct a new LocalServer object
         * 
         * @param parent 
         */
        explicit LocalServer(Object* parent = nullptr);

        void setup_notifier();

        int m_fd { -1 };
        bool m_listening { false };
        RefPtr<Notifier> m_notifier;
    }; // class LocalServer : public Object

} // namespace Core
