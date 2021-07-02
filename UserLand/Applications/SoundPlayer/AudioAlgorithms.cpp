// includes
#include "AudioAlgorithms.h"
#include <AK/Complex.h>
#include <math.h>


void fft(Vector<Complex<double>>& sample_data, bool invert)
{
    int n = sample_data.size();
    auto data = sample_data.data();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(data[i], data[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * M_PI / len * (invert ? -1 : 1);
        Complex<double> wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            Complex<double> w = { 1., 0. };
            for (int j = 0; j < len / 2; j++) {
                Complex<double> u = data[i + j], v = data[i + j + len / 2] * w;
                data[i + j] = u + v;
                data[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (int i = 0; i < n; i++)
            data[i] /= n;
    }
}
