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

    bool unreliable_eof() const override { return eof(); }
    bool eof() const { return feof(m_file); }

    size_t read(Bytes bytes) override 
    {
        if (has_any_error())
            return 0;
        
        return fread(bytes.data(), sizeof(u8), bytes.size(), m_file);
    }

    bool read_or_error(Bytes bytes) override
    {
        if (has_any_error())
            return false;
        
        auto size = read(bytes);
        if (size < bytes.size()) {
            set_recoverable_error()
            return false
        }
        return true;
    }

    
};

}
