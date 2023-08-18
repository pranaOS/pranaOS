/**
 * @file fifo.h
 * @author Krisna Pranav
 * @brief fifo
 * @version 6.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/doublebuffer.h>
#include <kernel/lock.h>
#include <kernel/unixtypes.h>
#include <kernel/waitqueue.h> 
#include <kernel/filesystem/file.h>

namespace Kernel 
{

    class FileDescription;

    class FIFO final : public File 
    {
    public:
        enum class Direction : u8 
        {
            Neither,
            Reader,
            Writer
        }; // enum 

        /**
         * @return NonnullRefPtr<FIFO> 
         */
        static NonnullRefPtr<FIFO> create(uid_t);

        /// @brief Destroy the FIFO object
        virtual ~FIFO() override;

        /**
         * @return uid_t 
         */
        uid_t uid() const 
        { 
            return m_uid; 
        }

        /**
         * @return NonnullRefPtr<FileDescription> 
         */
        NonnullRefPtr<FileDescription> open_direction(Direction);
        NonnullRefPtr<FileDescription> open_direction_blocking(Direction);

        /// @brief: attach + detach
        void attach(Direction);
        void detach(Direction);

    private:
        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override;
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override;

        /**
         * @return KResult 
         */
        virtual KResult stat(::stat&) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;
        virtual bool can_write(const FileDescription&, size_t) const override;

        /**
         * @return String 
         */
        virtual String absolute_path(const FileDescription&) const override;

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "FIFO"; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_fifo() const override 
        { 
            return true; 
        }

        /// @brief Construct a new FIFO object
        explicit FIFO(uid_t);

        unsigned m_writers { 0 };
        unsigned m_readers { 0 };

        DoubleBuffer m_buffer;

        uid_t m_uid { 0 };

        int m_fifo_id { 0 };

        WaitQueue m_read_open_queue;
        WaitQueue m_write_open_queue;

        Lock m_open_lock;
    }; // class FIFO

} // namespace Kernel
