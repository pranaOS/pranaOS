/*
 * Copyright (c) 2021, Krisna Pranav
 * Copyright (c) 2014 - 2021 Apple Inc. and the Swift project authors
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <stdio.h>

bool EnableOverride;
bool Ran;

class CompatibilityOverrideRuntimeTest
{
public:
    virtual void setup() {
        EnableOverride = true;
        Ran = false;
    }

    virtual void Down() {
        EnableOverride = false;
    }
};