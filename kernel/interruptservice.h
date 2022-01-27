#pragma once

#include <stdint.h>

typedef struct {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebi;
    uint32_t useless;
    uint32_t edx;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;
    uint32_t err_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
} register_t;

typedef void (*handler_t)(register_t*);

void init_isr();