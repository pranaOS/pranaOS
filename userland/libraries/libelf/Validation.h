/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <libc/elf.h>

namespace ELF {

bool validate_elf_header(const ElfW(Ehdr) & elf_header, size_t file_size, bool verbose = true);
bool validate_program_headers(const ElfW(Ehdr) & elf_header, size_t file_size, const u8* buffer, size_t buffer_size, String* interpreter_path, bool verbose = true);

} 
