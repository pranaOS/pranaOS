/**
 * @file stream.h
 * @author Krisna Pranav
 * @brief stream
 * @version 6.0
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/enumbits.h>
#include <mods/function.h>
#include <mods/ipv4address.h>
#include <mods/memmem.h>
#include <mods/noncopyable.h>
#include <mods/result.h>
#include <mods/span.h>
#include <mods/string.h>
#include <mods/time.h>
#include <mods/variant.h>
#include <libcore/notifier.h>
#include <libcore/socketaddress.h>
#include <errno.h>
#include <netdb.h>

namespace Core::Stream 
{

    class Stream 
    {
    public:
    
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const 
        { 
            return false; 
        }
        
        /**
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes) = 0;
        
        /**
         * @return true 
         * @return false 
         */
        virtual bool read_or_error(Bytes);

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_writable() const 
        { 
            return false; 
        }
        
        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes) = 0;
        
        /**
         * @return true 
         * @return false 
         */
        virtual bool write_or_error(ReadonlyBytes);

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const = 0;

        virtual bool is_open() const = 0;
        virtual void close() = 0;

        /**
         * @brief Destroy the Stream object
         * 
         */
        virtual ~Stream()
        {
        }
    }; // class Stream 

    enum class SeekMode {
        SetPosition,
        FromCurrentPosition,
        FromEndPosition,
    }; // enum class SeekMode

    
    class SeekableStream : public Stream 
    {
    public:
        /**
         * @param offset 
         * @return ErrorOr<off_t> 
         */
        virtual ErrorOr<off_t> seek(i64 offset, SeekMode) = 0;
        
        /**
         * @return ErrorOr<off_t> 
         */
        virtual ErrorOr<off_t> tell() const;
        
        /**
         * @return ErrorOr<off_t> 
         */
        virtual ErrorOr<off_t> size();
        
        /**
         * @param length 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> truncate(off_t length) = 0;
    }; // class SeekableStream : public Stream

    
    class Socket : public Stream 
    {
    public:
        /**
         * @brief Construct a new Socket object
         * 
         */
        Socket(Socket&&) = default;

        /**
         * @return Socket& 
         */
        Socket& operator=(Socket&&) = default;

        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> pending_bytes() const = 0;
        
        /**
         * @param timeout 
         * @return ErrorOr<bool> 
         */
        virtual ErrorOr<bool> can_read_without_blocking(int timeout = 0) const = 0;
        
        /**
         * @brief Set the blocking object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_blocking(bool enabled) = 0;
        
        /**
         * @brief Set the close on exec object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_close_on_exec(bool enabled) = 0;

        virtual void set_notifications_enabled(bool) { }

        Function<void()> on_ready_to_read;

    protected:
        enum class SocketDomain 
        {
            Local,
            Inet,
        }; // enum class SocketDomain 

        enum class SocketType 
        {
            Stream,
            Datagram,
        }; // enum class SocketType 

        /**
         * @brief Construct a new Socket object
         * 
         */
        Socket()
        {
        }
        
        /**
         * @brief Create a fd object
         * 
         * @return ErrorOr<int> 
         */
        static ErrorOr<int> create_fd(SocketDomain, SocketType);
        
        /**
         * @return ErrorOr<IPv4Address> 
         */
        static ErrorOr<IPv4Address> resolve_host(String const&, SocketType);

        /**
         * @param fd 
         * @param path 
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> connect_local(int fd, String const& path);

        /**
         * @param fd 
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> connect_inet(int fd, SocketAddress const&);
    }; // class Socket : public Stream 

    
    class ReusableSocket : public Socket 
    {
    public:
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_connected() = 0;
        
        /**
         * @param host 
         * @param port 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> reconnect(String const& host, u16 port) = 0;
        
        /**
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> reconnect(SocketAddress const&) = 0;
    }; // class ReusableSocket : public Socket 


    enum class OpenMode : unsigned {
        NotOpen = 0,
        Read = 1,
        Write = 2,
        ReadWrite = 3,
        Append = 4,
        Truncate = 8,
        MustBeNew = 16,
        KeepOnExec = 32,
        Nonblocking = 64,
    }; // enum class OpenMode : unsigned

    MODS_ENUM_BITWISE_OPERATORS(OpenMode)

    class File final : public SeekableStream 
    {
        MODS_MAKE_NONCOPYABLE(File);

    public:
        /**
         * @param filename 
         * @return ErrorOr<NonnullOwnPtr<File>> 
         */
        static ErrorOr<NonnullOwnPtr<File>> open(StringView filename, OpenMode, mode_t = 0644);

        /**
         * @param fd 
         * @return ErrorOr<NonnullOwnPtr<File>> 
         */
        static ErrorOr<NonnullOwnPtr<File>> adopt_fd(int fd, OpenMode);

        /**
         * @brief Construct a new File object
         * 
         * @param other 
         */
        File(File&& other) { operator=(move(other)); }

        /**
         * @param other 
         * @return File& 
         */
        File& operator=(File&& other)
        {
            if (&other == this)
                return *this;

            m_mode = exchange(other.m_mode, OpenMode::NotOpen);
            m_fd = exchange(other.m_fd, -1);
            m_last_read_was_eof = exchange(other.m_last_read_was_eof, false);
            return *this;
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const override;

        /**
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_writable() const override;

        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override;

        virtual void close() override;

        /**
         * @param offset 
         * @return ErrorOr<off_t> 
         */
        virtual ErrorOr<off_t> seek(i64 offset, SeekMode) override;

        /**
         * @param length 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> truncate(off_t length) override;

        /**
         * @brief Destroy the File object
         * 
         */
        virtual ~File() override 
        { 
            close(); 
        }

        /**
         * @param mode 
         * @return int 
         */
        static int open_mode_to_options(OpenMode mode);

    private:
        /**
         * @brief Construct a new File object
         * 
         * @param mode 
         */
        File(OpenMode mode)
            : m_mode(mode)
        {
        }

        /**
         * @param filename 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> open_path(StringView filename, mode_t);

        OpenMode m_mode { OpenMode::NotOpen };
        int m_fd { -1 };
        bool m_last_read_was_eof { false };
    }; // class File final : public SeekableStream

    class PosixSocketHelper 
    {
        MODS_MAKE_NONCOPYABLE(PosixSocketHelper);

    public:
        /**
         * @tparam T 
         */
        template<typename T>
        PosixSocketHelper(Badge<T>) requires(IsBaseOf<Socket, T>) { }

        /**
         * @brief Construct a new PosixSocketHelper object
         * 
         * @param other 
         */
        PosixSocketHelper(PosixSocketHelper&& other)
        {
            operator=(move(other));
        }

        /**
         * @param other 
         * @return PosixSocketHelper& 
         */
        PosixSocketHelper& operator=(PosixSocketHelper&& other)
        {
            m_fd = exchange(other.m_fd, -1);
            m_last_read_was_eof = exchange(other.m_last_read_was_eof, false);
            m_notifier = move(other.m_notifier);
            return *this;
        }

        /**
         * @return int 
         */
        int fd() const 
        { 
            return m_fd; 
        }

        /**
         * @brief Set the fd object
         * 
         * @param fd 
         */
        void set_fd(int fd) 
        { 
            m_fd = fd; 
        }

        /**
         * @param flags 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read(Bytes, int flags = 0);

        /**
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> write(ReadonlyBytes);

        /**
         * @return true 
         * @return false 
         */
        bool is_eof() const 
        { 
            return !is_open() || m_last_read_was_eof; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_open() const 
        { 
            return m_fd != -1; 
        }

        void close();

        /**
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> pending_bytes() const;

        /**
         * @param timeout 
         * @return ErrorOr<bool> 
         */
        ErrorOr<bool> can_read_without_blocking(int timeout) const;

        /**
         * @brief Set the blocking object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> set_blocking(bool enabled);

        /**
         * @brief Set the close on exec object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> set_close_on_exec(bool enabled);

        /**
         * @brief Set the receive timeout object
         * 
         * @param timeout 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> set_receive_timeout(Time timeout);

        void setup_notifier();

        /**
         * @return RefPtr<Core::Notifier> 
         */
        RefPtr<Core::Notifier> notifier() 
        { 
            return m_notifier; 
        }

    private:
        int m_fd { -1 };
        bool m_last_read_was_eof { false };
        RefPtr<Core::Notifier> m_notifier;
    }; // class PosixSocketHelper 

    class TCPSocket final : public Socket 
    {
    public:
        /**
         * @param host 
         * @param port 
         * @return ErrorOr<NonnullOwnPtr<TCPSocket>> 
         */
        static ErrorOr<NonnullOwnPtr<TCPSocket>> connect(String const& host, u16 port);

        /**
         * @param address 
         * @return ErrorOr<NonnullOwnPtr<TCPSocket>> 
         */
        static ErrorOr<NonnullOwnPtr<TCPSocket>> connect(SocketAddress const& address);
        
        /**
         * @param fd 
         * @return ErrorOr<NonnullOwnPtr<TCPSocket>> 
         */
        static ErrorOr<NonnullOwnPtr<TCPSocket>> adopt_fd(int fd);

        /**
         * @brief Construct a new TCPSocket object
         * 
         * @param other 
         */
        TCPSocket(TCPSocket&& other)
            : Socket(static_cast<Socket&&>(other))
            , m_helper(move(other.m_helper))
        {
            if (is_open())
                setup_notifier();
        }

        /**
         * @param other 
         * @return TCPSocket& 
         */
        TCPSocket& operator=(TCPSocket&& other)
        {
            Socket::operator=(static_cast<Socket&&>(other));
            m_helper = move(other.m_helper);
            if (is_open())
                setup_notifier();

            return *this;
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const override 
        { 
            return is_open(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_writable() const override 
        { 
            return is_open(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes buffer) override 
        { 
            return m_helper.read(buffer); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes buffer) override 
        { 
            return m_helper.write(buffer); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_helper.is_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return m_helper.is_open(); 
        };

        virtual void close() override 
        { 
            m_helper.close(); 
        };

        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> pending_bytes() const override 
        { 
            return m_helper.pending_bytes(); 
        }

        /**
         * @param timeout 
         * @return ErrorOr<bool> 
         */
        virtual ErrorOr<bool> can_read_without_blocking(int timeout = 0) const override 
        { 
            return m_helper.can_read_without_blocking(timeout); 
        }

        /**
         * @brief Set the notifications enabled object
         * 
         * @param enabled 
         */
        virtual void set_notifications_enabled(bool enabled) override
        {
            if (auto notifier = m_helper.notifier())
                notifier->set_enabled(enabled);
        }

        /**
         * @brief Set the blocking object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> set_blocking(bool enabled) override 
        { 
            return m_helper.set_blocking(enabled); 
        }

        /**
         * @brief Set the close on exec object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> set_close_on_exec(bool enabled) override 
        { 
            return m_helper.set_close_on_exec(enabled); 
        }

        /**
         * @brief Destroy the TCPSocket object
         * 
         */
        virtual ~TCPSocket() override { close(); }

    private:
        /**
         * @brief Construct a new TCPSocket object
         * 
         */
        TCPSocket()
        {
        }

        void setup_notifier()
        {
            VERIFY(is_open());

            m_helper.setup_notifier();
            m_helper.notifier()->on_ready_to_read = [this] {
                if (on_ready_to_read)
                    on_ready_to_read();
            };
        }

        PosixSocketHelper m_helper { Badge<TCPSocket> {} };
    }; // class TCPSocket final : public Socket 

    class UDPSocket final : public Socket 
    {
    public: 
        /**
         * @param host 
         * @param port 
         * @param timeout 
         * @return ErrorOr<NonnullOwnPtr<UDPSocket>> 
         */
        static ErrorOr<NonnullOwnPtr<UDPSocket>> connect(String const& host, u16 port, Optional<Time> timeout = {});

        /**
         * @param address 
         * @param timeout 
         * @return ErrorOr<NonnullOwnPtr<UDPSocket>> 
         */
        static ErrorOr<NonnullOwnPtr<UDPSocket>> connect(SocketAddress const& address, Optional<Time> timeout = {});

        /**
         * @brief Construct a new UDPSocket object
         * 
         * @param other 
         */
        UDPSocket(UDPSocket&& other)
            : Socket(static_cast<Socket&&>(other))
            , m_helper(move(other.m_helper))
        {
            if (is_open())
                setup_notifier();
        }

        /**
         * @param other 
         * @return UDPSocket& 
         */
        UDPSocket& operator=(UDPSocket&& other)
        {
            Socket::operator=(static_cast<Socket&&>(other));
            m_helper = move(other.m_helper);
            if (is_open())
                setup_notifier();

            return *this;
        }

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes buffer) override
        {
            auto pending_bytes = TRY(this->pending_bytes());
            if (pending_bytes > buffer.size()) {
                return Error::from_errno(EMSGSIZE);
            }

            return m_helper.read(buffer);
        }   

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const override 
        { 
            return is_open(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_writable() const override 
        { 
            return is_open(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes buffer) override 
        { 
            return m_helper.write(buffer); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_helper.is_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return m_helper.is_open(); 
        }

        virtual void close() override 
        { 
            m_helper.close(); 
        }

        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> pending_bytes() const override 
        { 
            return m_helper.pending_bytes(); 
        }

        /**
         * @param timeout 
         * @return ErrorOr<bool> 
         */
        virtual ErrorOr<bool> can_read_without_blocking(int timeout = 0) const override 
        { 
            return m_helper.can_read_without_blocking(timeout); 
        }

        /**
         * @brief Set the notifications enabled object
         * 
         * @param enabled 
         */
        virtual void set_notifications_enabled(bool enabled) override
        {
            if (auto notifier = m_helper.notifier())
                notifier->set_enabled(enabled);
        }

        /**
         * @brief Set the blocking object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> set_blocking(bool enabled) override 
        { 
            return m_helper.set_blocking(enabled); 
        }

        /**
         * @brief Set the close on exec object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> set_close_on_exec(bool enabled) override 
        { 
            return m_helper.set_close_on_exec(enabled); 
        }

        /**
         * @brief Destroy the UDPSocket object
         * 
         */
        virtual ~UDPSocket() override { close(); }

    private:
        /**
         * @brief Construct a new UDPSocket object
         * 
         */
        UDPSocket() = default;

        void setup_notifier()
        {
            VERIFY(is_open());

            m_helper.setup_notifier();
            m_helper.notifier()->on_ready_to_read = [this] {
                if (on_ready_to_read)
                    on_ready_to_read();
            };
        }

        PosixSocketHelper m_helper { Badge<UDPSocket> {} };
    }; // class UDPSocket final : public Socket 

    class LocalSocket final : public Socket 
    {
    public:
        /**
         * @param path 
         * @return ErrorOr<NonnullOwnPtr<LocalSocket>> 
         */
        static ErrorOr<NonnullOwnPtr<LocalSocket>> connect(String const& path);

        /**
         * @param fd 
         * @return ErrorOr<NonnullOwnPtr<LocalSocket>> 
         */
        static ErrorOr<NonnullOwnPtr<LocalSocket>> adopt_fd(int fd);

        /**
         * @brief Construct a new Local Socket object
         * 
         * @param other 
         */
        LocalSocket(LocalSocket&& other)
            : Socket(static_cast<Socket&&>(other))
            , m_helper(move(other.m_helper))
        {
            if (is_open())
                setup_notifier();
        }

        /**
         * @param other 
         * @return LocalSocket& 
         */
        LocalSocket& operator=(LocalSocket&& other)
        {
            Socket::operator=(static_cast<Socket&&>(other));
            m_helper = move(other.m_helper);
            if (is_open())
                setup_notifier();

            return *this;
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const override 
        { 
            return is_open(); 
        }
        
        virtual bool is_writable() const override 
        { 
            return is_open(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes buffer) override 
        { 
            return m_helper.read(buffer); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes buffer) override 
        { 
            return m_helper.write(buffer); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_helper.is_eof(); 
        }

        virtual bool is_open() const override 
        { 
            return m_helper.is_open(); 
        }

        virtual void close() override 
        { 
            m_helper.close(); 
        }

        virtual ErrorOr<size_t> pending_bytes() const override 
        { 
            return m_helper.pending_bytes(); 
        }

        /**
         * @param timeout 
         * @return ErrorOr<bool> 
         */
        virtual ErrorOr<bool> can_read_without_blocking(int timeout = 0) const override 
        { 
            return m_helper.can_read_without_blocking(timeout); 
        }

        /**
         * @brief Set the blocking object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_blocking(bool enabled) override 
        { 
            return m_helper.set_blocking(enabled); 
        }

        /**
         * @brief Set the close on exec object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_close_on_exec(bool enabled) override 
        { 
            return m_helper.set_close_on_exec(enabled); 
        }

        /**
         * @brief Set the notifications enabled object
         * 
         * @param enabled 
         */
        virtual void set_notifications_enabled(bool enabled) override
        {
            if (auto notifier = m_helper.notifier())
                notifier->set_enabled(enabled);
        }

        /**
         * @param flags 
         * @return ErrorOr<int> 
         */
        ErrorOr<int> receive_fd(int flags);

        /**
         * @param fd 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> send_fd(int fd);

        /**
         * @param ErrorOr<pid_t> 
         */
        ErrorOr<pid_t> peer_pid() const;

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read_without_waiting(Bytes buffer);

        ErrorOr<int> release_fd();

        virtual ~LocalSocket() { close(); }

    private:
        /**
         * @brief Construct a new Local Socket object
         * 
         */
        LocalSocket() = default;

        void setup_notifier()
        {
            VERIFY(is_open());

            m_helper.setup_notifier();
            m_helper.notifier()->on_ready_to_read = [this] {
                if (on_ready_to_read)
                    on_ready_to_read();
            };
        }

        PosixSocketHelper m_helper { Badge<LocalSocket> {} };
    }; // class LocalSocket final : public Socket 


    /**
     * @tparam T 
     */
    template<typename T>
    concept StreamLike = IsBaseOf<Stream, T>;
    template<typename T>
    concept SeekableStreamLike = IsBaseOf<SeekableStream, T>;
    template<typename T>
    concept SocketLike = IsBaseOf<Socket, T>;

    /**
     * @tparam T 
     */
    template<typename T>
    class BufferedHelper 
    {
        MODS_MAKE_NONCOPYABLE(BufferedHelper);

    public:
        /**
         * @brief Construct a new BufferedHelper object
         * 
         * @tparam U 
         * @param stream 
         * @param buffer 
         */
        template<StreamLike U>
        BufferedHelper(Badge<U>, NonnullOwnPtr<T> stream, ByteBuffer buffer)
            : m_stream(move(stream))
            , m_buffer(move(buffer))
        {
        }   

        /**
         * @brief Construct a new BufferedHelper object
         * 
         * @param other 
         */
        BufferedHelper(BufferedHelper&& other)
            : m_stream(move(other.m_stream))
            , m_buffer(move(other.m_buffer))
            , m_buffered_size(exchange(other.m_buffered_size, 0))
        {
        }

        /**
         * @param other 
         * @return BufferedHelper& 
         */
        BufferedHelper& operator=(BufferedHelper&& other)
        {
            m_stream = move(other.m_stream);
            m_buffer = move(other.m_buffer);
            m_buffered_size = exchange(other.m_buffered_size, 0);
            return *this;
        }

        /**
         * @brief Create a buffered object
         * 
         * @tparam BufferedType 
         * @param stream 
         * @param buffer_size 
         * @return ErrorOr<NonnullOwnPtr<BufferedType<T>>> 
         */
        template<template<typename> typename BufferedType>
        static ErrorOr<NonnullOwnPtr<BufferedType<T>>> create_buffered(NonnullOwnPtr<T> stream, size_t buffer_size)
        {
            if (!buffer_size)
                return Error::from_errno(EINVAL);
            if (!stream->is_open())
                return Error::from_errno(ENOTCONN);

            auto buffer = TRY(ByteBuffer::create_uninitialized(buffer_size));

            return adopt_nonnull_own_or_enomem(new BufferedType<T>(move(stream), move(buffer)));
        }

        /**
         * @return T& 
         */
        T& stream() 
        { 
            return *m_stream; 
        }

        /**
         * @return T const& 
         */
        T const& stream() const 
        { 
            return *m_stream; 
        }

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read(Bytes buffer)
        {
            if (!stream().is_open())
                return Error::from_errno(ENOTCONN);
            if (!buffer.size())
                return Error::from_errno(ENOBUFS);

            if (m_buffered_size == 0)
                TRY(populate_read_buffer());

            size_t buffer_nread = 0;

            if (m_buffered_size > 0) {
                size_t amount_to_take = min(buffer.size(), m_buffered_size);
                auto slice_to_take = m_buffer.span().slice(0, amount_to_take);
                auto slice_to_shift = m_buffer.span().slice(amount_to_take);

                slice_to_take.copy_to(buffer);
                buffer_nread += amount_to_take;

                if (amount_to_take < m_buffered_size) {
                    m_buffer.overwrite(0, slice_to_shift.data(), m_buffered_size - amount_to_take);
                }
                m_buffered_size -= amount_to_take;
            }

            return Bytes { buffer.data(), buffer_nread };
        }

        /**
         * @param buffer 
         * @return ErrorOr<StringView> 
         */
        ErrorOr<StringView> read_line(Bytes buffer)
        {
            return StringView { TRY(read_until(buffer, "\n"sv)) };
        }

        /**
         * @param buffer 
         * @param candidate 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read_until(Bytes buffer, StringView candidate)
        {
            return read_until_any_of(buffer, Array { candidate });
        }

        /**
         * @tparam N 
         * @param buffer 
         * @param candidates 
         * @return ErrorOr<Bytes> 
         */
        template<size_t N>
        ErrorOr<Bytes> read_until_any_of(Bytes buffer, Array<StringView, N> candidates)
        {
            if (!stream().is_open())
                return Error::from_errno(ENOTCONN);

            if (buffer.is_empty())
                return Error::from_errno(ENOBUFS);

            if (!TRY(can_read_line()))
                return Bytes {};

            if (stream().is_eof()) {
                if (buffer.size() < m_buffered_size) {
                    return Error::from_errno(EMSGSIZE);
                }
            }

            Optional<size_t> longest_match;
            size_t match_size = 0;
            for (auto& candidate : candidates) {
                auto result = Mods::memmem_optional(m_buffer.data(), m_buffered_size, candidate.bytes().data(), candidate.bytes().size());
                if (result.has_value()) {
                    auto previous_match = longest_match.value_or(*result);
                    if ((previous_match < *result) || (previous_match == *result && match_size < candidate.length())) {
                        longest_match = result;
                        match_size = candidate.length();
                    }
                }
            }
            if (longest_match.has_value()) {
                auto size_written_to_user_buffer = *longest_match;
                auto buffer_to_take = m_buffer.span().slice(0, size_written_to_user_buffer);
                auto buffer_to_shift = m_buffer.span().slice(size_written_to_user_buffer + match_size);

                buffer_to_take.copy_to(buffer);
                m_buffer.overwrite(0, buffer_to_shift.data(), buffer_to_shift.size());

                m_buffered_size -= size_written_to_user_buffer + match_size;

                return buffer.slice(0, size_written_to_user_buffer);
            }

            auto readable_size = min(m_buffered_size, buffer.size());
            auto buffer_to_take = m_buffer.span().slice(0, readable_size);
            auto buffer_to_shift = m_buffer.span().slice(readable_size);

            buffer_to_take.copy_to(buffer);
            m_buffer.overwrite(0, buffer_to_shift.data(), buffer_to_shift.size());

            m_buffered_size -= readable_size;

            return buffer.slice(0, readable_size);
        }

        /**
         * @return ErrorOr<bool> 
         */
        ErrorOr<bool> can_read_line()
        {
            if (stream().is_eof() && m_buffered_size > 0)
                return true;

            if (m_buffer.span().slice(0, m_buffered_size).contains_slow('\n'))
                return true;

            if (!stream().is_readable())
                return false;

            while (m_buffered_size < m_buffer.size()) {
                auto populated_slice = TRY(populate_read_buffer());

                if (stream().is_eof()) {
                    return m_buffered_size > 0;
                }

                if (populated_slice.contains_slow('\n'))
                    return true;

                if (populated_slice.is_empty())
                    break;
            }

            return false;
        }

        bool is_eof() const
        {
            if (m_buffered_size > 0) {
                return false;
            }

            return stream().is_eof();
        }

        size_t buffer_size() const
        {
            return m_buffer.size();
        }

        size_t buffered_data_size() const
        {
            return m_buffered_size;
        }

        void clear_buffer()
        {
            m_buffered_size = 0;
        }

    private:
        /**
         * @return ErrorOr<ReadonlyBytes> 
         */
        ErrorOr<ReadonlyBytes> populate_read_buffer()
        {
            if (m_buffered_size == m_buffer.size())
                return ReadonlyBytes {};

            auto fillable_slice = m_buffer.span().slice(m_buffered_size);
            size_t nread = 0;
            do {
                auto result = stream().read(fillable_slice);
                if (result.is_error()) {
                    if (!result.error().is_errno())
                        return result.error();
                    if (result.error().code() == EINTR)
                        continue;
                    if (result.error().code() == EAGAIN)
                        break;
                    return result.error();
                }
                auto read_size = result.value().size();
                m_buffered_size += read_size;
                nread += read_size;
                break;
            } while (true);
            return fillable_slice.slice(0, nread);
        }

        NonnullOwnPtr<T> m_stream;

        ByteBuffer m_buffer;
        size_t m_buffered_size { 0 };
    };

    /**
     * @tparam T 
     */
    template<SeekableStreamLike T>
    class BufferedSeekable final : public SeekableStream 
    {
        friend BufferedHelper<T>;

    public:
        /**
         * @param stream 
         * @param buffer_size 
         * @return ErrorOr<NonnullOwnPtr<BufferedSeekable<T>>> 
         */
        static ErrorOr<NonnullOwnPtr<BufferedSeekable<T>>> create(NonnullOwnPtr<T> stream, size_t buffer_size = 16384)
        {
            return BufferedHelper<T>::template create_buffered<BufferedSeekable>(move(stream), buffer_size);
        }

        /**
         * @brief Construct a new Buffered Seekable object
         * 
         * @param other 
         */
        BufferedSeekable(BufferedSeekable&& other) = default;

        /**
         * @param other 
         * @return BufferedSeekable& 
         */
        BufferedSeekable& operator=(BufferedSeekable&& other) = default;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const override 
        { 
            return m_helper.stream().is_readable(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes buffer) override 
        { 
            return m_helper.read(move(buffer)); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_writable() const override 
        { 
            return m_helper.stream().is_writable(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes buffer) override 
        { 
            return m_helper.stream().write(buffer); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_helper.is_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return m_helper.stream().is_open(); 
        }

        virtual void close() override 
        { 
            m_helper.stream().close(); 
        }

        /**
         * @param offset 
         * @param mode 
         * @return ErrorOr<off_t> 
         */
        virtual ErrorOr<off_t> seek(i64 offset, SeekMode mode) override
        {
            auto result = TRY(m_helper.stream().seek(offset, mode));
            m_helper.clear_buffer();
            return result;
        }

        /**
         * @param length 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> truncate(off_t length) override
        {
            return m_helper.stream().truncate(length);
        }

        /**
         * @param buffer 
         * @return ErrorOr<StringView> 
         */
        ErrorOr<StringView> read_line(Bytes buffer) 
        { 
            return m_helper.read_line(move(buffer)); 
        }

        /**
         * @param buffer 
         * @param candidate 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read_until(Bytes buffer, StringView candidate) 
        { 
            return m_helper.read_until(move(buffer), move(candidate)); 
        }

        /**
         * @tparam N 
         * @param buffer 
         * @param candidates 
         * @return ErrorOr<Bytes> 
         */
        template<size_t N>
        ErrorOr<Bytes> read_until_any_of(Bytes buffer, Array<StringView, N> candidates) { return m_helper.read_until_any_of(move(buffer), move(candidates)); }
        ErrorOr<bool> can_read_line() { return m_helper.can_read_line(); }

        size_t buffer_size() const { return m_helper.buffer_size(); }

        virtual ~BufferedSeekable() override = default;

    private:
        /**
         * @brief Construct a new BufferedSeekable object
         * 
         * @param stream 
         * @param buffer 
         */
        BufferedSeekable(NonnullOwnPtr<T> stream, ByteBuffer buffer)
            : m_helper(Badge<BufferedSeekable<T>> {}, move(stream), buffer)
        {
        }

        BufferedHelper<T> m_helper;
    }; // class BufferedSeekable final : public SeekableStream 

    class BufferedSocketBase : public Socket 
    {
    public:
        /**
         * @param buffer 
         * @return ErrorOr<StringView> 
         */
        virtual ErrorOr<StringView> read_line(Bytes buffer) = 0;

        /**
         * @param buffer 
         * @param candidate 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read_until(Bytes buffer, StringView candidate) = 0;

        /**
         * @return ErrorOr<bool> 
         */
        virtual ErrorOr<bool> can_read_line() = 0;

        /**
         * @return size_t 
         */
        virtual size_t buffer_size() const = 0;
    }; // class BufferedSocketBase : public Socket 

    /**
     * @tparam T 
     */
    template<SocketLike T>
    class BufferedSocket final : public BufferedSocketBase 
    {
        friend BufferedHelper<T>;

    public:
        /**
         * @param stream 
         * @param buffer_size 
         * @return ErrorOr<NonnullOwnPtr<BufferedSocket<T>>> 
         */
        static ErrorOr<NonnullOwnPtr<BufferedSocket<T>>> create(NonnullOwnPtr<T> stream, size_t buffer_size = 16384)
        {
            return BufferedHelper<T>::template create_buffered<BufferedSocket>(move(stream), buffer_size);
        }

        /**
         * @brief Construct a new Buffered Socket object
         * 
         * @param other 
         */
        BufferedSocket(BufferedSocket&& other)
            : BufferedSocketBase(static_cast<BufferedSocketBase&&>(other))
            , m_helper(move(other.m_helper))
        {
            setup_notifier();
        }

        /**
         * @param other 
         * @return BufferedSocket& 
         */
        BufferedSocket& operator=(BufferedSocket&& other)
        {
            Socket::operator=(static_cast<Socket&&>(other));
            m_helper = move(other.m_helper);

            setup_notifier();
            return *this;
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const override 
        { 
            return m_helper.stream().is_readable(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes buffer) override 
        { 
            return m_helper.read(move(buffer)); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_writable() const override 
        { 
            return m_helper.stream().is_writable(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes buffer) override 
        { 
            return m_helper.stream().write(buffer); 
        }

        /**

         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_helper.is_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return m_helper.stream().is_open(); 
        }

        virtual void close() override 
        { 
            m_helper.stream().close(); 
        }

        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> pending_bytes() const override
        {
            return TRY(m_helper.stream().pending_bytes()) + m_helper.buffered_data_size();
        }

        /**
         * @param timeout 
         * @return ErrorOr<bool> 
         */
        virtual ErrorOr<bool> can_read_without_blocking(int timeout = 0) const override 
        { 
            return m_helper.buffered_data_size() > 0 || TRY(m_helper.stream().can_read_without_blocking(timeout)); 
        }

        /**
         * @brief Set the blocking object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_blocking(bool enabled) override 
        { 
            return m_helper.stream().set_blocking(enabled); 
        }

        /**
         * @brief Set the close on exec object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_close_on_exec(bool enabled) override 
        { 
            return m_helper.stream().set_close_on_exec(enabled); 
        }

        /**
         * @brief Set the notifications enabled object
         * 
         * @param enabled 
         */
        virtual void set_notifications_enabled(bool enabled) override 
        { 
            m_helper.stream().set_notifications_enabled(enabled); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<StringView> 
         */
        virtual ErrorOr<StringView> read_line(Bytes buffer) override 
        { 
            return m_helper.read_line(move(buffer)); 
        }

        /**
         * @param buffer 
         * @param candidate 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read_until(Bytes buffer, StringView candidate) override 
        { 
            return m_helper.read_until(move(buffer), move(candidate)); 
        }

        /**
         * @tparam N 
         * @param buffer 
         * @param candidates 
         * @return ErrorOr<Bytes> 
         */
        template<size_t N>
        ErrorOr<Bytes> read_until_any_of(Bytes buffer, Array<StringView, N> candidates) { return m_helper.read_until_any_of(move(buffer), move(candidates)); }
        virtual ErrorOr<bool> can_read_line() override { return m_helper.can_read_line(); }
        
        /**
         * @return size_t 
         */
        virtual size_t buffer_size() const override 
        { 
            return m_helper.buffer_size(); 
        }

        /**
         * @brief Destroy the Buffered Socket object
         * 
         */
        virtual ~BufferedSocket() override = default;

    private:
        /**
         * @brief Construct a new BufferedSocket object
         * 
         * @param stream 
         * @param buffer 
         */
        BufferedSocket(NonnullOwnPtr<T> stream, ByteBuffer buffer)
            : m_helper(Badge<BufferedSocket<T>> {}, move(stream), buffer)
        {
            setup_notifier();
        }

        void setup_notifier()
        {
            m_helper.stream().on_ready_to_read = [this] {
                if (on_ready_to_read)
                    on_ready_to_read();
            };
        }

        BufferedHelper<T> m_helper;
    }; // class BufferedSocket final : public BufferedSocketBase 

    using BufferedFile = BufferedSeekable<File>;
    using BufferedTCPSocket = BufferedSocket<TCPSocket>;
    using BufferedUDPSocket = BufferedSocket<UDPSocket>;
    using BufferedLocalSocket = BufferedSocket<LocalSocket>;

    /**
     * @tparam T 
     */
    template<SocketLike T>
    class BasicReusableSocket final : public ReusableSocket 
    {
    public:
        /**
         * @param host 
         * @param port 
         * @return ErrorOr<NonnullOwnPtr<BasicReusableSocket<T>>> 
         */
        static ErrorOr<NonnullOwnPtr<BasicReusableSocket<T>>> connect(String const& host, u16 port)
        {
            return make<BasicReusableSocket<T>>(TRY(T::connect(host, port)));
        }

        /**
         * @param address 
         * @return ErrorOr<NonnullOwnPtr<BasicReusableSocket<T>>> 
         */
        static ErrorOr<NonnullOwnPtr<BasicReusableSocket<T>>> connect(SocketAddress const& address)
        {
            return make<BasicReusableSocket<T>>(TRY(T::connect(address)));
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_connected() override
        {
            return m_socket.is_open();
        }

        /**
         * @param host 
         * @param port 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> reconnect(String const& host, u16 port) override
        {
            if (is_connected())
                return Error::from_errno(EALREADY);

            m_socket = TRY(T::connect(host, port));
            return {};
        }

        /**
         * @param address 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> reconnect(SocketAddress const& address) override
        {
            if (is_connected())
                return Error::from_errno(EALREADY);

            m_socket = TRY(T::connect(address));
            return {};
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const override 
        { 
            return m_socket.is_readable(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes buffer) override 
        { 
            return m_socket.read(move(buffer)); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_writable() const override 
        { 
            return m_socket.is_writable(); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes buffer) override 
        { 
            return m_socket.write(buffer); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_socket.is_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return m_socket.is_open(); 
        }

        virtual void close() override 
        { 
            m_socket.close(); 
        }

        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> pending_bytes() const override 
        { 
            return m_socket.pending_bytes(); 
        }

        /**
         * @param timeout 
         * @return ErrorOr<bool> 
         */
        virtual ErrorOr<bool> can_read_without_blocking(int timeout = 0) const override 
        { 
            return m_socket.can_read_without_blocking(timeout); 
        }

        /**
         * @brief Set the blocking object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_blocking(bool enabled) override 
        { 
            return m_socket.set_blocking(enabled); 
        }

        /**
         * @brief Set the close on exec object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_close_on_exec(bool enabled) override 
        { 
            return m_socket.set_close_on_exec(enabled); 
        }

    private:
        /**
         * @brief Construct a new BasicReusableSocket object
         * 
         * @param socket 
         */
        BasicReusableSocket(NonnullOwnPtr<T> socket)
            : m_socket(move(socket))
        {
        }

        NonnullOwnPtr<T> m_socket;
    }; // class BasicReusableSocket final : public ReusableSocket 

    using ReusableTCPSocket = BasicReusableSocket<TCPSocket>;
    using ReusableUDPSocket = BasicReusableSocket<UDPSocket>;

} // namespace Mods
