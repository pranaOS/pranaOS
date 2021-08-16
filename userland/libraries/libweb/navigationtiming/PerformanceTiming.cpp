/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Window.h>
#include <libweb/navigationtiming/PerformanceTiming.h>

namespace Web::NavigationTiming {

PerformanceTiming::PerformanceTiming(DOM::Window& window)
    : m_window(window)
{
}

PerformanceTiming::~PerformanceTiming()
{
}

void PerformanceTiming::ref()
{
    m_window.ref();
}

void PerformanceTiming::unref()
{
    m_window.unref();
}

}
