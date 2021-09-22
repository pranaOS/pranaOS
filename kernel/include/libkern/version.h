#pragma once

#define OSTYPE "pranaOS"
#define OSRELEASE "leopard"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_REVISION 0
#define VERSION_VARIANT "0"

#ifdef __i386__
#define MACHINE "x86"
#elif __arm__
#define MACHINE "arm"
#endif