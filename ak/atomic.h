#pragma once

typedef struct {
    int counter;
} atomic_t;

static inline void atomic_add(int i, atomic_t* v) {
    __asm__ volatile(
        "addl %1 %0"
        : "=m"(v->counter)
        : "ir"(i), "m"(v->counter)
    );
}