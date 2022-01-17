#pragma once

#include <ak/types.h>

namespace Kernel {
    struct CPUState {
        struct {
            common::uint16_t GS;
            common::uint16_t HGS;
        };

        struct {
            common::uint16_t FS;
            common::uint16_t HFS;
        };

        struct {
            common::uint16_t ES;
            common::uint16_t HES;
        };

        struct {
            common::uint16_t DS;
            common::uint16_t HDS;
        };

        union {
            common::uint32_t EDI;
            common::uint16_t DI;
        };

        union {
            common::uint32_t ESI;
            common::uint16_t SI;
        };

        union {
            common::uint32_t EBP;
            common::uint16_t BP;
        };

        union {
            common::uint32_t ESP;
            common::uint16_t SP;
        };

        union {
            common::uint32_t EBX;
            common::uint16_t BX;

            struct {
                 common::uint8_t BL;
                common::uint8_t BH;
            };
        };

        union {
            common::uint32_t EDX;
            common::uint16_t DX;

            struct {
                common::uint8_t DL;
                common::uint8_t DH;
            };

        };

        union {
            common::uint32_t ECX;
            common::uint16_t CX;

            struct {
                common::uint8_t CL;
                common::uint8_t CH;
            };
        };

        union {
            common::uint32_t EAX;
            common::uint16_t AX;
            struct {
                common::uint8_t AL;
                common::uint8_t AH;
            };
        };

        common::uint32_t interruptNumber;
        common::uint32_t errorCode;

        union {
            common::uint32_t EIP;
            common::uint16_t IP;
        };

        struct {
            common::uint16_t CS;
            common::uint16_t HCS;
        };

        union {
            common::uint32_t EFLAGS;
            common::uint16_t FLAGS;
        };

        union {
            common::uint32_t userESP;
            common::uint16_t userSP;
        };

        union {
            common::uint16_t userSS;
            common::uint16_t huserSS;
        };

    } __attribute__((packed));
}