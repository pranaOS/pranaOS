/**
 * @file localserver.cpp
 * @author Krisna Pranav
 * @brief local server
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libcore/localserver.h>
#include <libcore/notifier.h>
#include <libcore/stream.h>
#include <libcore/system.h>
#include <libcore/systemservertakeover.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef SOCK_NONBLOCK
#    include <sys/ioctl.h>
#endif

namespace Core
{
    /**
     * @brief Construct a new LocalServer::LocalServer object
     * 
     * @param parent 
     */
    LocalServer::LocalServer(Object* parent)
        : Object(parent)
    {}  
} // namespace Core
