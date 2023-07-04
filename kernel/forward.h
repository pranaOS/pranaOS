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
    class Dedvice;
    class DiskCache;
    class DoubleBuffer;
    class File;
    class FileDescription;
    class IPv4Socket;
    class Inode;
    class InodeIdentifier;
    class SharedInodeVMObject;
    class KBuffer;
    class LocalSocket;
    class Lock;
    class MappedROM;
    class MasterPTY;
    class Pagedirectory;
    class PerformEventBuffer;
    class PhysicalPage;
    class Process;
    class ProcessGroup;
    class Range;
    class ThreadTracer;
    class RangeAlocator;
    class Region;
    class Schedule;

    class Socket;

    template<typename T>
    class KResultOr;
}