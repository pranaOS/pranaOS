/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief forward[kernel functionalities]
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Kernel {

    class BlockDevice;
    class CharacterDevice;
    class CoreDump;
    class Custody;
    class Device;
    class DiskCache;
    class DoubleBuffer;
    class File;
    class FileDescription;
    class IPv4Socket;
    class Inode;
    class InodeIdentifier;
    class SharedInodeVMObject;
    class InodeWatcher;
    class KBuffer;
    class KResult;
    class LocalSocket;
    class Lock;
    class MappedROM;
    class MasterPTY;
    class PageDirectory;
    class PerformanceEventBuffer;
    class PhysicalPage;
    class PhysicalRegion;
    class Process;
    class ProcessGroup;
    class ThreadTracer;
    class Range;
    class RangeAllocator;
    class Region;
    class Scheduler;
    class SchedulerPerProcessorData;
    class SharedBuffer;
    class Socket;
    class RecursiveSpinLock;
    class TCPSocket;
    class TTY;
    class Thread;
    class UDPSocket;
    class UserOrKernelBuffer;
    class VFS;
    class VMObject;
    class WaitQueue;

    template<typename T>
    class KResultOr;

    template<typename LockType>
    class ScopedSpinLock;

    template<typename BaseType>
    class SpinLock;

}
