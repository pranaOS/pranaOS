#pragma once

#include <ak/types.h>

namespace Kernel {
    struct cpuState {
        struct {
            ak::uint16_t GS;
            ak::uint16_t HGS;
        };

        struct {
            ak::uint16_t FS;
            ak::uint16_t HFS;
        };

        struct {
            ak::uint16_t ES;
            ak::uint16_t HES;
        };

        struct {
            ak::uint16_t DS;
            ak::uint16_t HDS;
        };

        struct {
            ak::uint32_t EDI;
            ak::uint16_t DI;
        };

        union {
            ak::uint32_t ESI;
            ak::uint16_t SI;
        };
    };
}