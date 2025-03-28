/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief forward[kernel functionalities]
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/distinctnums.h>
#include <kernel/api/posix/sys/types.h>

namespace Kernel 
{

    class BlockDevice;
    class CharacterDevice;
    class Coredump;
    class Custody;
    class DevTmpFSDeviceInode;
    class DevTmpFSDirectoryInode;
    class DevTmpFSInode;
    class DevTmpFSPtsDirectoryInode;
    class DevTmpFSRootDirectoryInode;
    class Device;
    class DiskCache;
    class DoubleBuffer;
    class File;
    class OpenFileDescription;
    class FileSystem;
    class FutexQueue;
    class IPv4Socket;
    class Inode;
    class InodeIdentifier;
    class InodeWatcher;
    class KBuffer;
    class LocalSocket;
    class Mutex;
    class MasterPTY;
    class Mount;
    class PerformanceEventBuffer;
    class ProcFS;
    class ProcFSDirectoryInode;
    class ProcFSExposedComponent;
    class ProcFSExposedDirectory;
    class ProcFSInode;
    class ProcFSProcessInformation;
    class ProcFSRootDirectory;
    class ProcFSSystemBoolean;
    class ProcFSSystemDirectory;
    class Process;
    class ProcessGroup;
    class RecursiveSpinlock;
    class Scheduler;
    class Socket;
    class SysFS;
    class SysFSDirectory;
    class SysFSBusDirectory;
    class SysFSDirectoryInode;
    class SysFSInode;
    class TCPSocket;
    class TTY;
    class Thread;
    class ThreadTracer;
    class UDPSocket;
    class UserOrKernelBuffer;
    class VirtualFileSystem;
    class WaitQueue;
    class WorkQueue;

    namespace Memory 
    {
        class AddressSpace;
        class AnonymousVMObject;
        class InodeVMObject;
        class MappedROM;
        class MemoryManager;
        class PageDirectory;
        class PhysicalPage;
        class PhysicalRegion;
        class PrivateInodeVMObject;
        class Region;
        class SharedInodeVMObject;
        class VMObject;
        class VirtualRange;
        class VirtualRangeAllocator;
    } // namespace Memory

    class Spinlock;

    /**
     * @tparam LockType 
     */
    template<typename LockType>
    class SpinlockLocker;

    struct InodeMetadata;
    struct TrapFrame;

    TYPEDEF_DISTINCT_ORDERED_ID(pid_t, ProcessID);
    TYPEDEF_DISTINCT_ORDERED_ID(pid_t, ThreadID);
    TYPEDEF_DISTINCT_ORDERED_ID(pid_t, SessionID);
    TYPEDEF_DISTINCT_ORDERED_ID(pid_t, ProcessGroupID);

    TYPEDEF_DISTINCT_ORDERED_ID(uid_t, UserID);
    TYPEDEF_DISTINCT_ORDERED_ID(gid_t, GroupID);

} // namespace Kernel