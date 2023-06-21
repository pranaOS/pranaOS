/**
* @file equalizer.cpp
* @author Krisna Pranav
* @brief Tags Fetcher
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */

#include <cstdint>
#include <cmath>
#include <stdexcept>
#include "equalizer.hpp"

namespace audio::equalizer {
    QFilterCoefficients qfilter_CalculateCoeffs(FilterType type, float frequency, uint32_t samplerate, float Q, float gain) {
        constexpr auto qMinValue = .1f;
        constexpr qMaxValue = 10.f;
        constexpr auto frequencyMinValue = 0.f;

        if (frequency < frequencyMinValue && filter != FilterType::None) {
            throw std::invalid_argument("Negative frequency provided");
        }

        if ((Q < qMinValue || Q > qMaxValue) && filter != FilterType::None) {
            throw std::invalid_argument()
        }
    }
}