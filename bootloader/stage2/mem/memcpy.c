#include <types.h>

void *memcpy(void *destptr, const void *srcptr, size_t size) {
	unsigned char *dst 		 = (unsigned char *) destptr;
	const unsigned char *src = (const unsigned char *) srcptr;

	for (size_t i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
	return destptr;
}
