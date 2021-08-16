/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Function.h>
#include <base/URL.h>
#include <libcore/Object.h>
#include <libweb/loader/Resource.h>

namespace Protocol {
class RequestClient;
}

namespace Web {

#if ARCH(I386)
#    define CPU_STRING "x86"
#else
#    define CPU_STRING "x86_64"
#endif

constexpr auto default_user_agent = "Mozilla/4.0 (pranaOS; " CPU_STRING ") LibWeb+LibJS (Not KHTML, nor Gecko) LibWeb";

class ResourceLoader : public Core::Object {
    C_OBJECT(ResourceLoader)
public:
    static ResourceLoader& the();

    RefPtr<Resource> load_resource(Resource::Type, const LoadRequest&);

    void load(const LoadRequest&, Function<void(ReadonlyBytes, const HashMap<String, String, CaseInsensitiveStringTraits>& response_headers, Optional<u32> status_code)> success_callback, Function<void(const String&, Optional<u32> status_code)> error_callback = nullptr);
    void load(const URL&, Function<void(ReadonlyBytes, const HashMap<String, String, CaseInsensitiveStringTraits>& response_headers, Optional<u32> status_code)> success_callback, Function<void(const String&, Optional<u32> status_code)> error_callback = nullptr);
    void load_sync(const LoadRequest&, Function<void(ReadonlyBytes, const HashMap<String, String, CaseInsensitiveStringTraits>& response_headers, Optional<u32> status_code)> success_callback, Function<void(const String&, Optional<u32> status_code)> error_callback = nullptr);

    Function<void()> on_load_counter_change;

    int pending_loads() const { return m_pending_loads; }

    Protocol::RequestClient& protocol_client() { return *m_protocol_client; }

    const String& user_agent() const { return m_user_agent; }
    void set_user_agent(const String& user_agent) { m_user_agent = user_agent; }

    void clear_cache();

private:
    ResourceLoader();
    static bool is_port_blocked(int port);

    int m_pending_loads { 0 };

    RefPtr<Protocol::RequestClient> m_protocol_client;
    String m_user_agent;
};

}
