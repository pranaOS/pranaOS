/**
 * @file blockdevice.h
 * @author Krisna Pranav
 * @brief block device
 * @version 6.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/device/device.h>

namespace Kernel 
{

    class BlockDevice;

    class AsyncBlockDeviceRequest : public AsyncDeviceRequest 
    {
    public:
        enum RequestType 
        {
            Read,
            Write
        }; // enum

        /**
         * @param block_device 
         * @param request_type 
         * @param block_index 
         * @param block_count 
         * @param buffer 
         * @param buffer_size 
         */
        AsyncBlockDeviceRequest(Device& block_device, RequestType request_type,
            u32 block_index, u32 block_count, const UserOrKernelBuffer& buffer, size_t buffer_size);

        /**
         * @return RequestType 
         */
        RequestType request_type() const 
        { 
            return m_request_type; 
        }

        /**
         * @return u32 
         */
        u32 block_index() const 
        { 
            return m_block_index; 
        }

        /**
         * @return u32 
         */
        u32 block_count() const 
        { 
            return m_block_count; 
        }

        /**
         * @return UserOrKernelBuffer& 
         */
        UserOrKernelBuffer& buffer() 
        { 
            return m_buffer; 
        }

        /**
         * @return const UserOrKernelBuffer& 
         */
        const UserOrKernelBuffer& buffer() const 
        { 
            return m_buffer; 
        }

        /**
         * @return size_t 
         */
        size_t buffer_size() const 
        { 
            return m_buffer_size; 
        }

        /// @brief: start
        virtual void start() override;

        /**
         * @return const char* 
         */
        virtual const char* name() const override
        {
            switch (m_request_type) {
            case Read:
                return "BlockDeviceRequest (read)";
            case Write:
                return "BlockDeviceRequest (read)";
            default:
                ASSERT_NOT_REACHED();
            }
        }

    private:
        BlockDevice& m_block_device;

        const RequestType m_request_type;

        const u32 m_block_index;
        const u32 m_block_count;

        UserOrKernelBuffer m_buffer;

        const size_t m_buffer_size;
    }; // class AsyncBlockDeviceRequest

    class BlockDevice : public Device 
    {
    public:
        /// @brief Destroy the BlockDevice object
        virtual ~BlockDevice() override;

        /**
         * @return size_t 
         */
        size_t block_size() const 
        { 
            return m_block_size; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_seekable() const override 
        { 
            return true; 
        }

        /**
         * @param index 
         * @return true 
         * @return false 
         */
        bool read_block(unsigned index, UserOrKernelBuffer&);

        /**
         * @param index 
         * @return true 
         * @return false 
         */
        bool write_block(unsigned index, const UserOrKernelBuffer&);

        /// @breif: start_request
        virtual void start_request(AsyncBlockDeviceRequest&) = 0;

    protected:

        /**
         * @param major 
         * @param minor 
         * @param block_size 
         */
        BlockDevice(unsigned major, unsigned minor, size_t block_size = PAGE_SIZE)
            : Device(major, minor)
            , m_block_size(block_size)
        { }

    private:
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_block_device() const final 
        { 
            return true; 
        }

        size_t m_block_size { 0 };

    }; // class BlockDevice

} // namespace Kernel