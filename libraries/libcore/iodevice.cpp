/**
 * @file iodevice.cpp
 * @author Krisna Pranav
 * @brief iodevice
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytebuffer.h>
#include <libcore/iodevice.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

namespace Core 
{

    /**
     * @brief Construct a new IODevice::IODevice object
     * 
     * @param parent 
     */
    IODevice::IODevice(Object* parent)
        : Object(parent)
    {
    }

    /**
     * @return char const* 
     */
    char const* IODevice::error_string() const
    {
        return strerror(m_error);
    }

    /**
     * @param buffer 
     * @param length 
     * @return int 
     */
    int IODevice::read(u8* buffer, int length)
    {
        auto read_buffer = read(length);
        memcpy(buffer, read_buffer.data(), length);

        return read_buffer.size();
    }

    /**
     * @param max_size 
     * @return ByteBuffer 
     */
    ByteBuffer IODevice::read(size_t max_size)
    {
        if (m_fd < 0)
            return {};
            
        if (!max_size)
            return {};

        if (m_buffered_data.size() < max_size)
            populate_read_buffer(max(max_size - m_buffered_data.size(), 1024));

        auto size = min(max_size, m_buffered_data.size());
        auto buffer_result = ByteBuffer::create_uninitialized(size);

        if (buffer_result.is_error()) {
            dbgln("IODevice::read: Not enough memory to allocate a buffer of {} bytes", size);
            return {};
        }

        auto buffer = buffer_result.release_value();
        auto* buffer_ptr = (char*)buffer.data();

        memcpy(buffer_ptr, m_buffered_data.data(), size);
        m_buffered_data.remove(0, size);

        return buffer;
    }

    /**
     * @return true 
     * @return false 
     */
    bool IODevice::can_read_from_fd() const
    {
        fd_set rfds {};
        FD_ZERO(&rfds);
        FD_SET(m_fd, &rfds);

        struct timeval timeout {
            0, 0
        };

        for (;;) {
            if (select(m_fd + 1, &rfds, nullptr, nullptr, &timeout) < 0) {
                if (errno == EINTR)
                    continue;
                perror("IODevice::can_read_from_fd: select");
                return false;
            }
            break;
        }
        return FD_ISSET(m_fd, &rfds);
    }

    /**
     * @return true 
     * @return false 
     */
    bool IODevice::can_read_line() const
    {
        if (m_eof && !m_buffered_data.is_empty())
            return true;

        if (m_buffered_data.contains_slow('\n'))
            return true;

        if (!can_read_from_fd())
            return false;

        while (true) {
            auto previous_buffer_size = m_buffered_data.size();

            populate_read_buffer();

            auto new_buffer_size = m_buffered_data.size();

            if (m_error)
                return false;

            if (m_eof)
                return !m_buffered_data.is_empty();

            if (m_buffered_data.contains_in_range('\n', previous_buffer_size, new_buffer_size - 1))
                return true;
        }
    }

    /**
     * @return true 
     * @return false 
     */
    bool IODevice::can_read() const
    {
        return !m_buffered_data.is_empty() || can_read_from_fd();
    }

    /**
     * @return ByteBuffer 
     */
    ByteBuffer IODevice::read_all()
    {
        off_t file_size = 0;
        struct stat st;
        int rc = fstat(fd(), &st);

        if (rc == 0)
            file_size = st.st_size;

        Vector<u8> data;
        data.ensure_capacity(file_size);

        if (!m_buffered_data.is_empty()) {
            data.append(m_buffered_data.data(), m_buffered_data.size());
            m_buffered_data.clear();
        }

        while (true) {
            char read_buffer[4096];
            int nread = ::read(m_fd, read_buffer, sizeof(read_buffer));
            if (nread < 0) {
                set_error(errno);
                break;
            }
            if (nread == 0) {
                set_eof(true);
                break;
            }
            data.append((u8 const*)read_buffer, nread);
        }

        auto result = ByteBuffer::copy(data);
        if (!result.is_error())
            return result.release_value();

        set_error(ENOMEM);
        return {};
    }

    /**
     * @param max_size 
     * @return String 
     */
    String IODevice::read_line(size_t max_size)
    {
        if (m_fd < 0)
            return {};

        if (!max_size)
            return {};

        if (!can_read_line())
            return {};

        if (m_eof) {
            if (m_buffered_data.size() > max_size) {
                dbgln("IODevice::read_line: At EOF but there's more than max_size({}) buffered", max_size);
                return {};
            }
            
            auto line = String((char const*)m_buffered_data.data(), m_buffered_data.size(), Chomp);
            m_buffered_data.clear();

            return line;
        }

        auto line_result = ByteBuffer::create_uninitialized(max_size + 1);

        if (line_result.is_error()) {
            dbgln("IODevice::read_line: Not enough memory to allocate a buffer of {} bytes", max_size + 1);
            return {};
        }

        auto line = line_result.release_value();
        size_t line_index = 0;

        while (line_index < max_size) {
            u8 ch = m_buffered_data[line_index];
            line[line_index++] = ch;
            if (ch == '\n') {
                Vector<u8> new_buffered_data;
                new_buffered_data.append(m_buffered_data.data() + line_index, m_buffered_data.size() - line_index);
                m_buffered_data = move(new_buffered_data);
                line.resize(line_index);
                return String::copy(line, Chomp);
            }
        }
        return {};
    }

    /**
     * @param size 
     * @return true 
     * @return false 
     */
    bool IODevice::populate_read_buffer(size_t size) const
    {
        if (m_fd < 0)
            return false;

        if (!size)
            return false;

        auto buffer_result = ByteBuffer::create_uninitialized(size);

        if (buffer_result.is_error()) {
            dbgln("IODevice::populate_read_buffer: Not enough memory to allocate a buffer of {} bytes", size);
            return {};
        }

        auto buffer = buffer_result.release_value();
        auto* buffer_ptr = (char*)buffer.data();

        int nread = ::read(m_fd, buffer_ptr, size);

        if (nread < 0) {
            set_error(errno);
            return false;
        }

        if (nread == 0) {
            set_eof(true);
            return false;
        }

        m_buffered_data.append(buffer.data(), nread);

        return true;
    }

    /**
     * @return true 
     * @return false 
     */
    bool IODevice::close()
    {
        if (fd() < 0 || m_mode == OpenMode::NotOpen)
            return false;

        int rc = ::close(fd());

        if (rc < 0) {
            set_error(errno);
            return false;
        }

        set_fd(-1);
        set_mode(OpenMode::NotOpen);

        return true;
    }

    /**
     * @param offset 
     * @param mode 
     * @param pos 
     * @return true 
     * @return false 
     */
    bool IODevice::seek(i64 offset, SeekMode mode, off_t* pos)
    {
        int m = SEEK_SET;

        switch (mode) {
        case SeekMode::SetPosition:
            m = SEEK_SET;
            break;
        case SeekMode::FromCurrentPosition:
            m = SEEK_CUR;
            offset -= m_buffered_data.size();
            break;
        case SeekMode::FromEndPosition:
            m = SEEK_END;
            break;
        }

        off_t rc = lseek(m_fd, offset, m);

        if (rc < 0) {
            set_error(errno);
            if (pos)
                *pos = -1;
            return false;
        }

        m_buffered_data.clear();
        m_eof = false;

        if (pos)
            *pos = rc;

        return true;
    }

    /**
     * @param size 
     * @return true 
     * @return false 
     */
    bool IODevice::truncate(off_t size)
    {
        int rc = ftruncate(m_fd, size);
        
        if (rc < 0) {
            set_error(errno);
            return false;
        }

        return true;
    }

    /**
     * @param data 
     * @param size 
     * @return true 
     * @return false 
     */
    bool IODevice::write(u8 const* data, int size)
    {
        int rc = ::write(m_fd, data, size);

        if (rc < 0) {
            set_error(errno);
            perror("IODevice::write: write");
            return false;
        }

        return rc == size;
    }

    /**
     * @param fd 
     */
    void IODevice::set_fd(int fd)
    {
        if (m_fd == fd)
            return;

        m_fd = fd;

        did_update_fd(fd);
    }

    /**
     * @param v 
     * @return true 
     * @return false 
     */
    bool IODevice::write(StringView v)
    {
        return write((u8 const*)v.characters_without_null_termination(), v.length());
    }

    /**
     * @brief Construct a new LineIterator::LineIterator object
     * 
     * @param device 
     * @param is_end 
     */
    LineIterator::LineIterator(IODevice& device, bool is_end)
        : m_device(device)
        , m_is_end(is_end)
    {
        if (!m_is_end) {
            ++*this;
        }
    }

    /**
     * @return true 
     * @return false 
     */
    bool LineIterator::at_end() const
    {
        return m_device->eof();
    }   

    /**
     * @return LineIterator& 
     */
    LineIterator& LineIterator::operator++()
    {
        m_buffer = m_device->read_line();
        return *this;
    }

    /**
     * @return LineIterator 
     */
    LineIterator LineRange::begin() 
    { 
        return m_device.line_begin(); 
    }
    
    LineIterator LineRange::end() 
    { 
        return m_device.line_end(); 
    }

} // namespace Core
