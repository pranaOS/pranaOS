/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Concepts.h>
#include <base/String.h>
#include <base/Vector.h>
#include <kernel/VirtualAddress.h>

namespace ELF {

bool perform_relative_relocations(FlatPtr base_address);

}
