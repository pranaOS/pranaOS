/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/types.h>

namespace Kernel {
    struct CPUState {
        struct {
            uint16_t GS;
            uint16_t HGS;
        };

        struct {
            uint16_t FS;
            uint16_t HFS;
        };

        struct {
            uint16_t ES;
            uint16_t HES;
        };

        struct {
            uint16_t DS;
            uint16_t HDS;
        };

        union {
            uint32_t EDI;
            uint16_t DI;
        };

        union {
            uint32_t ESI;
            uint16_t SI;
        };

        union {
            uint32_t EBP;
            uint16_t BP;
        };

        union {
            uint32_t ESP;
            uint16_t SP;
        };

        union {
            uint32_t EBX;
            uint16_t BX;

            struct {
                 uint8_t BL;
                uint8_t BH;
            };
        };

        union {
            uint32_t EDX;
            uint16_t DX;

            struct {
                uint8_t DL;
                uint8_t DH;
            };

        };

        union {
            uint32_t ECX;
            uint16_t CX;

            struct {
                uint8_t CL;
                uint8_t CH;
            };
        };

        union {
            uint32_t EAX;
            uint16_t AX;
            struct {
                uint8_t AL;
                uint8_t AH;
            };
        };

        uint32_t interruptNumber;
        uint32_t errorCode;

        union {
            uint32_t EIP;
            uint16_t IP;
        };

        struct {
            uint16_t CS;
            uint16_t HCS;
        };

        union {
            uint32_t EFLAGS;
            uint16_t FLAGS;
        };

        union {
            uint32_t userESP;
            uint16_t userSP;
        };

        union {
            uint16_t userSS;
            uint16_t huserSS;
        };

    } __attribute__((packed));
}