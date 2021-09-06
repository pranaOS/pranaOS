#pragma once

#include <bits/sys/stat.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int mkdir(const char* path);
int fstat(int nfds, fstat_t* stat);

__END_DECLS