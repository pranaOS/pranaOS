/**
 * @file regs.h
 * @author Krisna Pranav
 * @brief regs
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

#define RREGISTER(num)                         \
    union {                                    \
        u64 r##num;                            \
        struct {                               \
            u32 _unused##num;                  \
            union {                            \
                u32 r##num##d;                 \
                struct {                       \
                    u16 __unused##num;         \
                    union {                    \
                        u16 r##num##w;         \
                        struct {               \
                            u8 ___unused##num; \
                            u8 r##num##b;      \
                        };                     \
                    };                         \
                };                             \
            };                                 \
        };                                     \
    }

#define GPREGISTER(letter)                \
    union {                               \
        u64 r##letter##x;                 \
        struct                            \
        {                                 \
            u32 _unused##letter;          \
            union {                       \
                u32 e##letter##x;         \
                struct                    \
                {                         \
                    u16 __unused##letter; \
                    union {               \
                        u16 letter##x;    \
                        struct {          \
                            u8 letter##h; \
                            u8 letter##l; \
                        };                \
                    };                    \
                };                        \
            };                            \
        };                                \
    }

#define SPREGISTER(name)                        \
    union {                                     \
        u64 r##name;                            \
        struct                                  \
        {                                       \
            u32 _unused##name;                  \
            union {                             \
                u32 e##name;                    \
                struct                          \
                {                               \
                    u16 __unused##name;         \
                    union {                     \
                        u16 name;               \
                        struct {                \
                            u8 ___unused##name; \
                            u8 name##l;         \
                        };                      \
                    };                          \
                };                              \
            };                                  \
        };                                      \
    }

struct [[gnu::packed]] PtraceRegisters {
    GPREGISTER(a);
    GPREGISTER(b);
    GPREGISTER(c);
    GPREGISTER(d);

    SPREGISTER(sp);
    SPREGISTER(bp);
    SPREGISTER(si);
    SPREGISTER(di);
    SPREGISTER(ip); 

    RREGISTER(8);
    RREGISTER(9);
    RREGISTER(10);
    RREGISTER(11);
    RREGISTER(12);
    RREGISTER(13);
    RREGISTER(14);
    RREGISTER(15);
    
    union {
        u64 rflags;
        struct {
            u32 _;
            union {
                u32 eflags;
                struct {
                    u16 __;
                    u16 flags;
                };
            };
        };
    };

    u32 cs;
    u32 ss;
    u32 ds;
    u32 es;
    u32 fs;
    u32 gs;
};
