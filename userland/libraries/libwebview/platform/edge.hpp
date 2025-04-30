/**
 * @file edge.hpp
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-04-30
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "platform.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <ShellScalingApi.h>
#include <dwmapi.h>
#include <webview2/WebView2.h>
#include <webview2/WebView2EnvironmentOptions.h>
#include <winrt/base.h>

namespace LibWebView 
{
    class Edge final : public Platform 
    {
    public:
        /**
         * @brief Construct a new Edge object
         * 
         * @param appName 
         * @param title 
         * @param width 
         * @param height 
         * @param resizeable 
         * @param debugMode 
         */
        Edge(std::string_view const appName, std::string_view const title, uint32_t const width, uint32_t const height, bool const resizeable, bool const debugMode);

        /**
         * @brief Set the Window Max Size object
         * 
         * @param width 
         * @param height 
         */
        auto setWindowMaxSize(uint32_t const width, uint32_t const height) -> void override;

        /**
         * @brief Set the Window Min Size object
         * 
         * @param width 
         * @param height 
         */
        auto setWindowMinSize(uint32_t const width, uint32_t const height) -> void override;

        /**
         * @brief Set the Window Size object
         * 
         * @param width 
         * @param height 
         */
        auto setWindowSize(uint32_t const width, uint32_t const height) -> void override;

        /**
         * @param urlPath 
         */
        auto run(std::string_view const urlPath) -> void override;

        /**
         * @param executeCode 
         */
        auto executeJavaScript(std::string_view const executeCode) -> void override;

        auto quit() -> void override;

        /**
         * @param initialPath 
         * @param filter 
         * @return std::optional<std::filesystem::path> 
         */
        auto showSaveDialog(std::filesystem::path const& initialPath, std::string_view const filter) -> std::optional<std::filesystem::path> override;

    private:
        HWND window;
        DEVICE_SCALE_FACTOR scaleFactor;

        struct WindowSize
        {
            uint32_t width;
            uint32_t height;
        }; // struct WindowSize

        WindowSize minSize;
        WindowSize maxSize;

        winrt::com_ptr<ICoreWebView2Environment> environment;
        winrt::com_ptr<ICoreWebView2Controller> controller;
        winrt::com_ptr<ICoreWebView2> webview;
        EventRegistrationToken eventToken;
        bool isInitialized;
        std::binary_semaphore semaphore;

        static auto windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    }; // class Edge final : public Platform

} // namespace LibWebView
