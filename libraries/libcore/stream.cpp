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


} // namespace Core::Stream
