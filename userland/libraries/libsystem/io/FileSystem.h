/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libabi/Result.h>

JResult filesystem_link(const char *oldpath, const char *newpath);

JResult filesystem_unlink(const char *path);

JResult filesystem_mkdir(const char *path);

JResult filesystem_mkpipe(const char *path);

JResult filesystem_rename(const char *old_path, const char *new_path);

bool filesystem_exist(const char *path, JFileType type);
