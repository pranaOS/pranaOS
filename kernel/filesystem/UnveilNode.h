/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/Trie.h>

namespace Kernel {

enum UnveilAccess {
    Read = 1,
    Write = 2,
    Execute = 4,
    CreateOrRemove = 8,
    Browse = 16,

    None = 0,
};

struct UnveilNode;

struct UnveilMetadata {
    String full_path;
    UnveilAccess permissions { None };
    bool explicitly_unveiled { false };
};

struct UnveilNode final : public Trie<String, UnveilMetadata, Traits<String>, UnveilNode> {
    using Trie<String, UnveilMetadata, Traits<String>, UnveilNode>::Trie;

    bool was_explicitly_unveiled() const { return this->metadata_value().explicitly_unveiled; }
    UnveilAccess permissions() const { return this->metadata_value().permissions; }
    const String& path() const { return this->metadata_value().full_path; }
};

}
