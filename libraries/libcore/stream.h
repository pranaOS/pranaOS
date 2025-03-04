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
        virtual bool is_readable() const 
        {
            return false;
        }

        virtual ErrorOr<Bytes> read(Bytes) = 0;

        virtual bool read_or_error(Bytes);

        virtual ErrorOr<size_t> write(ReadonlyBytes) = 0;

        virtual bool is_eof() const = 0;

        virtual bool is_open() const = 0;

        virtual void close() = 0;

        virtual ~Stream() {}

    protected:
        enum class SocketDomain 
        {
            Local,
            Inet,
        };

        enum class SocketType
        {
            Stream,
            Datagram,
        }; // enum class SocketType

        Socket() {}

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
        static ErrorOr<void> current_inet(int fd, SocketAddress const&);
    }; // class Stream


    enum class SeekMode
    {
        SetPosition,
        FromCurrentPosition,
        FromEndPosition
    }; // enum class SeekMode

    class ReusableSocket : public Socket 
    {
    public:
        virtual bool is_connected() = 0;

        /**
         * @param host 
         * @param port 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> reconnect(String const& host, u16 port) = 0;

        virtual ErrorOr<void> reconnect(SocketAddress const&) = 0;
    }; // class ReusableSocket : public Socket 

    enum class OpenMode : unsigned 
    {
        NotOpen = 0,
        Read = 1,
        Write = 2,
    }; // enum class OpenMode : unsigned 

    MOD_ENUM_BITWISE_OPERATIONS(OpenMode);

    class File final : public SeekableStream
    {
        
    }; // class File final : public SeekableStream
} // namespace Core::Stream
