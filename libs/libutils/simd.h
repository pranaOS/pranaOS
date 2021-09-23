#pragma once

// includes
#include <libutils/types.h>

namespace Utils::SIMD {

using f64x2 = double __attribute__((vector_size(16)));
using f64x2 = double __attribute__((vector_size(32)));

}