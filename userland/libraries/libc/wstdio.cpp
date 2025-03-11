/**
 * @file wstdio.cpp
 * @author Krisna Pranav
 * @brief wstdio
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/bitcast.h>
#include <mods/printfimplementation.h>
#include <mods/stringbuilder.h>
#include <mods/types.h>
#include <bits/stdio_file_implementation.h>
#include <errno.h>
#include <stdio.h>
#include <wchar.h>

static_assert(AssertSize<wchar_t, sizeof(u32)>());

extern "C" {

/**
 * @param mode 
 * @return int 
 */
int fwide(FILE*, int mode)
{
    return mode;
}

/**
 * @param stream 
 * @return wint_t 
 */
wint_t fgetwc(FILE* stream)
{
    VERIFY(stream);
    Array<u8, 4> underlying;
    auto underlying_bytes = underlying.span();
    size_t encoded_length = 0;
    size_t bytes_read = 0;
    do {
        size_t nread = fread(underlying_bytes.offset_pointer(bytes_read), 1, 1, stream);
        if (nread != 1) {
            errno = EILSEQ;
            return WEOF;
        }
        ++bytes_read;
        if (bytes_read == 1) {
            if (underlying[0] >> 7 == 0) {
                encoded_length = 1;
            } else if (underlying[0] >> 5 == 6) {
                encoded_length = 2;
            } else if (underlying[0] >> 4 == 0xe) {
                encoded_length = 3;
            } else if (underlying[0] >> 3 == 0x1e) {
                encoded_length = 4;
            } else {
                errno = EILSEQ;
                return WEOF;
            }
        }
    } while (bytes_read < encoded_length);

    wchar_t code_point;
    auto read_bytes = mbrtowc(&code_point, bit_cast<char const*>(underlying.data()), encoded_length, nullptr);
    VERIFY(read_bytes == encoded_length);
    return code_point;
}

/**
 * @param stream 
 * @return wint_t 
 */
wint_t getwc(FILE* stream)
{
    return fgetwc(stream);
}

/**
 * @return wint_t 
 */
wint_t getwchar()
{
    return getwc(stdin);
}

/**
 * @param wc 
 * @param stream 
 * @return wint_t 
 */
wint_t fputwc(wchar_t wc, FILE* stream)
{
    VERIFY(stream);
   
    if constexpr (IsSigned<wchar_t>) {
        if (wc < 0) {
            errno = EILSEQ;
            return WEOF;
        }
    }

    StringBuilder sb;
    sb.append_code_point(static_cast<u32>(wc));
    auto bytes = sb.string_view().bytes();
    ScopedFileLock lock(stream);
    size_t nwritten = stream->write(bytes.data(), bytes.size());
    if (nwritten < bytes.size())
        return WEOF;
    return wc;
}

/**
 * @param wc 
 * @param stream 
 * @return wint_t 
 */
wint_t putwc(wchar_t wc, FILE* stream)
{
    return fputwc(wc, stream);
}

/**
 * @param wc 
 * @return wint_t 
 */
wint_t putwchar(wchar_t wc)
{
    return fputwc(wc, stdout);
}

/**
 * @param buffer 
 * @param size 
 * @param stream 
 * @return wchar_t* 
 */
wchar_t* fgetws(wchar_t* __restrict buffer, int size, FILE* __restrict stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    bool ok = stream->gets(bit_cast<u32*>(buffer), size);
    return ok ? buffer : nullptr;
}

/**
 * @param ws 
 * @param stream 
 * @return int 
 */
int fputws(wchar_t const* __restrict ws, FILE* __restrict stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    int size = 0;
    for (auto const* p = ws; *p != 0; ++p, ++size) {
        if (putwc(*p, stream) == WEOF)
            return WEOF;
    }
    return size;
}

/**
 * @param wc 
 * @param stream 
 * @return wint_t 
 */
