#pragma once

// includes
#include <Kernel/VM/MappedROM.h>

namespace Kernel {

MappedROM map_bios();
MappedROM map_ebda();

}
