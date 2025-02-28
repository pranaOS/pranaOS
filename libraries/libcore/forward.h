/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief forward
 * @version 6.0
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Core 
{

    class AnonymousBuffer;
    class ArgsParser;
    class ChildEvent;
    class ConfigFile;
    class CustomEvent;
    class DateTime;
    class DirIterator;
    class DeferredInvocationContext;
    class ElapsedTimer;
    class Event;
    class EventLoop;
    class File;
    class IODevice;
    class LocalServer;
    class MimeData;
    class NetworkJob;
    class NetworkResponse;
    class Notifier;
    class Object;
    class ObjectClassRegistration;
    class ProcessStatisticsReader;
    class SocketAddress;
    class TCPServer;
    class Timer;
    class TimerEvent;
    class UDPServer;

    enum class TimerShouldFireWhenNotVisible;

    namespace Stream 
    {
        class Socket;
        class Stream;
        class BufferedSocketBase;
    } // namespace Stream

} // namespace Core
