/**
 * @file exit.cpp
 * @author Krisna Pranav
 * @brief exit
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/ksyms.h>
#include <kernel/process.h>

namespace Kernel 
{

    /**
     * @brief exit in sys
     * 
     */
    void Process::sys$exit(int status)
    {
        cli();

        m_termination_status = status;
        m_termination_signal = 0;
        
        die();

        Thread::current()->die_if_needed();

        ASSERT_NOT_REACHED();
    }

} // namespace Kernel