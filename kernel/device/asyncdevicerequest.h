/**
 * @file asyncdevicerequest.h
 * @author Krisna Pranav
 * @brief async device request
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/nonnullrefptrvector.h>
#include <kernel/process.h>
#include <kernel/thread.h>
#include <kernel/userorkernelbuffer.h>
#include <kernel/waitqueue.h>
#include <kernel/vm/processpagingscope.h>

namespace Kernel
{
    class Device;

    class AsyncDeviceRequest : public RefCounted<AsyncDeviceRequest> 
    {
        MOD_MAKE_NONCOPYABLE(AsyncDeviceRequest);
        MOD_MAKE_NONMOVABLE(AsyncDeviceRequest);

    public:
        enum RequestResult 
        {
            Pending = 0,
            Started,
            Success,
            Failure,
            MemoryFault,
            Cancelled
        };  

        class RequestWaitResult
        {
            friend class AsyncDeviceRequest;

        public:

            /**
             * @return RequestResult 
             */
            RequestResult request_result() const
            {
                return m_request_result;
            }

            /**
             * @return Thread::BlockResult 
             */
            Thread::BlockResult wait_result() const
            {
                return m_wait_result;
            }

        private:

            /**
             * @param request_result 
             * @param wait_result 
             */
            RequestWaitResult(RequestResult request_result, Thread::BlockResult wait_result)
                : m_request_result(request_result)
                , m_wait_result(wait_result)
            {}

            RequestResult m_request_result;
            Thread::BlockResult m_wait_result;
        }; // class RequestWaitResult

        virtual ~AsyncDeviceRequest();

        virtual const char* name() const = 0;

        virtual void start() = 0;

        /// @brief: add sub request
        void add_sub_request(NonnullRefPtr<AsyncDeviceRequest>);

        /**
         * @return RequestWaitResult 
         */
        [[nodiscard]] RequestWaitResult wait(timeval* = nullptr);

        /// @brief: do_start
        void do_start(Badge<Device>)
        {
            do_start();
        }

        

    protected:
        AsyncDeviceRequest(Device&);

        RequestResult get_request_result() const;

    private:
        void sub_request_finished(AsyncDeviceRequest&);

        void request_finished();

        void do_start()
        {
            {
                ScopedSpinLock lock(m_lock);
                if (is_completed_result(m_result))
                    return;
                
                m_result = Started;
            }
        }

        /**
         * @param buffer 
         * @return true 
         * @return false 
         */
        bool in_target_context(const UserOrKernelBuffer& buffer) const
        {
            if (buffer.is_kernel_buffer())
                return true;
            
            return m_process == Process::current();
        }

        /**
         * @param result 
         * @return true 
         * @return false 
         */
        static bool is_completed_result(RequestResult result)
        {
            return result > Started;
        }

        Device& m_device;

        AsyncDeviceRequest* m_parent_request { nullptr };

        RequestResult m_result { Pending };

        NonnullRefPtrVector<AsyncDeviceRequest> m_sub_requests_pending;
        NonnullRefPtrVector<AsyncDeviceRequest> m_sub_requests_complete;

        WaitQueue m_queue;

        NonnullRefPtr<Process> m_process;

        void* m_private { nullptr };

        mutable SpinLock<u8> m_lock;

    }; // class AsyncDeviceRequest

} // namespace Kernel