/**
 * @file select.cpp
 * @author Krisna Pranav
 * @brief select
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <syscall.h>
#include <mods/vector.h>

extern "C" {

    /**
     * @param nfds 
     * @param readfds 
     * @param writefds 
     * @param exceptfds 
     * @param timeout_tv 
     * @return int 
     */
    int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout_tv)
    {
        timespec* timeout_ts = nullptr;
        timespec timeout;

        if (timeout_tv) {
            timeout_ts = &timeout;
            TIMEVAL_TO_TIMESPEC(timeout_tv, timeout_ts);
        }
        
        return pselect(nfds, readfds, writefds, exceptfds, timeout_ts, nullptr);
    }

    /**
     * @param nfds 
     * @param readfds 
     * @param writefds 
     * @param exceptfds 
     * @param timeout 
     * @param sigmask 
     * @return int 
     */
    int pselect(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timespec const* timeout, sigset_t const* sigmask)
    {
        Vector<pollfd, FD_SETSIZE> fds;

        if (nfds < 0 || static_cast<size_t>(nfds) >= fds.capacity())
            return EINVAL;

        for (int fd = 0; fd < nfds; fd++) {
            short events = 0;
            if (readfds && FD_ISSET(fd, readfds))
                events |= POLLIN;
            if (writefds && FD_ISSET(fd, writefds))
                events |= POLLOUT;
            if (exceptfds && FD_ISSET(fd, exceptfds))
                events |= POLLPRI;
            if (!events)
                continue;

            fds.unchecked_append({ fd, events, 0 });
        }

        if (ppoll(fds.data(), fds.size(), timeout, sigmask) < 0)
            return -1;

        if (readfds)
            FD_ZERO(readfds);
        if (writefds)
            FD_ZERO(writefds);
        if (exceptfds)
            FD_ZERO(exceptfds);

        int marked_fd_count = 0;
        for (auto& fd_entry : fds) {
            if (fd_entry.revents == 0)
                continue;
            if (readfds && (fd_entry.revents & POLLIN)) {
                FD_SET(fd_entry.fd, readfds);
                marked_fd_count++;
            }
            if (writefds && (fd_entry.revents & POLLOUT)) {
                FD_SET(fd_entry.fd, writefds);
                marked_fd_count++;
            }
            if (exceptfds && (fd_entry.revents & (POLLPRI | POLLERR | POLLHUP))) {
                FD_SET(fd_entry.fd, exceptfds);
                marked_fd_count++;
            }
        }

        return marked_fd_count;
    }
}
