/**
 * @file networkjob.h
 * @author Krisna Pranav
 * @brief network job 
 * @version 6.0
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <libcore/object.h>

namespace Core 
{
    class NetworkJob : public Object 
    {
        C_OBJECT_ABSTRACT(NetworkJob)
    public:

        enum class Error 
        {
            None,
            ConnectionFailed,
            TransmissionFailed,
            ProtocolFailed,
            Cancelled,
        }; // enum class Error

        /// @brief Destroy the Network Job object
        virtual ~NetworkJob() override;

        Function<void(bool success)> on_finish;
        Function<void(Optional<u32>, u32)> on_progress;

        /**
         * @return true 
         * @return false 
         */
        bool is_cancelled() const 
        { 
            return m_error == Error::Cancelled; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool has_error() const 
        { 
            return m_error != Error::None; 
        }

        /**
         * @return Error 
         */
        Error error() const 
        { 
            return m_error; 
        }

        /**
         * @return NetworkResponse* 
         */
        NetworkResponse* response() 
        { 
            return m_response.ptr(); 
        }

        /**
         * @return const NetworkResponse* 
         */
        const NetworkResponse* response() const 
        { 
            return m_response.ptr(); 
        }
        
        virtual void start() = 0;
        virtual void shutdown() = 0;

        void cancel()
        {
            shutdown();
            m_error = Error::Cancelled;
        }

    protected:
        /// @brief Construct a new Network Job object
        NetworkJob();

        void did_finish(NonnullRefPtr<NetworkResponse>&&);

        void did_fail(Error);

        /**
         * @param total_size 
         * @param downloaded 
         */
        void did_progress(Optional<u32> total_size, u32 downloaded);

    private:
        RefPtr<NetworkResponse> m_response;
        Error m_error { Error::None };

    }; // class NetworkJob

    /**
     * @return const char* 
     */
    const char* to_string(NetworkJob::Error);

} // namespace Core