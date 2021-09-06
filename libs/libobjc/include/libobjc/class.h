/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libobjc/module.h>

#define DISPATCH_TABLE_NOT_INITIALIZED (void*)0x0

bool class_resolve_links(Class cls);
void class_resolve_all_unresolved();

void class_table_init();
void class_add_from_module(struct objc_symtab* symtab);
IMP class_get_implementation(Class cls, SEL sel);

Class objc_getClass(const char* name);