/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <EP/ByteBuffer.h>
#include <EP/String.h>
#include <EP/StringView.h>

namespace EP {

size_t calculate_base64_decoded_length(const StringView&);

size_t calculate_base64_encoded_length(ReadonlyBytes);

ByteBuffer decode_base64(const StringView&);

String encode_base64(ReadonlyBytes);

}

using EP::decode_base64;
using EP::encode_base64;
