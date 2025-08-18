/**
 * @file types.h
 * @author Krisna Pranav
 * @brief types
 * @version 6.0
 * @date 2025-08-18
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>

namespace Finder 
{

    enum class EnableCallgrindProfiling 
    {
        No,
        Yes
    }; // enum class EnableCallgrindProfiling 

    enum class EnableGPUPainting 
    {
        No,
        Yes
    }; // enum class EnableGPUPainting 

    enum class EnableExperimentalCPUTransforms 
    {
        No,
        Yes
    }; // enum class EnableExperimentalCPUTransforms

    enum class IsLayoutTestMode 
    {
        No,
        Yes
    }; // enum class IsLayoutTestMode 

    enum class UseLagomNetworking 
    {
        No,
        Yes
    }; // enum class UseLagomNetworking 

    enum class WaitForDebugger 
    {
        No,
        Yes
    }; // enum class WaitForDebugger 
 
    enum class LogAllJSExceptions 
    {
        No,
        Yes
    }; // enum class LogAllJSExceptions 

    enum class EnableIDLTracing 
    {
        No,
        Yes
    }; // enum class EnableIDLTracing 

    enum class ExposeInternalsObject 
    {
        No,
        Yes
    }; // enum class ExposeInternalsObject 

    struct WebContentOptions 
    {
        String command_line;
        String executable_path;
        EnableCallgrindProfiling enable_callgrind_profiling { EnableCallgrindProfiling::No };
        EnableGPUPainting enable_gpu_painting { EnableGPUPainting::No };
        EnableExperimentalCPUTransforms enable_experimental_cpu_transforms { EnableExperimentalCPUTransforms::No };
        IsLayoutTestMode is_layout_test_mode { IsLayoutTestMode::No };
        UseLagomNetworking use_lagom_networking { UseLagomNetworking::Yes };
        WaitForDebugger wait_for_debugger { WaitForDebugger::No };
        LogAllJSExceptions log_all_js_exceptions { LogAllJSExceptions::No };
        EnableIDLTracing enable_idl_tracing { EnableIDLTracing::No };
        ExposeInternalsObject expose_internals_object { ExposeInternalsObject::No };
    }; // struct WebContentOptions 

} // namespace Finder
