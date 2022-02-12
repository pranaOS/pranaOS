#include <vsprintf.h>
#include <ak/cdefs.h>
#include <ak/ctype.h>
#include <ak/limits.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)

/**
 * @brief div
 * 
 */
#define do_div(n, base) ({                               \
	unsigned long __upper, __low, __high, __mod, __base; \
	__base = (base);                                     \
	asm(""                                               \
		: "=a"(__low), "=d"(__high)                      \
		: "A"(n));                                       \
	__upper = __high;                                    \
	if (__high)                                          \
	{                                                    \
		__upper = __high % (__base);                     \
		__high = __high / (__base);                      \
	}                                                    \
	asm("divl %2"                                        \
		: "=a"(__low), "=d"(__mod)                       \
		: "rm"(__base), "0"(__low), "1"(__upper));       \
	asm(""                                               \
		: "=A"(n)                                        \
		: "a"(__low), "d"(__high));                      \
	__mod;                                               \
})

/**
 * @brief simple strlen
 * 
 * @param s 
 * @param count 
 * @return size_t 
 */
size_t simple_strlen(const char *s, size_t count) {
    const char *sc;
    for (sc = s; count-- && *sc != '\0'; ++sc) 

    return sc - s;
}