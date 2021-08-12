/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/ByteBuffer.h>
#include <base/DistinctNumeric.h>
#include <base/String.h>
#include <base/Types.h>

namespace Kernel {

class FileSystem;
struct InodeMetadata;

TYPEDEF_DISTINCT_ORDERED_ID(u64, InodeIndex);

class InodeIdentifier {
public:
    InodeIdentifier() = default;
    InodeIdentifier(u32 fsid, InodeIndex inode)
        : m_fsid(fsid)
        , m_index(inode)
    {
    }

    bool is_valid() const { return m_fsid != 0 && m_index != 0; }

    u32 fsid() const { return m_fsid; }
    InodeIndex index() const { return m_index; }

    FileSystem* fs();
    const FileSystem* fs() const;

    bool operator==(const InodeIdentifier& other) const
    {
        return m_fsid == other.m_fsid && m_index == other.m_index;
    }

    bool operator!=(const InodeIdentifier& other) const
    {
        return m_fsid != other.m_fsid || m_index != other.m_index;
    }

    String to_string() const { return String::formatted("{}:{}", m_fsid, m_index); }

private:
    u32 m_fsid { 0 };
    InodeIndex m_index { 0 };
};

}

template<>
struct Base::Formatter<Kernel::InodeIdentifier> : Base::Formatter<FormatString> {
    void format(FormatBuilder& builder, Kernel::InodeIdentifier value)
    {
        return Base::Formatter<FormatString>::format(builder, "{}:{}", value.fsid(), value.index());
    }
};

template<>
struct Base::Formatter<Kernel::InodeIndex> : Base::Formatter<FormatString> {
    void format(FormatBuilder& builder, Kernel::InodeIndex value)
    {
        return Base::Formatter<FormatString>::format(builder, "{}", value.value());
    }
};
