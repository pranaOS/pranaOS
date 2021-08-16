/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/LexicalPath.h>
#include <base/Optional.h>
#include <base/String.h>
#include <spawn.h>

namespace Core {

String command(const String& program, const Vector<String>& arguments, Optional<LexicalPath> chdir);
String command(const String& command_string, Optional<LexicalPath> chdir);

}
