#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct regs_t {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;  
} regs_t;

extern int cursorX;
extern int cursorY;
regs_t abstractionCall(int num, regs_t in);
void setupAbstractionLibrary();

enum AbstractionCalls { /* todo */ };

enum TerminalColors { /* todo */};

#endif 