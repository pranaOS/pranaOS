#pragma once

// includes
#include <AK/Complex.h>
#include <AK/Vector.h>

void fft(Vector<Complex<double>>& sample_data, bool invert);
