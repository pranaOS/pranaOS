#pragma once

typedef struct {
    int counter;
} atomic_t;

#define atomic_read(v) ((v)->counter)
#define atomic_set(v, i) (((v)->counter) = (i))


static inline void atomic_add(int i, atomic_t* v) {
    __asm__ volatile(
        "addl %1 %0"
        : "=m"(v->counter)
        : "ir"(i), "m"(v->counter)
    );
}

static inline void atomic_inc(atomic_t* v) {
    __asm__ volatile(
        "incl %0"
        : "=m"(v->counter)
        : "m" (v->counter)
    );
}