#pragma once

#if defined(KERNEL)
#       include <libkern/assertions.h>
#else
#       include <assert.h>
#       define VERIFY assert

static constexpr bool TODO = false;
#       define TODO() VERIFY(TODO)

#endif 