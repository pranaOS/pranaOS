/**
 * @file uciendpoint.h
 * @author Krisna Pranav
 * @brief uci end point
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libchess/ucicommand.h>
#include <libcore/iodevice.h>
#include <libcore/notifier.h>
#include <libcore/object.h>

namespace Chess::UCI 
{

    class Endpoint : public Core::Object 
    {
        C_OBJECT(Endpoint)
    public:
        /**
         * @brief Destroy the Endpoint object
         * 
         */
        virtual ~Endpoint() override = default;

        virtual void handle_uci() { }
        virtual void handle_debug(DebugCommand const&) { }
        virtual void handle_isready() { }
        virtual void handle_setoption(SetOptionCommand const&) { }
        virtual void handle_position(PositionCommand const&) { }
        virtual void handle_go(GoCommand const&) { }
        virtual void handle_stop() { }
        virtual void handle_id(IdCommand const&) { }
        virtual void handle_uciok() { }
        virtual void handle_readyok() { }
        virtual void handle_bestmove(BestMoveCommand const&) { }
        virtual void handle_info(InfoCommand const&) { }

        void send_command(Command const&);

        virtual void event(Core::Event&) override;

        /**
         * @return Core::IODevice& 
         */
        Core::IODevice& in() 
        { 
            return *m_in; 
        }

        /**
         * @return Core::IODevice& 
         */
        Core::IODevice& out() 
        { 
            return *m_out; 
        }

        /**
         * @brief Set the in object
         * 
         * @param in 
         */
        void set_in(RefPtr<Core::IODevice> in)
        {
            m_in = in;
            set_in_notifier();
        }

        /**
         * @brief Set the out object
         * 
         * @param out 
         */
        void set_out(RefPtr<Core::IODevice> out) 
        { 
            m_out = out; 
        }

    protected:
        /**
         * @brief Construct a new Endpoint object
         * 
         */
        Endpoint() = default;

        /**
         * @brief Construct a new Endpoint object
         * 
         * @param in 
         * @param out 
         */
        Endpoint(NonnullRefPtr<Core::IODevice> in, NonnullRefPtr<Core::IODevice> out);

    private:
        void set_in_notifier();
        NonnullOwnPtr<Command> read_command();

        RefPtr<Core::IODevice> m_in;
        RefPtr<Core::IODevice> m_out;
        RefPtr<Core::Notifier> m_in_notifier;
    }; // class Endpoint : public Core::Object

} // namespace Chess::UCI
