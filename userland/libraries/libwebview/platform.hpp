/**
 * @file platform.hpp
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-04-29
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace LibWebView
{
    enum class MessageDialogType
    {
        Information,
        Error,
        Warning
    }; // enum class MessageDialogType

    /**
     * @param title 
     * @param message 
     * @param messageType 
     */
    extern auto showMessageDialog(std::string_view const title, std::string_view const message, MessageDialogType const messageType) -> void;

    class Platform
    {
    public:
        using bind_func_t = std::function<void(uint64_t const, std::string_view const)>;
        using idle_func_t = std::function<void()>;

        virtual ~Platform() = default;

        /**
         * @param appName 
         * @param title 
         * @param width 
         * @param height 
         * @param resizeable 
         * @param debugMode 
         * @return std::unique_ptr<Platform> 
         */
        static auto create(std::string_view const appName, std::string_view const title, uint32_t const width, uint32_t const height, bool const resizeable, bool const debugMode) -> std::unique_ptr<Platform>;

        /**
         * @brief Set the Window Max Size object
         * 
         * @param width 
         * @param height 
         */
        virtual auto setWindowMaxSize(uint32_t const width, uint32_t const height) -> void = 0;

        /**
         * @brief Set the Window Min Size object
         * 
         * @param width 
         * @param height 
         */
        virtual auto setWindowMinSize(uint32_t const width, uint32_t const height) -> void = 0;

        /**
         * @brief Set the Window Size object
         * 
         * @param width 
         * @param height 
         */
        virtual auto setWindowSize(uint32_t const width, uint32_t const height) -> void = 0;

        /**
         * @param urlPath 
         */
        virtual auto run(std::string_view const urlPath) -> void = 0;

        /**
         * @param executeCode 
         */
        virtual auto executeJavaScript(std::string_view const executeCode) -> void = 0;

        virtual auto quit() -> void = 0;

        /**
         * @brief Set the Idle Callback object
         * 
         * @tparam Func 
         * @param function 
         */
        template <typename Func>
        auto setIdleCallback(Func&& function) -> void
        {
            idleCallback = [function]() { function(); };
        }

        /**
         * @tparam Func 
         * @param functionName 
         * @param function 
         */
        template <typename Func>
        auto bind(std::string_view const functionName, Func&& function) -> void
        {
            if (bindCallbacks.find(std::string(functionName)) != bindCallbacks.end())
            {
                throw std::runtime_error("Cannot to bind a function that already exists");
            }
            bindCallbacks.emplace(
                std::string(functionName),
                [function](uint64_t const index, std::string_view const arguments) { function(index, arguments); });
        }

        /**
         * @param index 
         * @param success 
         * @param data 
         */
        auto result(uint64_t const index, bool const success, std::string_view const data) -> void;

        /**
         * @param initialPath 
         * @param filter 
         * @return std::optional<std::filesystem::path> 
         */
        virtual auto showSaveDialog(std::filesystem::path const& initialPath, std::string_view const filter) -> std::optional<std::filesystem::path> = 0;

    protected:
        std::unordered_map<std::string, bind_func_t> bindCallbacks;
        idle_func_t idleCallback;
    }; // class Platform

} // namespace LibWebView