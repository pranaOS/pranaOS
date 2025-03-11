/**
 * @file assertions.cpp
 * @author Krisna Pranav
 * @brief assertions
 * @version 6.0
 * @date 2025-03-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/format.h>
#include <mods/platform.h>
#include <mods/stringbuilder.h>
#include <mods/stringview.h>

#if (defined(MODS_OS_LINUX) && !defined(MODS_OS_ANDROID)) || defined(MODS_LIBC_GLIBC) || defined(MODS_OS_BSD_GENERIC) || defined(MODS_OS_SOLARIS) || defined(MODS_OS_HAIKU)
#    define EXECINFO_BACKTRACE
#endif

#if defined(MODS_OS_ANDROID) && (__ANDROID_API__ >= 33)
#    include <android/log.h>
#    define EXECINFO_BACKTRACE
#    define PRINT_ERROR(s) __android_log_write(ANDROID_LOG_WARN, "Mods", (s))
#else
#    define PRINT_ERROR(s) (void)fputs((s), stderr)
#endif

#if defined(EXECINFO_BACKTRACE)
#    include <cxxabi.h>
#    include <execinfo.h>
#endif

#if defined(MODS_OS_PRANAOS)
#    define ERRORLN dbgln
#else
#    define ERRORLN warnln
#endif

#if !defined(KERNEL)

#    if defined(EXECINFO_BACKTRACE)
namespace 
{
    
ALWAYS_INLINE void dump_backtrace()
{
    void* trace[256] = {};
    int const num_frames = backtrace(trace, array_size(trace));
    char** syms = backtrace_symbols(trace, num_frames);

    for (auto i = 0; i < num_frames; ++i) {
        StringView sym(syms[i], strlen(syms[i]));
        StringBuilder error_builder;
        if (auto idx = sym.find("_Z"sv); idx.has_value()) {
            syms[i][idx.value() - 1] = '\0';
            error_builder.append(syms[i], strlen(syms[i]));
            error_builder.append(' ');

            auto sym_substring = sym.substring_view(idx.value());
            auto end_of_sym = sym_substring.find_any_of("+ "sv).value_or(sym_substring.length() - 1);
            syms[i][idx.value() + end_of_sym] = '\0';

            size_t buf_size = 128u;
            char* buf = static_cast<char*>(malloc(buf_size));
            auto* raw_str = &syms[i][idx.value()];
            buf = abi::__cxa_demangle(raw_str, buf, &buf_size, nullptr);

            auto* buf_to_print = buf ? buf : raw_str;
            error_builder.append(buf_to_print, strlen(buf_to_print));
            free(buf);

            error_builder.append(' ');
            auto* end_of_line = &syms[i][idx.value() + end_of_sym + 1];
            error_builder.append(end_of_line, strlen(end_of_line));
        } else {
            error_builder.append(sym);
        }
#        if !defined(MODS_OS_ANDROID)
        error_builder.append('\n');
#        endif
        error_builder.append('\0');
        PRINT_ERROR(error_builder.string_view().characters_without_null_termination());
    }
    free(syms);
}
}
#    endif

extern "C" {

void mods_verification_failed(char const* message)
{
#    if defined(MODS_OS_PRANAOS) || defined(MODS_OS_ANDROID)
    bool colorize_output = true;
#    elif defined(MODS_OS_WINDOWS)
    bool colorize_output = false;
#    else
    bool colorize_output = isatty(STDERR_FILENO) == 1;
#    endif

    if (colorize_output)
        ERRORLN("\033[31;1mVERIFICATION FAILED\033[0m: {}", message);
    else
        ERRORLN("VERIFICATION FAILED: {}", message);

#    if defined(EXECINFO_BACKTRACE)
    dump_backtrace();
#    endif
    __builtin_trap();
}
}

#endif
