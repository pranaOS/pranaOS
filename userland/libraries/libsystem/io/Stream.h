/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libabi/IOCall.h>
#include <libabi/Paths.h>
#include <libio/Handle.h>
#include <libio/Seek.h>

#define STREAM_BUFFER_SIZE 512

struct Stream
{
    Handle handle;

    bool has_unget;
    int unget_char;

    bool is_end_of_file;
};

Stream *stream_open(const char *path, JOpenFlag flags);

Stream *stream_open_handle(int handle_id, JOpenFlag flags);

void stream_close(Stream *stream);

void stream_cleanup(Stream **stream);

size_t stream_read(Stream *stream, void *buffer, size_t size);

size_t stream_write(Stream *stream, const void *buffer, size_t size);

JResult stream_call(Stream *stream, IOCall request, void *arg);

int stream_seek(Stream *stream, IO::SeekFrom from);

void stream_stat(Stream *stream, JStat *stat);

int stream_format(Stream *stream, const char *fmt, ...);

int stream_vprintf(Stream *stream, const char *fmt, va_list va);