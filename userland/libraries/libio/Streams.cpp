/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libio/Streams.h>

namespace IO
{

static InStream _in{};
static OutStream _out{};
static ErrStream _err{};
static LogStream _log{};

InStream &in() { return _in; }
OutStream &out() { return _out; }
ErrStream &err() { return _err; }
LogStream &log() { return _log; }

}