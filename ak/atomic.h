#pragma once

/**
 * @brief counter volatiles atomic struct
 * 
 */
typedef struct {
	volatile int counter;
} atomic_t;

/**
 * @brief atomic init define block
 * 
 */
#define ATOMIC_INIT(i) \
	{                  \
		(i)            \
	}

/**
 * @brief atomic read and set
 * @returns v
 */
#define atomic_read(v) ((v)->counter)
#define atomic_set(v, i) (((v)->counter) = (i))

/**
 * @brief add atomic values
 * 
 * @param i 
 * @param v 
 */
static inline void atomic_add(int i, atomic_t *v) {
	__asm__ __volatile__(
		"addl %1,%0"
		: "=m"(v->counter)
		: "ir"(i), "m"(v->counter)
    );
}

/**
 * @brief atomic sub
 * 
 * @param i 
 * @param v 
 */
static void atomic_sub(int i, atomic_t *v) {
	__asm__ __volatile__(
		"subl %1,%0"
		: "=m"(v->counter)
		: "ir"(i), "m"(v->counter)
    );
}

/**
 * @brief atomic increase
 * 
 * @param v 
 */
static inline void atomic_inc(atomic_t *v) {
	__asm__ __volatile__(
		"incl %0"
		: "=m"(v->counter)
		: "m"(v->counter)
    );
}

/**
 * @brief atomic decrease
 * 
 * @param v 
 */
static inline void atomic_dec(atomic_t *v) {
	__asm__ __volatile__(
		"decl %0"
		: "=m"(v->counter)
		: "m"(v->counter));
}