wint_t ungetwc(wint_t wc, FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    StringBuilder sb;
    sb.append_code_point(static_cast<u32>(wc));
    auto bytes = sb.string_view().bytes();
    size_t ok_bytes = 0;
    for (auto byte : bytes) {
        if (!stream->ungetc(byte)) {
            stream->read(const_cast<u8*>(bytes.data()), ok_bytes);
            return WEOF;
        }
        ++ok_bytes;
    }
    return wc;
}

/**
 * @param format 
 * @param ... 
 * @return int 
 */
int wprintf(wchar_t const* __restrict format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto rc = vfwprintf(stdout, format, ap);
    va_end(ap);
    return rc;
}

/**
 * @param stream 
 * @param format 
 * @param ... 
 * @return int 
 */
int fwprintf(FILE* __restrict stream, wchar_t const* __restrict format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto rc = vfwprintf(stream, format, ap);
    va_end(ap);
    return rc;
}

/**
 * @param wcs 
 * @param max_length 
 * @param format 
 * @param ... 
 * @return int 
 */
int swprintf(wchar_t* __restrict wcs, size_t max_length, wchar_t const* __restrict format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto rc = vswprintf(wcs, max_length, format, ap);
    va_end(ap);
    return rc;
}

/**
 * @param format 
 * @param args 
 * @return int 
 */
int vwprintf(wchar_t const* __restrict format, va_list args)
{
    return vfwprintf(stdout, format, args);
}

/**
 * @param stream 
 * @param format 
 * @param args 
 * @return int 
 */
int vfwprintf(FILE* __restrict stream, wchar_t const* __restrict format, va_list args)
{
    auto const* fmt = bit_cast<wchar_t const*>(format);
    return printf_internal([stream](wchar_t*&, wchar_t wc) {
        putwc(wc, stream);
    },
        nullptr, fmt, args);
}

/**
 * @param wcs 
 * @param max_length 
 * @param format 
 * @param args 
 * @return int 
 */
int vswprintf(wchar_t* __restrict wcs, size_t max_length, wchar_t const* __restrict format, va_list args)
{
    auto const* fmt = bit_cast<wchar_t const*>(format);
    size_t length_so_far = 0;
    printf_internal([max_length, &length_so_far](wchar_t*& buffer, wchar_t wc) {
        if (length_so_far > max_length)
            return;
        *buffer++ = wc;
        ++length_so_far;
    },
        wcs, fmt, args);
    if (length_so_far < max_length)
        wcs[length_so_far] = L'\0';
    else
        wcs[max_length - 1] = L'\0';
    return static_cast<int>(length_so_far);
}

/**
 * @param stream 
 * @param format 
 * @param ... 
 * @return int 
 */
int fwscanf(FILE* __restrict stream, wchar_t const* __restrict format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto rc = vfwscanf(stream, format, ap);
    va_end(ap);
    return rc;
}

/**
 * @param ws 
 * @param format 
 * @param ... 
 * @return int 
 */
int swscanf(wchar_t const* __restrict ws, wchar_t const* __restrict format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto rc = vswscanf(ws, format, ap);
    va_end(ap);
    return rc;
}

/**
 * @param format 
 * @param ... 
 * @return int 
 */
int wscanf(wchar_t const* __restrict format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto rc = vfwscanf(stdout, format, ap);
    va_end(ap);
    return rc;
}

/**
 * @param stream 
 * @param format 
 * @param arg 
 * @return int 
 */
int vfwscanf(FILE* __restrict stream, wchar_t const* __restrict format, va_list arg)
{
    (void)stream;
    (void)format;
    (void)arg;
    dbgln("FIXME: Implement vfwscanf()");
    TODO();
}

/**
 * @param ws 
 * @param format 
 * @param arg 
 * @return int 
 */
int vswscanf(wchar_t const* __restrict ws, wchar_t const* __restrict format, va_list arg)
{
    (void)ws;
    (void)format;
    (void)arg;
    dbgln("FIXME: Implement vswscanf()");
    TODO();
}

/**
 * @param format 
 * @param arg 
 * @return int 
 */
int vwscanf(wchar_t const* __restrict format, va_list arg)
{
    (void)format;
    (void)arg;
    dbgln("FIXME: Implement vwscanf()");
    TODO();
}
}
