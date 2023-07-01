#if !defined(SQLITE_TEST) || SQLITE_OS_UNIX

#include "sqlite3.h"

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <memory>
#include <cstring>
#include <filesystem>
#include "free_rtos.h"
#include "task.h"
#include "config.h"
#include "utils.hpp"
#include "dirent.h"

#ifndef SQLITE_ECOPHONEVFS_BUFFERSZ
#define SQLITE_ECOPHONEVFS_BUFFERSZ 8192
#endif

#define MAXPATHNAME 512

#define UNUSED(x) ((void)(x))

typedef struct EcophoneFile EcophoneFile;
struct EcophoneFile {
    sqlite3_file base; 
    std::FILE *fd;     
    std::unique_ptr<char[]> streamBuffer;

    char *aBuffer;             
    int nBuffer;              
    sqlite3_int64 iBufferOfst; 
    long _pos  = -1;

    /**
     * @return std::size_t 
     */
    std::size_t size() {
        _pos = std::ftell(fd);
        std::fseek(fd, 0, SEEK_END);
        const auto size = std::ftell(fd);
        std::fseek(fd, _pos, SEEK_SET);
        return size;
    }   

    /**
     * @param off 
     * @param dir 
     * @return auto 
     */
    auto seek(long off, int dir) {
        int res{0};
        switch (dir) {
        case SEEK_SET:
            if (off == _pos)
                return 0;
            res  = std::fseek(fd, off, dir);
            _pos = (res) ? (-1) : (off);
            break;
        case SEEK_CUR:
            res = std::fseek(fd, off, dir);
            if (res)
                _pos = -1;
            else
                _pos += off;
            break;
        case SEEK_END:
            if ((_pos >= 0) && (_pos == size()) && !off)
                return 0;
            res = std::fseek(fd, off, dir);
            if (res)
                _pos = -1;
            else {
                _pos = std::ftell(fd);
            }
            break;
        }
        return res;
    }

    /**
     * @param buf 
     * @param size 
     * @return ssize_t 
     */
    ssize_t read(void *buf, size_t size) {
        auto s = std::fread(buf, 1, size, fd);
        if (std::ferror(fd)) {
            _pos = -1;
            return -1;
        }
        _pos += s;
        return s;
    }

    /**
     * @param buf 
     * @param size 
     * @return ssize_t 
     */
    ssize_t write(const void *buf, size_t size) {
        auto s = std::fwrite(buf, 1, size, fd);
        if (std::ferror(fd)) {
            _pos  = -1;
            return -1;
        }
        _pos += s;
        return s;
    }

    /**
     * @param off 
     * @return auto 
     */
    auto seekOrEnd(long off) {
        if (!off || off < size()) {
            if (seek(off, SEEK_SET) != 0)
                return SQLITE_IOERR_WRITE;
            return SQLITE_OK;
        }

        if (seek(0, SEEK_END) != 0) {
            return SQLITE_IOERR_READ;
        }

        return SQLITE_OK;
    }

    /**
     * @param off 
     * @return auto 
     */
    auto seekOrAppend(long off) {
        if (!off || off < size()) {
            if (seek(off, SEEK_SET) != 0)
                return SQLITE_IOERR_WRITE;
            return SQLITE_OK;
        }
        else {
            if (seek(0, SEEK_END) != 0) {
                return SQLITE_IOERR_WRITE;
            }

            const auto currentSize = size();
            if (currentSize > off) {
                return SQLITE_IOERR_SEEK;
            }

            auto bytesLeft = off - currentSize;
            auto zero_buf  = std::make_unique<char[]>(bytesLeft);
            auto ret       = std::fwrite(zero_buf.get(), 1, bytesLeft, fd);
            if (ret != bytesLeft) {
                return SQLITE_IOERR_WRITE;
            }
        }
        return SQLITE_OK;
    }
};

/**
 * @param p 
 * @param zBuf 
 * @param iAmt 
 * @param iOfst 
 * @return int 
 */
static int ecophoneDirectWrite(EcophoneFile *p, const void *zBuf, int iAmt, sqlite_int64 iOfst ) {
    ssize_t nWrite;

    const auto result = p->seekOrAppend(iOfst);
    if (result != SQLITE_OK) {
        return result;
    }

    nWrite = p->write(zBuf, iAmt);

    if (nWrite != iAmt) {
        return SQLITE_IOERR_WRITE;
    }
    if (std::fflush(p->fd) != 0) {
        return SQLITE_IOERR_WRITE;
    }
    return SQLITE_OK;
}

/**
 * @param p 
 * @return int 
 */
