/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Span.h>
#include <libcrypto/asn1/ASN1.h>
#include <libcrypto/asn1/DER.h>

namespace Crypto {

ByteBuffer decode_pem(ReadonlyBytes);

}
