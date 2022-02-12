/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#include <vsprintf.h>
#include <ak/cdefs.h>
#include <ak/ctype.h>
#include <ak/limits.h>

/**
 * @brief page shift, page size
 * 
 */
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
 * @brief simple strnlen
 * 
 * @param s 
 * @param count 
 * @return size_t 
 */
size_t simple_strnlen(const char *s, size_t count) {
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
        /* ..*/;
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
		   (value = isdigit(*cp) ? *cp - '0' : toupper(*cp) - 'A' + 10) < base)
	{
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
	if (*cp == '-')
		return -simple_strtoul(cp + 1, endp, base);
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
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp - '0' : (islower(*cp) ? toupper(*cp) : *cp) - 'A' + 10) < base)
	{
		result = result * base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

/**
 * @brief simple strtoll
 * 
 * @param cp 
 * @param endp 
 * @param base 
 * @return long long 
 */
long long simple_strtoll(const char *cp, char **endp, unsigned int base) {
	if (*cp == '-')
		return -simple_strtoull(cp + 1, endp, base);
	return simple_strtoull(cp, endp, base);
}

/**
 * @brief skipatoi
 * 
 * @param s 
 * @return int 
 */
static int skip_atoi(const char **s) {
	int i = 0;

	while (isdigit(**s))
		i = i * 10 + *((*s)++) - '0';
	return i;
}

/**
 * @brief zeropad, sign, plus, space, left, special, large
 * 
 */
#define ZEROPAD 1  
#define SIGN 2	   
#define PLUS 4	   
#define SPACE 8	   
#define LEFT 16	   
#define SPECIAL 32 
#define LARGE 64   

/**
 * @brief number
 * 
 * @param buf 
 * @param end 
 * @param num 
 * @param base 
 * @param size 
 * @param precision 
 * @param type 
 * @return char* 
 */
static char *number(char *buf, char *end, unsigned long long num, int base, int size, int precision, int type) {
	char c, sign, tmp[66];
	const char *digits;
	static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	static const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	digits = (type & LARGE) ? large_digits : small_digits;
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return NULL;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN)
	{
		if ((signed long long)num < 0)
		{
			sign = '-';
			num = -(signed long long)num;
			size--;
		}
		else if (type & PLUS)
		{
			sign = '+';
			size--;
		}
		else if (type & SPACE)
		{
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL)
	{
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++] = '0';
	else
		while (num != 0)
			tmp[i++] = digits[do_div(num, base)];
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type & (ZEROPAD + LEFT)))
	{
		while (size-- > 0)
		{
			if (buf <= end)
				*buf = ' ';
			++buf;
		}
	}
	if (sign)
	{
		if (buf <= end)
			*buf = sign;
		++buf;
	}
	if (type & SPECIAL)
	{
		if (base == 8)
		{
			if (buf <= end)
				*buf = '0';
			++buf;
		}
		else if (base == 16)
		{
			if (buf <= end)
				*buf = '0';
			++buf;
			if (buf <= end)
				*buf = digits[33];
			++buf;
		}
	}
	if (!(type & LEFT))
	{
		while (size-- > 0)
		{
			if (buf <= end)
				*buf = c;
			++buf;
		}
	}
	while (i < precision--)
	{
		if (buf <= end)
			*buf = '0';
		++buf;
	}
	while (i-- > 0)
	{
		if (buf <= end)
			*buf = tmp[i];
		++buf;
	}
	while (size-- > 0)
	{
		if (buf <= end)
			*buf = ' ';
		++buf;
	}
	return buf;
}

