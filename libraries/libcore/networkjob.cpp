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
    
} // namespace Core
