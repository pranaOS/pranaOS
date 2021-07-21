/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Stream.h>
#ifndef KERNEL
#    include <errno.h>
#    include <stdio.h>

namespace Base {

class InputFileStream : public InputStream {
public:
    explicit InputFileStream(int fd)
        : m_file(fdopen(fd, "r"))
        , m_owned(true)
    {
        if (!m_file)
            set_fatal_error();
    }
    
    explicit InputFileStream(FILE* fp)
        : m_file(fp)
    {
        if (!m_file)
            set_fatal_error();
    }

    ~InputFileStream()
    {
        if (m_file) {
            fflush(m_file);
            if (m_owned)
                fclose(m_file);
        }
    }

    
};

}
