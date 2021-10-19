#if defined(__clang__) && !defined(__OBJC_RUNTIME_INTERNAL__)
#pragma clang system_header
#endif

#include "objc-visibility.h"

#ifdef STRICT_MACOS_X
#    define OBJC_NONPORTABLE __attribute__((error("Function not supported by the Apple runtime")))
#else
#    define OBJC_NONPORTABLE
#endif


