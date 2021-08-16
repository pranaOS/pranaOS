/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Forward.h>
#include <base/HashMap.h>
#include <base/NonnullRefPtrVector.h>
#include <base/RefCounted.h>
#include <base/String.h>

namespace Desktop {

class Launcher {
public:
    enum class LauncherType {
        Default = 0,
        Application,
        UserPreferred,
        UserDefault
    };

    struct Details : public RefCounted<Details> {
        String name;
        String executable;
        LauncherType launcher_type { LauncherType::Default };

        static NonnullRefPtr<Details> from_details_str(const String&);
    };

    [[nodiscard]] static bool add_allowed_url(const URL&);
    [[nodiscard]] static bool add_allowed_handler_with_any_url(const String& handler);
    [[nodiscard]] static bool add_allowed_handler_with_only_specific_urls(const String& handler, const Vector<URL>&);
    [[nodiscard]] static bool seal_allowlist();
    static bool open(const URL&, const String& handler_name = {});
    static bool open(const URL&, const Details& details);
    static Vector<String> get_handlers_for_url(const URL&);
    static NonnullRefPtrVector<Details> get_handlers_with_details_for_url(const URL&);
};

}
