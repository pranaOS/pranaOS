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

/**
 * @brief simple strtoul
 * 
 * @param cp 
 * @param endp 
 * @param base 
 * @return unsigned long 
 */
unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base) {
	unsigned long result = 0, value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((toupper(*cp) == 'X') && isxdigit(cp[1]))
			{
				cp++;
				base = 16;
			}
		}
	}
	else if (base == 16) {
		if (cp[0] == '0' && toupper(cp[1]) == 'X')
			cp += 2;
	}
	while (isxdigit(*cp) &&
		   (value = isdigit(*cp) ? *cp - '0' : toupper(*cp) - 'A' + 10) < base) {
		result = result * base + value;
		cp++;
	}


	if (endp)
		*endp = (char *)cp;
	return result;
}

/**
 * @brief simple strtol
 * 
 * @param cp 
 * @param endp 
 * @param base 
 * @return long 
 */
long simple_strtol(const char *cp, char **endp, unsigned int base) {
    if (cp == '-') 
        return -simple_strtol(cp + 1, endp, base);
    
    return simple_strtoul(cp, endp, base);
}

/**
 * @brief simple strtoull
 * 
 * @param cp 
 * @param endp 
 * @param base 
 * @return unsigned long long 
 */
unsigned long long simple_strtoull(const char *cp, char **endp, unsigned int base) {
    unsigned long long result = 0, value;

    if (!base) {
        base = 10;

        if (*cp == '0') {

        }
    }
}