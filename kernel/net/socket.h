/**
 * @file socket.h
 * @author Krisna Pranav
 * @brief socket
 * @version 6.0
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        Yes = 1
    }; // ShouldBlock

    class FileDescription;

    class Socket : public File 
    {
    public:

        /**
         * @param domain 
         * @param type 
         * @param protocol 
         * @return KResultOr<NonnullRefPtr<Socket>> 
         */
        static KResultOr<NonnullRefPtr<Socket>> create(int domain, int type, int protocol);

        // @brief Destroy the Socket object
        virtual ~Socket() override;

        /**
         * @return int 
         */
        int domain() const 
        { 
            return m_domain; 
        }

        int type() const 
        { 
            return m_type; 
        }

        int protocol() const 
        { 
            return m_protocol; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_shut_down_for_writing() const 
        { 
            return m_shut_down_for_writing; 
        }

        bool is_shut_down_for_reading() const 
        { 
            return m_shut_down_for_reading; 
        }

        enum class SetupState 
        {
            Unstarted,  
            InProgress, 
            Completed,  
        }; // enum

        enum class Role : u8 
        {
            None,
            Listener,
            Accepted,
            Connected,
            Connecting
        }; // enum

        /**
         * @param setup_state 
         * @return const char* 
         */
        static const char* to_string(SetupState setup_state)
        {
            switch (setup_state) {
            case SetupState::Unstarted:
                return "Unstarted";
            case SetupState::InProgress:
                return "InProgress";
            case SetupState::Completed:
                return "Completed";
            default:
                return "None";
            }
        }

        /**
         * @return SetupState 
         */
        SetupState setup_state() const 
        { 
            return m_setup_state; 
        }

        /**
         * @param setup_state 
         */
        void set_setup_state(SetupState setup_state);

        /**
         * @return Role 
         */
        virtual Role role(const FileDescription&) const 
        { 
            return m_role; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_connected() const 
        { 
            return m_connected; 
        }

        /// @brief: set_connected
        void set_connected(bool);

        /**
         * @return true 
         * @return false 
         */
        bool can_accept() const 
        { 
            return !m_pending.is_empty(); 
        }

        /**
         * @return RefPtr<Socket> 
         */
        RefPtr<Socket> accept();

        /**
         * @param how 
         * @return KResult 
         */
        KResult shutdown(int how);

        /**
         * @return KResult 
         */
        virtual KResult bind(Userspace<const sockaddr*>, socklen_t) = 0;
        virtual KResult connect(FileDescription&, Userspace<const sockaddr*>, socklen_t, ShouldBlock) = 0;
        virtual KResult listen(size_t) = 0;

        /// @breif: get[local + peer address]
        virtual void get_local_address(sockaddr*, socklen_t*) = 0;
        virtual void get_peer_address(sockaddr*, socklen_t*) = 0;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_local() const 
        { 
            return false; 
        }

        virtual bool is_ipv4() const 
        { 
            return false; 
        }
        
        /// @breif: attach, detach
        virtual void attach(FileDescription&) = 0;
        virtual void detach(FileDescription&) = 0;

        /**
         * @param flags 
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> sendto(FileDescription&, const UserOrKernelBuffer&, size_t, int flags, Userspace<const sockaddr*>, socklen_t) = 0;

        /**
         * @param flags 
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> recvfrom(FileDescription&, UserOrKernelBuffer&, size_t, int flags, Userspace<sockaddr*>, Userspace<socklen_t*>, timeval&) = 0;

        /**
         * @param level 
         * @param option 
         * @return KResult 
         */
        virtual KResult setsockopt(int level, int option, Userspace<const void*>, socklen_t);

        /**
         * @param level 
         * @param option 
         * @return KResult 
         */
        virtual KResult getsockopt(FileDescription&, int level, int option, Userspace<void*>, Userspace<socklen_t*>);

        /**
         * @return pid_t 
         */
        pid_t origin_pid() const 
        { 
            return m_origin.pid; 
        }

        uid_t origin_uid() const 
        { 
            return m_origin.uid; 
        }

        gid_t origin_gid() const 
        { 
            return m_origin.gid; 
        }

        pid_t acceptor_pid() const 
        { 
            return m_acceptor.pid; 
        }

        uid_t acceptor_uid() const 
        { 
            return m_acceptor.uid; 
        }

        gid_t acceptor_gid() const 
        { 
            return m_acceptor.gid; 
        }

        /**
         * @return const RefPtr<NetworkAdapter> 
         */
        const RefPtr<NetworkAdapter> bound_interface() const 
        { 
            return m_bound_interface; 
        }

        /**
         * @return Lock& 
         */
        Lock& lock() 
        { 
            return m_lock; 
        }

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override final;
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override final;

        /**
         * @return KResult 
         */
        virtual KResult stat(::stat&) const override;

        /**
         * @return String 
         */
        virtual String absolute_path(const FileDescription&) const override = 0;

        /// @brief: has receive 
        bool has_receive_timeout() const 
        { 
            return m_receive_timeout.tv_sec || m_receive_timeout.tv_usec; 
        }

        /**
         * @return const timeval& 
         */
        const timeval& receive_timeout() const 
        { 
            return m_receive_timeout; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool has_send_timeout() const 
        { 
            return m_send_timeout.tv_sec || m_send_timeout.tv_usec; 
        }

        /**
         * @return const timeval& 
         */
        const timeval& send_timeout() const 
        { 
            return m_send_timeout; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool wants_timestamp() const 
        { 
            return m_timestamp; 
        }

    protected:
        /**
         * @param domain 
         * @param type 
         * @param protocol 
         */
        Socket(int domain, int type, int protocol);

        /**
         * @return KResult 
         */
        KResult queue_connection_from(NonnullRefPtr<Socket>);

        /**
         * @return size_t 
         */
        size_t backlog() const 
        { 
            return m_backlog; 
        }

        /**
         * @param backlog 
         */
        void set_backlog(size_t backlog) 
        { 
            m_backlog = backlog; 
        }   

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "Socket"; 
        }

        /// @brief: shutdown for reading + writing.
        virtual void shut_down_for_reading() { }
        virtual void shut_down_for_writing() { }

        Role m_role { Role::None };

    protected:
        ucred m_origin { 0, 0, 0 };
        ucred m_acceptor { 0, 0, 0 };

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

        SetupState m_setup_state { SetupState::Unstarted };

        bool m_connected { false };
        bool m_shut_down_for_reading { false };
        bool m_shut_down_for_writing { false };

        RefPtr<NetworkAdapter> m_bound_interface { nullptr };

        timeval m_receive_timeout { 0, 0 };
        timeval m_send_timeout { 0, 0 };

        int m_timestamp { 0 };

        NonnullRefPtrVector<Socket> m_pending;
    }; // class Socket

    /**
     * @tparam SocketType 
     */
    template<typename SocketType>
    class SocketHandle 
    {
    public:
        /// @brief Construct a new Socket Handle object
        SocketHandle() { }

        /**
         * @param socket 
         */
        SocketHandle(NonnullRefPtr<SocketType>&& socket)
            : m_socket(move(socket))
        {
            if (m_socket)
                m_socket->lock().lock();
        }

        /**
         * @param other 
         */
        SocketHandle(SocketHandle&& other)
            : m_socket(move(other.m_socket))
        {
        }

        /// @brief Destroy the Socket Handle object
        ~SocketHandle()
        {
            if (m_socket)
                m_socket->lock().unlock();
        }

        SocketHandle(const SocketHandle&) = delete;

        /**
         * @return SocketHandle& 
         */
        SocketHandle& operator=(const SocketHandle&) = delete;

        /**
         * @return true 
         * @return false 
         */
        operator bool() const 
        {
            return m_socket; 
        }

        /**
         * @return SocketType* 
         */
        SocketType* operator->() 
        { 
            return &socket();
        }

        /**
         * @return const SocketType* 
         */
        const SocketType* operator->() const 
        { 
            return &socket(); 
        }

        /**
         * @return SocketType& 
         */
        SocketType& socket() 
        { 
            return *m_socket; 
        }

        /**
         * @return const SocketType& 
         */
        const SocketType& socket() const 
        { 
            return *m_socket; 
        }

    private:
        RefPtr<SocketType> m_socket;
    }; // class SocketHandle

} // namespace Kernel
