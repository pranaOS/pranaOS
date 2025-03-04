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
        return const_cast<SeekableStream*>(this)->seek(0);
    }
} // namespace Core::Stream
