/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/StringView.h>
#include <kernel/KString.h>

namespace Kernel::KLexicalPath {

bool is_absolute(StringView const&);
bool is_canonical(StringView const&);
StringView basename(StringView const&);
StringView dirname(StringView const&);
Vector<StringView> parts(StringView const&);

OwnPtr<KString> try_join(StringView const&, StringView const&);

}
