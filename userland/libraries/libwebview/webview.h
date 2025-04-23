/**
 * @file webview.h
 * @author Krisna Pranav
 * @brief webview
 * @version 6.0
 * @date 2025-04-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifdef WIN32
#define LIB_WEBVIEW_API __declspec(dllexport)
#else
#define LIB_WEBVIEW_API
#endif

extern "C"
{
    typedef void* C_Webview;

    /**
     * @param app_name 
     * @param title 
     * @param width 
     * @param height 
     * @param resizeable 
     * @param debug_mode 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API C_Webview webview_create_app(char const* app_name, char const* title, uint32_t const width,
                                                 uint32_t const height, bool const resizeable, bool const debug_mode);

    /**
     * @param instance 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API void webview_delete_app(C_Webview instance);

    /**
     * @param instance 
     * @param url_path 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API bool webview_run_app(C_Webview instance, char const* url_path);

    /**
     * @param instance 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API void webview_quit_app(C_Webview instance);

    /**
     * @param instance 
     * @param width 
     * @param height 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API void webview_set_max_size_app(C_Webview instance, uint32_t const width, uint32_t const height);

    /**
     * @param instance 
     * @param width 
     * @param height 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API void webview_set_min_size_app(C_Webview instance, uint32_t const width, uint32_t const height);

    /**
     * @param instance 
     * @param width 
     * @param height 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API void webview_set_size_app(C_Webview instance, uint32_t const width, uint32_t const height);

    /**
     * @param instance 
     * @param name 
     * @param function 
     * @param context 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API bool webview_bind(C_Webview instance, char const* name,
                                      void (*function)(void*, uint64_t, char const*), void* context);

    /**
     * @param instance 
     * @param index 
     * @param success 
     * @param data 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API void webview_result(C_Webview instance, uint64_t index, bool success, char const* data);

    /**
     * @param instance 
     * @param function 
     * @param context 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API void webview_set_idle_callback(C_Webview instance, void (*function)(void*), void* context);

    /**
     * @param instance 
     * @param initial_path 
     * @param filter 
     * @return LIB_WEBVIEW_API 
     */
    LIB_WEBVIEW_API void webview_show_save_dialog(C_Webview instance, char const* initial_path, char const* filter);
} // extern "C"
