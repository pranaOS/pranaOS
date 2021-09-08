/*
 * Copyright (c) 2021, Krisna Pranav, NukeWilliams
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libio/seek.h>

namespace IO {

template <typename T>
concept SeekableWriter = IsBaseOf<Writer, T>::value &&IsBaseOf<Seek, T>::value;


}