/**
 * @file stdio_file_implementation.h
 * @author Krisna Pranav
 * @brief stdio file implementation
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/array.h>
#include <mods/intrusivelist.h>
#include <mods/types.h>
#include <libc/bits/FILE.h>
#include <libc/bits/pthread_integration.h>
#include <libc/bits/wchar.h>
#include <sys/types.h>

#pragma once

struct FILE {
public:
    /**
     * @brief Construct a new FILE object
     * 
     * @param fd 
     * @param mode 
     */
    FILE(int fd, int mode)
        : m_fd(fd)
        , m_mode(mode)
    {
        pthread_mutexattr_t attr = { __PTHREAD_MUTEX_RECURSIVE };
        __pthread_mutex_init(&m_mutex, &attr);
    }

    /**
     * @brief Destroy the FILE object
     * 
     */
    ~FILE();

    /**
     * @param fd 
     * @param mode 
     * @return FILE* 
     */
    static FILE* create(int fd, int mode);

    /**
     * @param data 
     * @param mode 
     * @param size 
     */
    void setbuf(u8* data, int mode, size_t size) { m_buffer.setbuf(data, mode, size); }

    bool flush();
    void purge();
    size_t pending();
    bool close();

    void lock();
    void unlock();

    /**
     * @return int 
     */
    int fileno() const 
    { 
        return m_fd; 
    }

    /**
     * @return true 
     * @return false 
     */
    bool eof() const 
    { 
        return m_eof; 
    }

    /**
     * @return int 
     */
    int mode() const 
    { 
        return m_mode; 
    }

    /**
     * @return u8 
     */
    u8 flags() const 
    { 
        return m_flags; 
    }

    /**
     * @return int 
     */
    int error() const 
    { 
        return m_error; 
    }

    void clear_err() 
    { 
        m_error = 0; 
    }

    void set_err() 
    { 
        m_error = 1; 
    }

    /**
     * @return size_t 
     */
    size_t read(u8*, size_t);

    /**
     * @return size_t 
     */
    size_t write(u8 const*, size_t);

    /**
     * @tparam CharType 
     * @return true 
     * @return false 
     */
    template<typename CharType>
    bool gets(CharType*, size_t);

    /**
     * @param byte 
     * @return true 
     * @return false 
     */
    bool ungetc(u8 byte) 
    { 
        return m_buffer.enqueue_front(byte); 
    }

    /**
     * @param offset 
     * @param whence 
     * @return int 
     */
    int seek(off_t offset, int whence);

    /**
     * @return off_t 
     */
    off_t tell();
    
    /**
     * @return pid_t 
     */
    pid_t popen_child() 
    { 
        return m_popen_child; 
    }

    /**
     * @brief Set the popen child object
     * 
     * @param child_pid 
     */
    void set_popen_child(pid_t child_pid) 
    {
        m_popen_child = child_pid; 
    }

    /**
     * @param fd 
     * @param mode 
     */
    void reopen(int fd, int mode);

    /**
     * @param available_size 
     * @return u8 const* 
     */
    u8 const* readptr(size_t& available_size);

    /**
     * @param increment 
     */
    void readptr_increase(size_t increment);

    enum Flags : u8 
    {
        None = 0,
        LastRead = 1,
        LastWrite = 2,
    }; // enum Flags : u8 

private:
    struct Buffer {
    public:
        /**
         * @brief Destroy the Buffer object
         * 
         */
        ~Buffer();

        /**
         * @return int 
         */
        int mode() const 
        { 
            return m_mode; 
        }

        /**
         * @param data 
         * @param mode 
         * @param size 
         */
        void setbuf(u8* data, int mode, size_t size);
        
        /**
         * @param fd 
         */
        void realize(int fd);
        void drop();

        bool may_use() const;

        bool is_not_empty() const 
        { 
            return m_ungotten || !m_empty; 
        }

        /**
         * @return size_t 
         */
        size_t buffered_size() const;

        /**
         * @param available_size 
         * @return u8 const* 
         */
        u8 const* begin_dequeue(size_t& available_size) const;

        /**
         * @param actual_size 
         */
        void did_dequeue(size_t actual_size);

        /**
         * @param available_size 
         * @return u8* 
         */
        u8* begin_enqueue(size_t& available_size) const;

        /**
         * @param actual_size 
         */
        void did_enqueue(size_t actual_size);

        /**
         * @param byte 
         * @return true 
         * @return false 
         */
        bool enqueue_front(u8 byte);

    private:
        constexpr static auto unget_buffer_size = MB_CUR_MAX;
        constexpr static u32 ungotten_mask = ((u32)0xffffffff) >> (sizeof(u32) * 8 - unget_buffer_size);

        u8* m_data { nullptr };
        size_t m_capacity { BUFSIZ };
        size_t m_begin { 0 };
        size_t m_end { 0 };

        int m_mode { -1 };
        Array<u8, unget_buffer_size> m_unget_buffer { 0 };
        u32 m_ungotten : unget_buffer_size { 0 };
        bool m_data_is_malloced : 1 { false };

        bool m_empty : 1 { true };
    }; // struct Buffer

    /**
     * @return ssize_t 
     */
    ssize_t do_read(u8*, size_t);

    /**
     * @return ssize_t 
     */
    ssize_t do_write(u8 const*, size_t);

    bool read_into_buffer();
    
    bool write_from_buffer();

    int m_fd { -1 };
    int m_mode { 0 };
    u8 m_flags { Flags::None };
    int m_error { 0 };
    bool m_eof { false };
    pid_t m_popen_child { -1 };
    Buffer m_buffer;
    __pthread_mutex_t m_mutex;
    IntrusiveListNode<FILE> m_list_node;

public:
    using List = IntrusiveList<&FILE::m_list_node>;
}; // struct File

class ScopedFileLock 
{
public:
    /**
     * @brief Construct a new ScopedFileLock object
     * 
     * @param file 
     */
    ScopedFileLock(FILE* file)
        : m_file(file)
    {
        m_file->lock();
    }

    /**
     * @brief Destroy the ScopedFileLock object
     * 
     */
    ~ScopedFileLock()
    {
        m_file->unlock();
    }

private:
    FILE* m_file;
}; // class ScopedFileLock
