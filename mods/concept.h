/**
 * @file concept.h
 * @author Patrick, pranaOS-developers
 * @brief Concept
 * @version 6.0
 * 
 * @copyright Copyright (c) 2021 - 2023 Patrick, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "stdlibextra.h"

namespace Mods::Concepts {

    #if defined(__cpp_concepts) && !defined(__COVERITY__)
    template<typename T>
    concept Integral = IsIntegral<T>::value;

    template<typename T>
    concept FloatingPoint = IsFloatingPoint<T>::value;

    #endif

}

#if defined(__cpp_concepts) && !defined(__COVERITY__)

using Mods::IsFloatingPoint;
using Mods::IsIntegral;

#endif
