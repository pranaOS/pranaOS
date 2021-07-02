/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

int utime(const char* pathname, const struct utimbuf*);

__END_DECLS
