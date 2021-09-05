#pragma once

#include <bits/errno.h>

extern int errno;

#define set_errno(x) (errno = x)