/**
 * @file injection.hpp
 * @author Krisna Pranav
 * @brief injection
 * @version 6.0
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace LibWebView
{
    namespace JS
    {
        std::string const onLoadHTMLInjection = R"()";

        std::string const onResultResolveInjection = "webview.results";
    } // namespace JS
} // namespace LibWebView