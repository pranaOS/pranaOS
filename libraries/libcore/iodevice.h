/**
 * @file iodevice.h
 * @author Krisna Pranav
 * @brief io device
 * @version 6.0
 * @date 2023-09-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/enumbits.h>
#include <mods/forward.h>
#include <libcore/object.h>

namespace Core 
{

    class IODevice;

    class LineIterator 
    {
        MOD_MAKE_NONCOPYABLE(LineIterator);

    public:
        /**
         * @brief Construct a new Line Iterator object
         * 
         * @param is_end 
         */
        explicit LineIterator(IODevice&, bool is_end = false);

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(LineIterator const& other) const 
        { 
            return &other == this || (at_end() && other.is_end()) || (other.at_end() && is_end()); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_end() const { return m_is_end; }
        bool at_end() const;

        /**
         * @return LineIterator& 
         */
        LineIterator& operator++();

        /**
         * @return StringView 
         */
        StringView operator*() const { return m_buffer; }

    private:
        NonnullRefPtr<IODevice> m_device;
        bool m_is_end { false };
        String m_buffer;
    }; // class LineIterator

    class LineRange 
    {
    public:
        /**
         * @brief Construct a new LineRange object
         * 
         */
        LineRange() = delete;

        /**
         * @brief Construct a new LineRange object
         * 
         * @param device 
         */
        explicit LineRange(IODevice& device)
            : m_device(device)
        {
        }

        /**
         * @return LineIterator 
         */
        LineIterator begin();
        LineIterator end();

    private:
        IODevice& m_device;
    }; // class LineRange

    enum class OpenMode : unsigned 
    {
        NotOpen = 0,
        ReadOnly = 1,
        WriteOnly = 2,
        ReadWrite = 3,
        Append = 4,
        Truncate = 8,
        MustBeNew = 16,
        KeepOnExec = 32,
    }; // enum class OpenMode : unsigned 

    enum class SeekMode 
    {
        SetPosition,
        FromCurrentPosition,
        FromEndPosition,
    }; // enum class SeekMode 

    MOD_ENUM_BITWISE_OPERATORS(OpenMode)

    class IODevice : public Object 
    {
        C_OBJECT_ABSTRACT(IODevice)
    public:
        /**
         * @brief Destroy the IODevice object
         * 
         */
        virtual ~IODevice() override = default;

        /**
         * @return int 
         */
        int fd() const 
        { 
            return m_fd; 
        }

        /**
         * @return OpenMode 
         */
        OpenMode mode() const 
        { 
            return m_mode; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_open() const 
        { 
            return m_mode != OpenMode::NotOpen; 
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
        int error() const 
        { 
            return m_error; 
        }

        /**
         * @return char const* 
         */
        char const* error_string() const;

        /**
         * @return true 
         * @return false 
         */
        bool has_error() const 
        { 
            return m_error != 0; 
        }

        /**
         * @param buffer 
         * @param length 
         * @return int 
         */
        int read(u8* buffer, int length);

        /**
         * @param max_size 
         * @return ByteBuffer 
         */
        ByteBuffer read(size_t max_size);

        ByteBuffer read_all();

        /**
         * @param max_size 
         * @return String 
         */
        String read_line(size_t max_size = 16384);

        /**
         * @param size 
         * @return true 
         * @return false 
         */
        bool write(u8 const*, int size);

        /**
         * @return true 
         * @return false 
         */
        bool write(StringView);

        bool truncate(off_t);

        bool can_read_line() const;

        bool can_read() const;
        bool can_read_only_from_buffer() const 
        { 
            return !m_buffered_data.is_empty() && !can_read_from_fd(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool seek(i64, SeekMode = SeekMode::SetPosition, off_t* = nullptr);

        virtual bool open(OpenMode) = 0;
        virtual bool close();

        /**
         * @return LineIterator 
         */
        LineIterator line_begin() & { return LineIterator(*this); }
        LineIterator line_end() { return LineIterator(*this, true); }

        /**
         * @return LineRange 
         */
        LineRange lines()
        {
            return LineRange(*this);
        }

    protected:
        /**
         * @brief Construct a new IODevice object
         * 
         * @param parent 
         */
        explicit IODevice(Object* parent = nullptr);

        void set_fd(int);

        /**
         * @brief Set the mode object
         * 
         * @param mode 
         */
        void set_mode(OpenMode mode) 
        { 
            m_mode = mode; 
        }

        /**
         * @brief Set the error object
         * 
         * @param error 
         */
        void set_error(int error) const 
        { 
            m_error = error; 
        }

        /**
         * @brief Set the eof object
         * 
         * @param eof 
         */
        void set_eof(bool eof) const 
        { 
            m_eof = eof; 
        }

        virtual void did_update_fd(int) { }

    private:
        /**
         * @param size 
         * @return true 
         * @return false 
         */
        bool populate_read_buffer(size_t size = 1024) const;
        bool can_read_from_fd() const;

        int m_fd { -1 };
        OpenMode m_mode { OpenMode::NotOpen };
        mutable int m_error { 0 };
        mutable bool m_eof { false };
        mutable Vector<u8> m_buffered_data;
    }; // class IODevice : public Object
 
} // namespace Core
