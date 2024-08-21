/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief forward
 * @version 6.0
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

namespace Core 
{
    class ArgsParser;
    class ChildEvent;
    class ConfigFile;
    class CustomEvent;
    class DateTime;
    class DirIterator;
    class ElapsedTimer;
    class Event;
    class EventLoop;
    class File;
    class IODevice;
    class LocalServer;
    class LocalSocket;
    class MimeData;
    class NetworkJob;
    class NetworkResponse;
    class Notifier;
    class Object;
    class ProcessStatisticsReader;
    class Socket;
    class SocketAddress;
    class TCPServer;
    class TCPSocket;
    class Timer;
    class TimerEvent;
    class UDPServer;
    class UDPSocket;

    enum class TimerShouldFireWhenNotVisible;
} // namespace Core