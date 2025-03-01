/**
 * @file networkjob.cpp
 * @author Krisna Pranav
 * @brief Network Job
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/debug.h>
#include <libcore/networkjob.h>
#include <libcore/networkresponse.h>

namespace Core 
{

    /**
     * @brief Construct a new NetworkJob::NetworkJob object
     * 
     * @param output_stream 
     */
    NetworkJob::NetworkJob(Core::Stream::Stream& output_stream)
        : m_output_stream(output_stream)
    {
    }

    void NetworkJob::start(Core::Stream::Socket&)
    {
    }

    void NetworkJob::shutdown(ShutdownMode)
    {
    }

    /**
     * @param response 
     */
    void NetworkJob::did_finish(NonnullRefPtr<NetworkResponse>&& response)
    {
        if (is_cancelled())
            return;

        NonnullRefPtr<NetworkJob> protector(*this);

        m_response = move(response);
        dbgln_if(NETWORKJOB_DEBUG, "{} job did_finish", *this);
        VERIFY(on_finish);
        on_finish(true);
        shutdown(ShutdownMode::DetachFromSocket);
    }

    /**
     * @param error 
     */
    void NetworkJob::did_fail(Error error)
    {
        if (is_cancelled())
            return;

        NonnullRefPtr<NetworkJob> protector(*this);

        m_error = error;
        dbgln_if(NETWORKJOB_DEBUG, "{}{{{:p}}} job did_fail! error: {} ({})", class_name(), this, (unsigned)error, to_string(error));
        VERIFY(on_finish);
        on_finish(false);
        shutdown(ShutdownMode::DetachFromSocket);
    }

    /**
     * @param total_size 
     * @param downloaded 
     */
    void NetworkJob::did_progress(Optional<u32> total_size, u32 downloaded)
    {
        if (is_cancelled())
            return;

        NonnullRefPtr<NetworkJob> protector(*this);

        if (on_progress)
            on_progress(total_size, downloaded);
    }

    /**
     * @param error 
     * @return char const* 
     */
    char const* to_string(NetworkJob::Error error)
    {
        switch (error) {
        case NetworkJob::Error::ProtocolFailed:
            return "ProtocolFailed";
        case NetworkJob::Error::ConnectionFailed:
            return "ConnectionFailed";
        case NetworkJob::Error::TransmissionFailed:
            return "TransmissionFailed";
        case NetworkJob::Error::Cancelled:
            return "Cancelled";
        default:
            return "(Unknown error)";
        }
    }

} // namespace Core
