/**
 * @file networkjob.h
 * @author Krisna Pranav
 * @brief network job 
 * @version 6.0
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <mods/stream.h>
#include <libcore/forward.h>
#include <libcore/object.h>
#include <libcore/stream.h>

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

        /**
         * @brief Destroy the NetworkJob object
         * 
         */
        virtual ~NetworkJob() override = default;

        Function<void(HashMap<String, String, CaseInsensitiveStringTraits> const& response_headers, Optional<u32> response_code)> on_headers_received;
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

        NetworkResponse const* response() const 
        { 
            return m_response.ptr(); 
        }

        enum class ShutdownMode 
        {
            DetachFromSocket,
            CloseSocket,
        }; // enum class ShutdownMode 

        virtual void start(Core::Stream::Socket&) = 0;
        virtual void shutdown(ShutdownMode) = 0;

        /**
         * @param error 
         */
        virtual void fail(Error error) 
        { 
            did_fail(error); 
        }

        void cancel()
        {
            shutdown(ShutdownMode::DetachFromSocket);
            m_error = Error::Cancelled;
        }

    protected:
        NetworkJob(Core::Stream::Stream&);
        void did_finish(NonnullRefPtr<NetworkResponse>&&);
        void did_fail(Error);

        /**
         * @param total_size 
         * @param downloaded 
         */
        void did_progress(Optional<u32> total_size, u32 downloaded);

        /**
         * @param bytes 
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> do_write(ReadonlyBytes bytes) 
        { 
            return m_output_stream.write(bytes); 
        }

    private:
        RefPtr<NetworkResponse> m_response;
        Core::Stream::Stream& m_output_stream;
        Error m_error { Error::None };
    }; // class NetworkJob : public Object

    char const* to_string(NetworkJob::Error);

} // namespace Core

template<>
struct Mods::Formatter<Core::NetworkJob> : Formatter<Core::Object> {
};
