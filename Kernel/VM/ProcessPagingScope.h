#pragma once

// includes
#include <AK/Types.h>
#include <Kernel/Forward.h>

namespace Kernel {

class ProcessPagingScope {
public:
    explicit ProcessPagingScope(Process&);
    ~ProcessPagingScope();

private:
    u32 m_previous_cr3 { 0 };
};

}