static int ecophoneFlushBuffer(EcophoneFile *p) {
    int rc = SQLITE_OK;
    if (p->nBuffer) {
        rc         = ecophoneDirectWrite(p, p->aBuffer, p->nBuffer, p->iBufferOfst);
        p->nBuffer = 0;
    }
    return rc;
}

/**
 * @param pFile 
 * @return int 
 */
static int ecophoneClose(sqlite3_file *pFile) {
    int rc;
    EcophoneFile *p = (EcophoneFile *)pFile;
    rc              = ecophoneFlushBuffer(p);
    sqlite3_free(p->aBuffer);
    p->streamBuffer.reset();

    std::fclose(p->fd);
    return rc;
}

/**
 * @param pFile 
 * @param zBuf 
 * @param iAmt 
 * @param iOfst 
 * @return int 
 */
static int ecophoneRead(sqlite3_file *pFile, void *zBuf, int iAmt, sqlite_int64 iOfst) {
    ssize_t nRead;

    EcophoneFile *p = (EcophoneFile *)pFile;

    auto rc = ecophoneFlushBuffer(p);
    if (rc != SQLITE_OK) {
        return rc;
    }
    p->seekOrEnd(iOfst);

    nRead = p->read(zBuf, iAmt);

    if (nRead == iAmt) {
        return SQLITE_OK;
    }
    else if (nRead >= 0) {
        return SQLITE_IOERR_SHORT_READ;
    }

    return SQLITE_IOERR_READ;
}

/**
 * @param pFile 
 * @param zBuf 
 * @param iAmt 
 * @param iOfst 
 * @return int 
 */
static int ecophoneWrite(sqlite3_file *pFile, const void *zBuf, int iAmt, sqlite_int64 iOfst) {
    EcophoneFile *p = (EcophoneFile *)pFile;

    if (p->aBuffer) {
        char *z         = (char *)zBuf; 
        int n           = iAmt;         
        sqlite3_int64 i = iOfst;        

        while (n > 0) {
            int nCopy;
            if (p->nBuffer == SQLITE_ECOPHONEVFS_BUFFERSZ || p->iBufferOfst + p->nBuffer != i) {
                int rc = ecophoneFlushBuffer(p);
                if (rc != SQLITE_OK) {
                    return rc;
                }
            }
            assert(p->nBuffer == 0 || p->iBufferOfst + p->nBuffer == i);
            p->iBufferOfst = i - p->nBuffer;
            nCopy = SQLITE_ECOPHONEVFS_BUFFERSZ - p->nBuffer;
            if (nCopy > n) {
                nCopy = n;
            }
            memcpy(&p->aBuffer[p->nBuffer], z, nCopy);
            p->nBuffer += nCopy;

            n -= nCopy;
            i += nCopy;
            z += nCopy;
        }
    }
    else {
        return ecophoneDirectWrite(p, zBuf, iAmt, iOfst);
    }

    return SQLITE_OK;
}

/**
 * @param pFile 
 * @param size 
 * @return int 
 */
static int ecophoneTruncate(sqlite3_file *pFile, sqlite_int64 size) {
#if 0
    if( ftruncate(((EcophoneFile *)pFile)->fd, size) ) return SQLITE_IOERR_TRUNCATE;
#else
    UNUSED(pFile);
    UNUSED(size);
#endif
    return SQLITE_IOERR_TRUNCATE;
}

/**
 * @param pFile 
 * @param flags 
 * @return int 
 */
static int ecophoneSync(sqlite3_file *pFile, int flags) {
    EcophoneFile *p = (EcophoneFile *)pFile;
    int rc;

    UNUSED(flags);

    rc = ecophoneFlushBuffer(p);
    if (rc != SQLITE_OK) {
        return rc;
    }
    rc = fileno(p->fd);
    if (rc > 0) {
        rc = fsync(rc);
    }
    return (rc == 0 ? SQLITE_OK : SQLITE_IOERR_FSYNC);
}

/**
 * @param pFile 
 * @param pSize 
 * @return int 
 */
static int ecophoneFileSize(sqlite3_file *pFile, sqlite_int64 *pSize) {
    EcophoneFile *p = (EcophoneFile *)pFile;
    int rc; 

    rc = ecophoneFlushBuffer(p);
    if (rc != SQLITE_OK) {
        return rc;
    }

    *pSize = p->size();

    return SQLITE_OK;
}

/**
 * @param pFile 
 * @param eLock 
 * @return int 
 */
