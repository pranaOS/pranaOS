/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libio/Path.h>

#include "system/node/Handle.h"

struct Handles
{
private:
    Lock _lock{"handles-lock"};

    RefPtr<FsHandle> _handles[PROCESS_HANDLE_COUNT];

    ResultOr<int> add(RefPtr<FsHandle> handle);

    JResult add_at(RefPtr<FsHandle> handle, int index);

    bool is_valid_handle(int handle);

    JResult remove(int handle_index);

    RefPtr<FsHandle> acquire(int handle_index);

    JResult release(int handle_index);

public:
    Handles() {}

    ~Handles() { close_all(); }

    ResultOr<int> open(Domain &domain, IO::Path &path, JOpenFlag flags);

    ResultOr<int> connect(Domain &domain, IO::Path &socket_path);

    JResult close(int handle_index);

    void close_all();

    JResult reopen(int handle, int *reopened);

    JResult copy(int source, int destination);

    JResult poll(HandlePoll *handles, size_t count, Timeout timeout);

    ResultOr<size_t> read(int handle_index, void *buffer, size_t size);

    ResultOr<size_t> write(int handle_index, const void *buffer, size_t size);

    ResultOr<ssize64_t> seek(int handle_index, IO::SeekFrom from);

    JResult call(int handle_index, IOCall request, void *args);

    JResult stat(int handle_index, JStat *stat);

    ResultOr<int> accept(int handle_index);

    JResult duplex(
            RefPtr<FsNode> node,
            int *server, JOpenFlag server_flags,
            int *client, JOpenFlag client_flags);

    JResult term(int *server, int *client);

    JResult pipe(int *reader, int *writer);

    JResult pass(Handles &handles, int source, int destination);
};