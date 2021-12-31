#pragma once

#include <stdlib/runtime/ubsan.h>
#include <cstdio>

__BEGIN_DECLS
void __ubsan_set_output_file(void* ptr, FILE* file);
__END_DECLS