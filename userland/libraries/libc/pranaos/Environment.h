/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

#include <libutils/String.h>
#include <libutils/json/Json.h>

void environment_load(const char *buffer);

Json::Value &environment();

String environment_copy();
