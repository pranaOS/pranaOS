/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Exception.h>
#include <libjs/runtime/VM.h>

namespace JS {

class TemporaryClearException {
public:
    explicit TemporaryClearException(VM& vm)
        : m_vm(vm)
        , m_previous_exception(vm.exception())
    {
        m_vm.clear_exception();
    }

    ~TemporaryClearException()
    {
        if (m_previous_exception)
            m_vm.set_exception(*m_previous_exception);
    }

private:
    VM& m_vm;
    Exception* m_previous_exception;
};

}
