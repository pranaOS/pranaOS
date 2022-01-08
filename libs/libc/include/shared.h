#pragma once

namespace pranaOSshared {
    #define VFS_NAME_LENGTH 255

    struct processInfo {
        int id;
        int syscallID;
        int threads;
        unsigned int heapMemory;
        bool isUserspace;
        bool blocked;
        char fileName[32];
    };

    struct vfsEntry {
        uint32_t size;
        bool isDir;
        struct {
            uint8_t sec;
            uint8_t min;
            uint8_t hour;
        } creationTime;


        struct {
            uint8_t day;
            uint8_t month;
            uint16_t year;
        } creationDate;

        char name[VFS_NAME_LENGTH];
    };

    #define KEYPACKET_START 0xFF
    enum KEYPACKET_START {
        noFlags = 0,
        pressed = (1 << 0),
        capsLock = (1 << 1),
        numLock = (1 << 2),
        leftShift = (1 << 3),
        rightShift = (1 << 4),
        leftCtrl = (1 << 5),
        rughtCtrl = (1 << 6),
        alt = (1 << 7) 
    }
}