#pragma once

#include <bits/sys/ioctls.h>
#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int ioctl(int fd, uint32_t cmd, uint32_t arg);

__END_DECLS