/**
 * @brief vsnprintf
 * 
 * @param buf 
 * @param size 
 * @param fmt 
 * @param args 
 * @return int 
 */
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	int len;
	unsigned long long num;
	int i, base;
	char *str, *end, c;
	const char *s;

	int flags; 

	int field_width; 
	int precision;	 
				  
	int qualifier;	
					
	if (unlikely((int)size < 0))
	{
		return 0;
	}

	str = buf;
	end = buf + size - 1;

	if (end < buf - 1)
	{
		end = ((void *)-1);
		size = end - buf + 1;
	}

	for (; *fmt; ++fmt)
	{
		if (*fmt != '%')
		{
			if (str <= end)
				*str = *fmt;
			++str;
			continue;
		}

		flags = 0;
	repeat:
		++fmt; 
		switch (*fmt)
		{
		case '-':
			flags |= LEFT;
			goto repeat;
		case '+':
			flags |= PLUS;
			goto repeat;
		case ' ':
			flags |= SPACE;
			goto repeat;
		case '#':
			flags |= SPECIAL;
			goto repeat;
		case '0':
			flags |= ZEROPAD;
			goto repeat;
		}

		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*')
		{
			++fmt;
			field_width = va_arg(args, int);
			if (field_width < 0)
			{
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		precision = -1;
		if (*fmt == '.')
		{
			++fmt;
			if (isdigit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*')
			{
				++fmt;
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
			*fmt == 'Z' || *fmt == 'z')
		{
			qualifier = *fmt;
			++fmt;
			if (qualifier == 'l' && *fmt == 'l')
			{
				qualifier = 'L';
				++fmt;
			}
		}

		base = 10;

		switch (*fmt)
		{
		case 'c':
			if (!(flags & LEFT))
			{
				while (--field_width > 0)
				{
					if (str <= end)
						*str = ' ';
					++str;
				}
			}
			c = (unsigned char)va_arg(args, int);
			if (str <= end)
				*str = c;
			++str;
			while (--field_width > 0)
			{
				if (str <= end)
					*str = ' ';
				++str;
			}
			continue;

		case 's':
			s = va_arg(args, char *);
			if ((unsigned long)s < PAGE_SIZE)
				s = "<NULL>";

			len = simple_strnlen(s, precision);

			if (!(flags & LEFT))
			{
				while (len < field_width--)
				{
					if (str <= end)
						*str = ' ';
					++str;
				}
			}
			for (i = 0; i < len; ++i)
			{
				if (str <= end)
					*str = *s;
				++str;
				++s;
			}
			while (len < field_width--)
			{
				if (str <= end)
					*str = ' ';
				++str;
			}
			continue;

		case 'p':
			if (field_width == -1)
			{
				field_width = 2 * sizeof(void *);
				flags |= ZEROPAD;
			}
			str = number(str, end,
						 (unsigned long)va_arg(args, void *),
						 16, field_width, precision, flags);
			continue;

		case 'n':
			if (qualifier == 'l')
			{
				long *ip = va_arg(args, long *);
				*ip = (str - buf);
			}
			else if (qualifier == 'Z' || qualifier == 'z')
			{
				size_t *ip = va_arg(args, size_t *);
				*ip = (str - buf);
			}
			else
			{
				int *ip = va_arg(args, int *);
				*ip = (str - buf);
			}
			continue;

		case '%':
			if (str <= end)
				*str = '%';
			++str;
			continue;

		case 'o':
			base = 8;
			break;

		case 'X':
			flags |= LARGE;
		case 'x':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
			if (str <= end)
				*str = '%';
			++str;
			if (*fmt)
			{
				if (str <= end)
					*str = *fmt;
				++str;
			}
			else
			{
				--fmt;
			}
			continue;
		}
		if (qualifier == 'L')
			num = va_arg(args, long long);
		else if (qualifier == 'l')
		{
			num = va_arg(args, unsigned long);
			if (flags & SIGN)
				num = (signed long)num;
		}
		else if (qualifier == 'Z' || qualifier == 'z')
		{
			num = va_arg(args, size_t);
		}
		else if (qualifier == 'h')
		{
			num = (unsigned short)va_arg(args, int);
			if (flags & SIGN)
				num = (signed short)num;
		}
		else
		{
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int)num;
		}
		str = number(str, end, num, base,
					 field_width, precision, flags);
	}
	if (str <= end)
		*str = '\0';
	else if (size > 0)
		*end = '\0';
	return str - buf;
}

/**
 * @brief vscnprintf
 * 
 * @param buf 
 * @param size 
 * @param fmt 
 * @param args 
 * @return int 
 */
int vscnprintf(char *buf, size_t size, const char *fmt, va_list args) {
	int i;

	i = vsnprintf(buf, size, fmt, args);
	return (i >= size) ? (size - 1) : i;
}

/**
 * @brief snprintf
 * 
 * @param buf 
 * @param size 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int snprintf(char *buf, size_t size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsnprintf(buf, size, fmt, args);
	va_end(args);
	return i;
}

/**
 * @brief scanprintf
 * 
 * @param buf 
 * @param size 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int scnprintf(char *buf, size_t size, const char *fmt, ...) {
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsnprintf(buf, size, fmt, args);
	va_end(args);
	return (i >= size) ? (size - 1) : i;
}

/**
 * @brief vsprintf
 * 
 * @param buf 
 * @param fmt 
 * @param args 
 * @return int 
 */
int vsprintf(char *buf, const char *fmt, va_list args) {
	return vsnprintf(buf, INT_MAX, fmt, args);
}

/**
 * @brief sprintf
 * 
 * @param buf 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsnprintf(buf, INT_MAX, fmt, args);
	va_end(args);
	return i;
}

/**
 * @brief vsscanf
 * 
 * @param buf 
 * @param fmt 
 * @param args 
 * @return int 
 */
int vsscanf(const char *buf, const char *fmt, va_list args)
{
	const char *str = buf;
	char *next;
	char digit;
	int num = 0;
	int qualifier;
	int base;
	int field_width;
	int is_sign = 0;

	while (*fmt && *str)
	{
		if (isspace(*fmt))
		{
			while (isspace(*fmt))
				++fmt;
			while (isspace(*str))
				++str;
		}

		if (*fmt != '%' && *fmt)
		{
			if (*fmt++ != *str++)
				break;
			continue;
		}

		if (!*fmt)
			break;
		++fmt;

		if (*fmt == '*')
		{
			while (!isspace(*fmt) && *fmt)
				fmt++;
			while (!isspace(*str) && *str)
				str++;
			continue;
		}

		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);

		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
			*fmt == 'Z' || *fmt == 'z')
		{
			qualifier = *fmt++;
			if (unlikely(qualifier == *fmt))
			{
				if (qualifier == 'h')
				{
					qualifier = 'H';
					fmt++;
				}
				else if (qualifier == 'l')
				{
					qualifier = 'L';
					fmt++;
				}
			}
		}
		base = 10;
		is_sign = 0;

		if (!*fmt || !*str)
			break;

		switch (*fmt++)
		{
		case 'c':
		{
			char *s = (char *)va_arg(args, char *);
			if (field_width == -1)
				field_width = 1;
			do
			{
				*s++ = *str++;
			} while (--field_width > 0 && *str);
			num++;
		}
			continue;
		case 's':
		{
			char *s = (char *)va_arg(args, char *);
			if (field_width == -1)
				field_width = INT_MAX;
			while (isspace(*str))
				str++;

			while (*str && !isspace(*str) && field_width--)
			{
				*s++ = *str++;
			}
			*s = '\0';
			num++;
		}
			continue;
		case 'n':
			{
				int *i = (int *)va_arg(args, int *);
				*i = str - buf;
			}
			continue;
		case 'o':
			base = 8;
			break;
		case 'x':
		case 'X':
			base = 16;
			break;
		case 'i':
			base = 0;
		case 'd':
			is_sign = 1;
		case 'u':
			break;
		case '%':
			if (*str++ != '%')
				return num;
			continue;
		default:
			return num;
		}

		while (isspace(*str))
			str++;

		digit = *str;
		if (is_sign && digit == '-')
			digit = *(str + 1);

		if (!digit || (base == 16 && !isxdigit(digit)) || (base == 10 && !isdigit(digit)) || (base == 8 && (!isdigit(digit) || digit > '7')) || (base == 0 && !isdigit(digit)))
			break;

		switch (qualifier)
		{
		case 'H': 
			if (is_sign)
			{
				signed char *s = (signed char *)va_arg(args, signed char *);
				*s = (signed char)simple_strtol(str, &next, base);
			}
			else
			{
				unsigned char *s = (unsigned char *)va_arg(args, unsigned char *);
				*s = (unsigned char)simple_strtoul(str, &next, base);
			}
			break;
		case 'h':
			if (is_sign)
			{
				short *s = (short *)va_arg(args, short *);
				*s = (short)simple_strtol(str, &next, base);
			}
			else
			{
				unsigned short *s = (unsigned short *)va_arg(args, unsigned short *);
				*s = (unsigned short)simple_strtoul(str, &next, base);
			}
			break;
		case 'l':
			if (is_sign)
			{
				long *l = (long *)va_arg(args, long *);
				*l = simple_strtol(str, &next, base);
			}
			else
			{
				unsigned long *l = (unsigned long *)va_arg(args, unsigned long *);
				*l = simple_strtoul(str, &next, base);
			}
			break;
		case 'L':
			if (is_sign)
			{
				long long *l = (long long *)va_arg(args, long long *);
				*l = simple_strtoll(str, &next, base);
			}
			else
			{
				unsigned long long *l = (unsigned long long *)va_arg(args, unsigned long long *);
				*l = simple_strtoull(str, &next, base);
			}
			break;
		case 'Z':
		case 'z':
		{
			size_t *s = (size_t *)va_arg(args, size_t *);
			*s = (size_t)simple_strtoul(str, &next, base);
		}
		break;
		default:
			if (is_sign)
			{
				int *i = (int *)va_arg(args, int *);
				*i = (int)simple_strtol(str, &next, base);
			}
			else
			{
				unsigned int *i = (unsigned int *)va_arg(args, unsigned int *);
				*i = (unsigned int)simple_strtoul(str, &next, base);
			}
			break;
		}
		num++;

		if (!next)
			break;
		str = next;
	}
	return num;
}

/**
 * @brief sscanf
 * 
 * @param buf 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int sscanf(const char *buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsscanf(buf, fmt, args);
	va_end(args);
	return i;
}