static int ecophoneLock(sqlite3_file *pFile, int eLock) {
    UNUSED(pFile);
    UNUSED(eLock);
    return SQLITE_OK;
}

/**
 * @param pFile 
 * @param eLock 
 * @return int 
 */
static int ecophoneUnlock(sqlite3_file *pFile, int eLock) {
    UNUSED(pFile);
    UNUSED(eLock);
    return SQLITE_OK;
}

/**
 * @param pFile 
 * @param pResOut 
 * @return int 
 */
static int ecophoneCheckReservedLock(sqlite3_file *pFile, int *pResOut) {
    UNUSED(pFile);
    *pResOut = 0;
    return SQLITE_OK;
}

/**
 * @param pFile 
 * @param op 
 * @param pArg 
 * @return int 
 */
static int ecophoneFileControl(sqlite3_file *pFile, int op, void *pArg) {
    UNUSED(pFile);
    UNUSED(pArg);
    return SQLITE_NOTFOUND;
}

/**
 * @param pFile 
 * @return int 
 */
static int ecophoneSectorSize(sqlite3_file *pFile) {
    UNUSED(pFile);
    static constexpr auto SECTOR_SIZE = 512;
    return SECTOR_SIZE;
}

/**
 * @param pFile 
 * @return int 
 */
static int ecophoneDeviceCharacteristics(sqlite3_file *pFile) {
    UNUSED(pFile);
    return SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN;
}

/**
 * @param pVfs 
 * @param zPath 
 * @param flags 
 * @param pResOut 
 * @return int 
 */
static int ecophoneAccess(sqlite3_vfs *pVfs, const char *zPath, int flags, int *pResOut) {
    std::FILE *fd;
    UNUSED(pVfs);

    assert(flags == SQLITE_ACCESS_EXISTS || flags == SQLITE_ACCESS_READ || flags == SQLITE_ACCESS_READWRITE );

    fd = std::fopen(zPath, "r");
    if (fd != NULL) {
        if (pResOut)
            *pResOut = flags;
        std::fclose(fd);
    }
    else if (pResOut)
        *pResOut = 0;
    return SQLITE_OK;
}

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

/**
 * @param pVfs 
 * @param zName 
 * @param pFile 
 * @param flags 
 * @param pOutFlags 
 * @return int 
 */
static int ecophoneOpen(sqlite3_vfs *pVfs,const char *zName, sqlite3_file *pFile, int flags, int *pOutFlags) {
    UNUSED(pVfs);

    static const sqlite3_io_methods ecophoneio = {
        1,                            
        ecophoneClose,                
        ecophoneRead,                 
        ecophoneWrite,                
        ecophoneTruncate,             
        ecophoneSync,                 
        ecophoneFileSize,             
        ecophoneLock,                 
        ecophoneUnlock,               
        ecophoneCheckReservedLock,    
        ecophoneFileControl,          
        ecophoneSectorSize,           
        ecophoneDeviceCharacteristics 
    };

    EcophoneFile *p = (EcophoneFile *)pFile; 
    char *aBuf      = 0;

    if (zName == 0) {
        return SQLITE_IOERR;
    }

    if (flags & SQLITE_OPEN_MAIN_JOURNAL) {
        aBuf = (char *)sqlite3_malloc(SQLITE_ECOPHONEVFS_BUFFERSZ);
        if (!aBuf) {
            return SQLITE_NOMEM;
        }
    }

    memset(p, 0, sizeof(EcophoneFile));
    p->_pos = -1;

    std::string oflags;
    if (flags & SQLITE_OPEN_READONLY) {
        oflags = "r";
    }
    else if ((flags & SQLITE_OPEN_READWRITE) && (flags & SQLITE_OPEN_CREATE)) {
        p->fd = std::fopen(zName, "r");
        if (p->fd == nullptr) {
            oflags = "w+";
        }
        else {
            std::fclose(p->fd);
            oflags = "r+";
        }
    }
    else {
        oflags = "r+";
    }

    p->fd = std::fopen(zName, oflags.c_str());
    if (p->fd == nullptr) {
        sqlite3_free(aBuf);
        return SQLITE_CANTOPEN;
    }

    constexpr size_t streamBufferSize = 16 * 1024;
    p->streamBuffer                   = std::make_unique<char[]>(streamBufferSize);
    setvbuf(p->fd, p->streamBuffer.get(), _IOFBF, streamBufferSize);
    p->aBuffer = aBuf;

    if (pOutFlags) {
        *pOutFlags = flags;
    }
    p->base.pMethods = &ecophoneio;
    return SQLITE_OK;
}

