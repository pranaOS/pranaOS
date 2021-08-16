/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// incldues
#include <base/Badge.h>
#include <base/ByteBuffer.h>
#include <base/FileStream.h>
#include <base/Function.h>
#include <base/MemoryStream.h>
#include <base/RefCounted.h>
#include <base/String.h>
#include <base/WeakPtr.h>
#include <libcore/Notifier.h>
#include <libipc/Forward.h>

namespace Protocol {

class RequestClient;

class Request : public RefCounted<Request> {
public:
    struct CertificateAndKey {
        String certificate;
        String key;
    };

    static NonnullRefPtr<Request> create_from_id(Badge<RequestClient>, RequestClient& client, i32 request_id)
    {
        return adopt_ref(*new Request(client, request_id));
    }

    int id() const { return m_request_id; }
    int fd() const { return m_fd; }
    bool stop();

    void stream_into(OutputStream&);

    bool should_buffer_all_input() const { return m_should_buffer_all_input; }

    void set_should_buffer_all_input(bool);

    Function<void(bool success, u32 total_size, const HashMap<String, String, CaseInsensitiveStringTraits>& response_headers, Optional<u32> response_code, ReadonlyBytes payload)> on_buffered_request_finish;
    Function<void(bool success, u32 total_size)> on_finish;
    Function<void(Optional<u32> total_size, u32 downloaded_size)> on_progress;
    Function<void(const HashMap<String, String, CaseInsensitiveStringTraits>& response_headers, Optional<u32> response_code)> on_headers_received;
    Function<CertificateAndKey()> on_certificate_requested;

    void did_finish(Badge<RequestClient>, bool success, u32 total_size);
    void did_progress(Badge<RequestClient>, Optional<u32> total_size, u32 downloaded_size);
    void did_receive_headers(Badge<RequestClient>, const HashMap<String, String, CaseInsensitiveStringTraits>& response_headers, Optional<u32> response_code);
    void did_request_certificates(Badge<RequestClient>);

    RefPtr<Core::Notifier>& write_notifier(Badge<RequestClient>) { return m_write_notifier; }
    void set_request_fd(Badge<RequestClient>, int fd) { m_fd = fd; }

private:
    explicit Request(RequestClient&, i32 request_id);
    WeakPtr<RequestClient> m_client;
    int m_request_id { -1 };
    RefPtr<Core::Notifier> m_write_notifier;
    int m_fd { -1 };
    bool m_should_buffer_all_input { false };

    struct InternalBufferedData {
        InternalBufferedData(int fd)
            : read_stream(fd)
        {
        }

        InputFileStream read_stream;
        DuplexMemoryStream payload_stream;
        HashMap<String, String, CaseInsensitiveStringTraits> response_headers;
        Optional<u32> response_code;
    };

    struct InternalStreamData {
        InternalStreamData(int fd)
            : read_stream(fd)
        {
        }

        InputFileStream read_stream;
        RefPtr<Core::Notifier> read_notifier;
        bool success;
        u32 total_size { 0 };
        bool request_done { false };
    };

    OwnPtr<InternalBufferedData> m_internal_buffered_data;
    OwnPtr<InternalStreamData> m_internal_stream_data;
};

}
