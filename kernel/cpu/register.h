#pragma once

#include <ak/types.h>

namespace Kernel {
    struct CPUState {
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

        union {
            ak::uint32_t EDI;
            ak::uint16_t DI;
        };

        union {
            ak::uint32_t ESI;
            ak::uint16_t SI;
        };

        union {
            ak::uint32_t EBP;
            ak::uint16_t BP;
        };

        union {
            ak::uint32_t ESP;
            ak::uint16_t SP;
        };

        union {
            ak::uint32_t EBX;
            ak::uint16_t BX;

            struct {
                 ak::uint8_t BL;
                ak::uint8_t BH;
            };
        };

        union {
            ak::uint32_t EDX;
            ak::uint16_t DX;

            struct {
                ak::uint8_t DL;
                ak::uint8_t DH;
            };

        };

        union {
            ak::uint32_t ECX;
            ak::uint16_t CX;

            struct {
                ak::uint8_t CL;
                ak::uint8_t CH;
            };
        };

        union {
            ak::uint32_t EAX;
            ak::uint16_t AX;
            struct {
                ak::uint8_t AL;
                ak::uint8_t AH;
            };
        };

        ak::uint32_t interruptNumber;
        ak::uint32_t errorCode;

        union {
            ak::uint32_t EIP;
            ak::uint16_t IP;
        };

        struct {
            ak::uint16_t CS;
            ak::uint16_t HCS;
        };

        union {
            ak::uint32_t EFLAGS;
            ak::uint16_t FLAGS;
        };

        union {
            ak::uint32_t userESP;
            ak::uint16_t userSP;
        };

        union {
            ak::uint16_t userSS;
            ak::uint16_t huserSS;
        };

    } __attribute__((packed));
}