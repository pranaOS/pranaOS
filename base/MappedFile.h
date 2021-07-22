/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Noncopyable.h>
#include <base/NonnullRefPtr.h>
#include <base/OSError.h>
#include <base/RefCounted.h>
#include <base/Result.h>

namespace Base {

class MappedFile : public RefCounted<MappedFile> {
    BASE_MAKE_NONCOPYABLE(MappedFile);
    BASE_MAKE_NONMOVABLE(MappedFile);

public:
    static Result<NonnullRefPtr<MappedFile>, OSError> map(const String& path);
    ~MappedFile();

    void* data() { return m_data; }
    const void* data() const { return m_data; }

    size_t size() const { return m_size; }

    ReadonlyBytes bytes() const { return { m_data, m_size }; }

private:
    explicit MappedFile(void*, size_t);

    void* m_data { nullptr };
    size_t m_size { 0 };
};

}

using Base::MappedFile;