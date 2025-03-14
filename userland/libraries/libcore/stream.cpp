/**
 * @file stream.cpp
 * @author Krisna Pranav
 * @brief stream
 * @version 6.0
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "stream.h"
#include <libcore/system.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef __pranaos__
#    include <pranaos.h>
#endif
#ifdef __FreeBSD__
#    include <sys/ucred.h>
#endif

namespace Core::Stream 
{

    /**
     * @param buffer 
     * @return true 
     * @return false 
     */
    bool Stream::read_or_error(Bytes buffer)
    {
        VERIFY(buffer.size());

        size_t nread = 0;

        do {
            if (is_eof())
                return false;

            auto result = read(buffer.slice(nread));
            if (result.is_error()) {
                if (result.error().is_errno() && result.error().code() == EINTR) {
                    continue;
                }

                return false;
            }

            nread += result.value().size();

        } while (nread < buffer.size());

        return true;
    }

    /**
     * @param buffer 
     * @return true 
     * @return false 
     */
    bool Stream::write_or_error(ReadonlyBytes buffer)
    {
        VERIFY(buffer.size());

        size_t nwritten = 0;

        do {
            auto result = write(buffer.slice(nwritten));
            if (result.is_error()) {
                if (result.error().is_errno() && result.error().code() == EINTR) {
                    continue;
                }

                return false;
            }

            nwritten += result.value();

        } while (nwritten < buffer.size());

        return true;
    }

    /**
     * @return ErrorOr<off_t> 
     */
    ErrorOr<off_t> SeekableStream::tell() const
    {
        return const_cast<SeekableStream*>(this)->seek(0, SeekMode::FromCurrentPosition);
    }

    /**
     * @return ErrorOr<off_t> 
     */
    ErrorOr<off_t> SeekableStream::size()
    {
        auto original_position = TRY(tell());
        auto seek_result = seek(0, SeekMode::FromEndPosition);

        if (seek_result.is_error()) {
            auto restore_result = seek(original_position, SeekMode::SetPosition);

            if (restore_result.is_error()) {
                dbgln("Core::SeekableStream::size: Couldn't restore initial position, stream might have incorrect position now!");
            }

            return seek_result.release_error();
        }

        TRY(seek(original_position, SeekMode::SetPosition));
        return seek_result.value();
    }

    /**
     * @param filename 
     * @param mode 
     * @param permissions 
     * @return ErrorOr<NonnullOwnPtr<File>> 
     */
    ErrorOr<NonnullOwnPtr<File>> File::open(StringView filename, OpenMode mode, mode_t permissions)
    {
        auto file = TRY(adopt_nonnull_own_or_enomem(new (nothrow) File(mode)));
        TRY(file->open_path(filename, permissions));
        return file;
    }

    /**
     * @param fd 
     * @param mode 
     * @return ErrorOr<NonnullOwnPtr<File>> 
     */
    ErrorOr<NonnullOwnPtr<File>> File::adopt_fd(int fd, OpenMode mode)
    {
        if (fd < 0) {
            return Error::from_errno(EBADF);
        }

        if (!has_any_flag(mode, OpenMode::ReadWrite)) {
            dbgln("Core::File::adopt_fd: Attempting to adopt a file with neither Read nor Write specified in mode");
            return Error::from_errno(EINVAL);
        }

        auto file = TRY(adopt_nonnull_own_or_enomem(new (nothrow) File(mode)));
        file->m_fd = fd;

        return file;
    }

    /**
     * @param mode 
     * @return int 
     */
    int File::open_mode_to_options(OpenMode mode)
    {
        int flags = 0;

        if (has_flag(mode, OpenMode::ReadWrite)) {
            flags |= O_RDWR | O_CREAT;
        } else if (has_flag(mode, OpenMode::Read)) {
            flags |= O_RDONLY;
        } else if (has_flag(mode, OpenMode::Write)) {
            flags |= O_WRONLY | O_CREAT;
            bool should_truncate = !has_any_flag(mode, OpenMode::Append | OpenMode::MustBeNew);
            if (should_truncate)
                flags |= O_TRUNC;
        }

        if (has_flag(mode, OpenMode::Append))
            flags |= O_APPEND;
        if (has_flag(mode, OpenMode::Truncate))
            flags |= O_TRUNC;
        if (has_flag(mode, OpenMode::MustBeNew))
            flags |= O_EXCL;
        if (!has_flag(mode, OpenMode::KeepOnExec))
            flags |= O_CLOEXEC;
        if (!has_flag(mode, OpenMode::Nonblocking))
            flags |= O_NONBLOCK;
        return flags;
    }

    /**
     * @param filename 
     * @param permissions 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> File::open_path(StringView filename, mode_t permissions)
    {
        VERIFY(m_fd == -1);
        auto flags = open_mode_to_options(m_mode);

        m_fd = TRY(System::open(filename.characters_without_null_termination(), flags, permissions));
        return {};
    }

    /**
     * @return true 
     * @return false 
     */
    bool File::is_readable() const 
    { 
        return has_flag(m_mode, OpenMode::Read); 
    }


    bool File::is_writable() const 
    { 
        return has_flag(m_mode, OpenMode::Write); 
    }

    /**
     * @param buffer 
     * @return ErrorOr<Bytes> 
     */
    ErrorOr<Bytes> File::read(Bytes buffer)
    {
        if (!has_flag(m_mode, OpenMode::Read)) {
            return Error::from_errno(EBADF);
        }

        ssize_t nread = TRY(System::read(m_fd, buffer));
        m_last_read_was_eof = nread == 0;
        return buffer.trim(nread);
    }

    /**
     * @param buffer 
     * @return ErrorOr<size_t> 
     */
    ErrorOr<size_t> File::write(ReadonlyBytes buffer)
    {
        if (!has_flag(m_mode, OpenMode::Write)) {
            return Error::from_errno(EBADF);
        }

        return TRY(System::write(m_fd, buffer));
    }

    /**
     * @return true 
     * @return false 
     */
    bool File::is_eof() const 
    { 
        return m_last_read_was_eof; 
    }

    bool File::is_open() const 
    { 
        return m_fd >= 0; 
    }

    void File::close()
    {
        if (!is_open()) {
            return;
        }

        ErrorOr<void> result;
        do {
            result = System::close(m_fd);
        } while (result.is_error() && result.error().code() == EINTR);

        VERIFY(!result.is_error());
        m_fd = -1;
    }

    /**
     * @param offset 
     * @param mode 
     * @return ErrorOr<off_t> 
     */
    ErrorOr<off_t> File::seek(i64 offset, SeekMode mode)
    {
        int syscall_mode;
        switch (mode) {
        case SeekMode::SetPosition:
            syscall_mode = SEEK_SET;
            break;
        case SeekMode::FromCurrentPosition:
            syscall_mode = SEEK_CUR;
            break;
        case SeekMode::FromEndPosition:
            syscall_mode = SEEK_END;
            break;
        default:
            VERIFY_NOT_REACHED();
        }

        off_t seek_result = TRY(System::lseek(m_fd, offset, syscall_mode));
        m_last_read_was_eof = false;
        return seek_result;
    }

    /**
     * @param length 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> File::truncate(off_t length)
    {
        return System::ftruncate(m_fd, length);
    }

    /**
     * @param domain 
     * @param type 
     * @return ErrorOr<int> 
     */
    ErrorOr<int> Socket::create_fd(SocketDomain domain, SocketType type)
    {
        int socket_domain;
        switch (domain) {
        case SocketDomain::Inet:
            socket_domain = AF_INET;
            break;
        case SocketDomain::Local:
            socket_domain = AF_LOCAL;
            break;
        default:
            VERIFY_NOT_REACHED();
        }

        int socket_type;
        switch (type) {
        case SocketType::Stream:
            socket_type = SOCK_STREAM;
            break;
        case SocketType::Datagram:
            socket_type = SOCK_DGRAM;
            break;
        default:
            VERIFY_NOT_REACHED();
        }

    #ifdef SOCK_CLOEXEC
        return System::socket(socket_domain, socket_type | SOCK_CLOEXEC, 0);
    #else
        auto fd = TRY(System::socket(socket_domain, socket_type, 0));
        TRY(System::fcntl(fd, F_SETFD, FD_CLOEXEC));
        return fd;
    #endif
    }

    /**
     * @param host 
     * @param type 
     * @return ErrorOr<IPv4Address> 
     */
    ErrorOr<IPv4Address> Socket::resolve_host(String const& host, SocketType type)
    {
        int socket_type;
        switch (type) {
        case SocketType::Stream:
            socket_type = SOCK_STREAM;
            break;
        case SocketType::Datagram:
            socket_type = SOCK_DGRAM;
            break;
        default:
            VERIFY_NOT_REACHED();
        }

        struct addrinfo hints = {};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = socket_type;
        hints.ai_flags = 0;
        hints.ai_protocol = 0;

        struct addrinfo* results = nullptr;
        int rc = getaddrinfo(host.characters(), nullptr, &hints, &results);
        if (rc != 0) {
            if (rc == EAI_SYSTEM) {
                return Error::from_syscall("getaddrinfo", -errno);
            }

            return Error::from_string_literal(gai_strerror(rc));
        }

        auto* socket_address = bit_cast<struct sockaddr_in*>(results->ai_addr);
        NetworkOrdered<u32> network_ordered_address { socket_address->sin_addr.s_addr };

        freeaddrinfo(results);

        return IPv4Address { network_ordered_address };
    }

    /**
     * @param fd 
     * @param path 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Socket::connect_local(int fd, String const& path)
    {
        auto address = SocketAddress::local(path);
        auto maybe_sockaddr = address.to_sockaddr_un();

        if (!maybe_sockaddr.has_value()) {
            dbgln("Core::Stream::Socket::connect_local: Could not obtain a sockaddr_un");
            return Error::from_errno(EINVAL);
        }

        auto addr = maybe_sockaddr.release_value();
        return System::connect(fd, bit_cast<struct sockaddr*>(&addr), sizeof(addr));
    }

    /**
     * @param fd 
     * @param address 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Socket::connect_inet(int fd, SocketAddress const& address)
    {
        auto addr = address.to_sockaddr_in();
        return System::connect(fd, bit_cast<struct sockaddr*>(&addr), sizeof(addr));
    }

    /**
     * @param buffer 
     * @param flags 
     * @return ErrorOr<Bytes> 
     */
    ErrorOr<Bytes> PosixSocketHelper::read(Bytes buffer, int flags)
    {
        if (!is_open()) {
            return Error::from_errno(ENOTCONN);
        }

        ssize_t nread = TRY(System::recv(m_fd, buffer.data(), buffer.size(), flags));
        m_last_read_was_eof = nread == 0;

        if (m_last_read_was_eof && m_notifier)
            m_notifier->set_enabled(false);

        return buffer.trim(nread);
    }

    /**
     * @param buffer 
     * @return ErrorOr<size_t> 
     */
    ErrorOr<size_t> PosixSocketHelper::write(ReadonlyBytes buffer)
    {
        if (!is_open()) {
            return Error::from_errno(ENOTCONN);
        }

        return TRY(System::send(m_fd, buffer.data(), buffer.size(), 0));
    }
    
    void PosixSocketHelper::close()
    {
        if (!is_open()) {
            return;
        }

        if (m_notifier)
            m_notifier->set_enabled(false);

        ErrorOr<void> result;
        do {
            result = System::close(m_fd);
        } while (result.is_error() && result.error().code() == EINTR);

        VERIFY(!result.is_error());
        m_fd = -1;
    }

    /**
     * @param timeout 
     * @return ErrorOr<bool> 
     */
    ErrorOr<bool> PosixSocketHelper::can_read_without_blocking(int timeout) const
    {
        struct pollfd the_fd = { .fd = m_fd, .events = POLLIN, .revents = 0 };

        int rc;
        do {
            rc = ::poll(&the_fd, 1, timeout);
        } while (rc < 0 && errno == EINTR);

        if (rc < 0) {
            return Error::from_syscall("poll", -errno);
        }

        return (the_fd.revents & POLLIN) > 0;
    }

    /**
     * @param enabled 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> PosixSocketHelper::set_blocking(bool enabled)
    {
        int value = enabled ? 0 : 1;
        return System::ioctl(m_fd, FIONBIO, &value);
    }

    /**
     * @param enabled 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> PosixSocketHelper::set_close_on_exec(bool enabled)
    {
        int flags = TRY(System::fcntl(m_fd, F_GETFD));

        if (enabled)
            flags |= FD_CLOEXEC;
        else
            flags &= ~FD_CLOEXEC;

        TRY(System::fcntl(m_fd, F_SETFD, flags));
        return {};
    }

    /**
     * @param timeout 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> PosixSocketHelper::set_receive_timeout(Time timeout)
    {
        auto timeout_spec = timeout.to_timespec();
        return System::setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout_spec, sizeof(timeout_spec));
    }

    void PosixSocketHelper::setup_notifier()
    {
        if (!m_notifier)
            m_notifier = Core::Notifier::construct(m_fd, Core::Notifier::Read);
    }   

    /**
     * @param host 
     * @param port 
     * @return ErrorOr<NonnullOwnPtr<TCPSocket>> 
     */
    ErrorOr<NonnullOwnPtr<TCPSocket>> TCPSocket::connect(String const& host, u16 port)
    {
        auto ip_address = TRY(resolve_host(host, SocketType::Stream));
        return connect(SocketAddress { ip_address, port });
    }

    /**
     * @param address 
     * @return ErrorOr<NonnullOwnPtr<TCPSocket>> 
     */
    ErrorOr<NonnullOwnPtr<TCPSocket>> TCPSocket::connect(SocketAddress const& address)
    {
        auto socket = TRY(adopt_nonnull_own_or_enomem(new (nothrow) TCPSocket()));

        auto fd = TRY(create_fd(SocketDomain::Inet, SocketType::Stream));
        socket->m_helper.set_fd(fd);

        TRY(connect_inet(fd, address));

        socket->setup_notifier();
        return socket;
    }

    /**
     * @param fd 
     * @return ErrorOr<NonnullOwnPtr<TCPSocket>> 
     */
    ErrorOr<NonnullOwnPtr<TCPSocket>> TCPSocket::adopt_fd(int fd)
    {
        if (fd < 0) {
            return Error::from_errno(EBADF);
        }

        auto socket = TRY(adopt_nonnull_own_or_enomem(new (nothrow) TCPSocket()));
        socket->m_helper.set_fd(fd);
        socket->setup_notifier();
        return socket;
    }

    /**
     * @return ErrorOr<size_t> 
     */
    ErrorOr<size_t> PosixSocketHelper::pending_bytes() const
    {
        if (!is_open()) {
            return Error::from_errno(ENOTCONN);
        }

        int value;
        TRY(System::ioctl(m_fd, FIONREAD, &value));
        return static_cast<size_t>(value);
    }

    /**
     * @param host 
     * @param port 
     * @param timeout 
     * @return ErrorOr<NonnullOwnPtr<UDPSocket>> 
     */
    ErrorOr<NonnullOwnPtr<UDPSocket>> UDPSocket::connect(String const& host, u16 port, Optional<Time> timeout)
    {
        auto ip_address = TRY(resolve_host(host, SocketType::Datagram));
        return connect(SocketAddress { ip_address, port }, timeout);
    }

    /**
     * @param address 
     * @param timeout 
     * @return ErrorOr<NonnullOwnPtr<UDPSocket>> 
     */
    ErrorOr<NonnullOwnPtr<UDPSocket>> UDPSocket::connect(SocketAddress const& address, Optional<Time> timeout)
    {
        auto socket = TRY(adopt_nonnull_own_or_enomem(new (nothrow) UDPSocket()));

        auto fd = TRY(create_fd(SocketDomain::Inet, SocketType::Datagram));
        socket->m_helper.set_fd(fd);
        if (timeout.has_value()) {
            TRY(socket->m_helper.set_receive_timeout(timeout.value()));
        }

        TRY(connect_inet(fd, address));

        socket->setup_notifier();
        return socket;
    }

    /**
     * @param path 
     * @return ErrorOr<NonnullOwnPtr<LocalSocket>> 
     */
    ErrorOr<NonnullOwnPtr<LocalSocket>> LocalSocket::connect(String const& path)
    {
        auto socket = TRY(adopt_nonnull_own_or_enomem(new (nothrow) LocalSocket()));

        auto fd = TRY(create_fd(SocketDomain::Local, SocketType::Stream));
        socket->m_helper.set_fd(fd);

        TRY(connect_local(fd, path));

        socket->setup_notifier();
        return socket;
    }

    /**
     * @param fd 
     * @return ErrorOr<NonnullOwnPtr<LocalSocket>> 
     */
    ErrorOr<NonnullOwnPtr<LocalSocket>> LocalSocket::adopt_fd(int fd)
    {
        if (fd < 0) {
            return Error::from_errno(EBADF);
        }

        auto socket = TRY(adopt_nonnull_own_or_enomem(new (nothrow) LocalSocket()));
        socket->m_helper.set_fd(fd);
        socket->setup_notifier();
        return socket;
    }

    /**
     * @param flags 
     * @return ErrorOr<int> 
     */
    ErrorOr<int> LocalSocket::receive_fd(int flags)
    {
    #ifdef __pranaos__
        return Core::System::recvfd(m_helper.fd(), flags);
    #else
        (void)flags;
        return Error::from_string_literal("File descriptor passing not supported on this platform");
    #endif
    }

    /**
     * @param fd 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> LocalSocket::send_fd(int fd)
    {
    #ifdef __pranaos__
        return Core::System::sendfd(m_helper.fd(), fd);
    #else
        (void)fd;
        return Error::from_string_literal("File descriptor passing not supported on this platform");
    #endif
    }

    /**
     * @return ErrorOr<pid_t> 
     */
    ErrorOr<pid_t> LocalSocket::peer_pid() const
    {
    #ifdef MODS_OS_MACOS
        pid_t pid;
        socklen_t pid_size = sizeof(pid);
    #elif defined(__FreeBSD__)
        struct xucred creds = {};
        socklen_t creds_size = sizeof(creds);
    #elif defined(__OpenBSD__)
        struct sockpeercred creds = {};
        socklen_t creds_size = sizeof(creds);
    #else
        struct ucred creds = {};
        socklen_t creds_size = sizeof(creds);
    #endif

    #ifdef MODS_OS_MACOS
        TRY(System::getsockopt(m_helper.fd(), SOL_LOCAL, LOCAL_PEERPID, &pid, &pid_size));
        return pid;
    #elif defined(__FreeBSD__)
        TRY(System::getsockopt(m_helper.fd(), SOL_LOCAL, LOCAL_PEERCRED, &creds, &creds_size));
        return creds.cr_pid;
    #else
        TRY(System::getsockopt(m_helper.fd(), SOL_SOCKET, SO_PEERCRED, &creds, &creds_size));
        return creds.pid;
    #endif
    }

    /**
     * @param buffer 
     * @return ErrorOr<Bytes> 
     */
    ErrorOr<Bytes> LocalSocket::read_without_waiting(Bytes buffer)
    {
        return m_helper.read(buffer, MSG_DONTWAIT);
    }

    /**
     * @return ErrorOr<int> 
     */
    ErrorOr<int> LocalSocket::release_fd()
    {
        if (!is_open()) {
            return Error::from_errno(ENOTCONN);
        }

        auto fd = m_helper.fd();
        m_helper.set_fd(-1);
        return fd;
    }

} // namespace Core
