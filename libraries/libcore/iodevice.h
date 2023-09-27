/**
 * @file iodevice.h
 * @author Krisna Pranav
 * @brief io device
 * @version 6.0
 * @date 2023-09-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/forward.h>
#include <libcore/object.h>

namespace Core
{
    class IODevice : public Object
    {
        C_OBJECT_ABSTRACT(IODevice)

    public:
        enum OpenMode
        {
            NotOpen = 0,
            ReadOnly = 1,
            WriteOnly = 2,
            ReadWrite = 3,
            Append = 4,
            Truncate = 8,
            MustBeNew = 16,
        }; // enum OpenMode

        enum class SeekMode
        {
            SetPosition,
            FromCurrentPosition,
            FromEndPosition,
        }; // enum class SeekMode

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
        bool write(const u8*, int size);

        bool write(const StringView&);

        /**
         * @param ... 
         * @return int 
         */
        int printf(const char*, ...);
    
    protected:
        /**
         * @param parent 
         */
        explicit IODevice(Object* parent = nullptr);

        void set_fd(int);

        /**
         * @param mode 
         */
        void set_mode(OpenMode mode)
        {
            m_mode = mode;
        }

        /**
         * @param error 
         */
        void set_error(int error) const
        {
            m_error = error;
        }

        /**
         * @param eof 
         */
        void set_eof(bool eof) const
        {
            m_eof = eof;
        }

        virtual void did_update_fd(int) {}

    private:
        bool populate_read_buffer() const;
        bool can_read_from_fd() const;

        int m_fd { -1 };

        OpenMode m_mode { NotOpen };

        mutable int m_error { 0 };
        mutable bool m_eof { false };
        mutable Vector<u8> m_buffered_data;
    }; // class IODevice
} // namespace Core