/**
 * @param pVfs 
 * @param zPath 
 * @param dirSync 
 * @return int 
 */
static int ecophoneDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync) {
    UNUSED(pVfs);
    int rc = std::filesystem::remove_all(zPath); /* Return code */

    if (rc != 0)
        return SQLITE_OK;

    if (rc == 0 && dirSync) {
        DIR *dfd;                   
        int i;                      
        char zDir[MAXPATHNAME + 1]; 

        sqlite3_snprintf(MAXPATHNAME, zDir, "%q", zPath);
        zDir[MAXPATHNAME] = '\0';
        for (i = strlen(zDir); i > 1 && zDir[i] != '/'; i++)
            ;
        zDir[i] = '\0';

        dfd = opendir(zDir);
        if (dfd == NULL) {
            rc = -1;
        }
        else {
            rc = SQLITE_OK;
            closedir(dfd);
        }
    }
    return (rc == 0 ? SQLITE_OK : SQLITE_IOERR_DELETE);
}

/**
 * @param pVfs 
 * @param zPath 
 * @param nPathOut 
 * @param zPathOut 
 * @return int 
 */
static int ecophoneFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nPathOut, char *zPathOut) {
    UNUSED(pVfs);
    sqlite3_snprintf(nPathOut, zPathOut, "%s", zPath);
    zPathOut[nPathOut - 1] = '\0';
    return SQLITE_OK;
}

/**
 * @param pVfs 
 * @param zPath 
 * @return void* 
 */
static void *ecophoneDlOpen(sqlite3_vfs *pVfs, const char *zPath) {
    UNUSED(pVfs);
    UNUSED(zPath);
    return nullptr;
}

/**
 * @param pVfs 
 * @param nByte 
 * @param zErrMsg 
 */
static void ecophoneDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg) {
    UNUSED(pVfs);
    sqlite3_snprintf(nByte, zErrMsg, "Loadable extensions are not supported");
    zErrMsg[nByte - 1] = '\0';
}

/**
 * @param pVfs 
 * @param pH 
 * @param z 
 */
static void (*ecophoneDlSym(sqlite3_vfs *pVfs, void *pH, const char *z))(void) {
    UNUSED(pVfs);
    UNUSED(pH);
    UNUSED(z);
    return nullptr;
}

/**
 * @param pVfs 
 * @param pHandle 
 */
static void ecophoneDlClose(sqlite3_vfs *pVfs, void *pHandle) {
    UNUSED(pVfs);
    UNUSED(pHandle);
}

/**
 * @param pVfs 
 * @param nByte 
 * @param zByte 
 * @return int 
 */
static int ecophoneRandomness(sqlite3_vfs *pVfs, int nByte, char *zByte) {
    return SQLITE_PERM;
}

/**
 * @param pVfs 
 * @param nMicro 
 * @return int 
 */
static int ecophoneSleep(sqlite3_vfs *pVfs, int nMicro) {
    UNUSED(pVfs);

    const TickType_t xDelay = nMicro / 1000 / portTICK_PERIOD_MS;
    vTaskDelay(xDelay);

    return nMicro;
}

struct julian_clock {
    using rep        = double;
    using period     = std::ratio<86400>; 
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<julian_clock>;

    static constexpr bool is_steady = false;

    /**
     * @return constexpr auto 
     */
    static constexpr auto jdiff() {
        using namespace std::chrono_literals;
        return 58574100h;
    }

    /**
     * @return time_point 
     */
    static time_point now() noexcept {
        using namespace std::chrono;
        return time_point{duration{system_clock::now().time_since_epoch()} + jdiff()};
    }
};

/**
 * @param pVfs 
 * @param pTime 
 * @return int 
 */
static int ecophoneCurrentTime(sqlite3_vfs *pVfs, double *pTime) {
    *pTime = julian_clock::now().time_since_epoch().count();
    return SQLITE_OK;
}

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

/**
 * @return sqlite3_vfs* 
 */
sqlite3_vfs *sqlite3_ecophonevfs(void) {
    static sqlite3_vfs ecophonevfs = {
        1,                   
        sizeof(EcophoneFile),
        MAXPATHNAME,         
        0,                   
        "ecophone",          
        0,                   
        ecophoneOpen,        
        ecophoneDelete,      
        ecophoneAccess,      
        ecophoneFullPathname,
        ecophoneDlOpen,      
        ecophoneDlError,     
        ecophoneDlSym,       
        ecophoneDlClose,     
        ecophoneRandomness,  
        ecophoneSleep,       
        ecophoneCurrentTime, 
    };
    return &ecophonevfs;
}

#endif
