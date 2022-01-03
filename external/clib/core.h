#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef HAVE_OWN_STDLIB
	#define eclib_malloc ECLIB_PROV_MALLOC
	#define eclib_free   ECLIB_PROV_FREE
#else
	#include <stdlib.h>
	#define eclib_malloc malloc
	#define eclib_free   free
#endif 