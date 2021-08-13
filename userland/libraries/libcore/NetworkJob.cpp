/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Debug.h>
#include <libcore/NetworkJob.h>
#include <libcore/NetworkResponse.h>

namespace Core {

NetworkJob::NetworkJob(OutputStream& output_stream)
    : m_output_stream(output_stream)
{
}

NetworkJob::~NetworkJob()
{
}

void NetworkJob::start()
{
}

void NetworkJob::shutdown()
{
}

void NetworkJob::did_finish(NonnullRefPtr<NetworkResponse>&& response)
{
    NonnullRefPtr<NetworkJob> protector(*this);

    m_response = move(response);
    dbgln_if(CNETWORKJOB_DEBUG, "{} job did_finish", *this);
    VERIFY(on_finish);
    on_finish(true);
    shutdown();
}

void NetworkJob::did_fail(Error error)
{
    NonnullRefPtr<NetworkJob> protector(*this);

    m_error = error;
    dbgln_if(CNETWORKJOB_DEBUG, "{}{{{:p}}} job did_fail! error: {} ({})", class_name(), this, (unsigned)error, to_string(error));
    VERIFY(on_finish);
    on_finish(false);
    shutdown();
}

void NetworkJob::did_progress(Optional<u32> total_size, u32 downloaded)
{
    NonnullRefPtr<NetworkJob> protector(*this);

    if (on_progress)
        on_progress(total_size, downloaded);
}

const char* to_string(NetworkJob::Error error)
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

}
