/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// inludes
#include <base/Result.h>
#include <base/Vector.h>
#include <libelf/DynamicObject.h>

namespace ELF {

class DynamicLinker {
public:
    static Optional<DynamicObject::SymbolLookupResult> lookup_global_symbol(const StringView& symbol);
    [[noreturn]] static void linker_main(String&& main_program_name, int fd, bool is_secure, int argc, char** argv, char** envp);

private:
    DynamicLinker() = delete;
    ~DynamicLinker() = delete;
};

}
