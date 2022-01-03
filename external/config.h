#pragma once

#define HAVE_OWN_STDLIB 1
#define HAVE_OWN_STRING 1

#define ECLIB_PROV_MALLOC kmalloc
#define ECLIB_PROV_FREE   kfree
#define ECLIB_PROV_MEMSET memset
#define ECLIB_PROV_MEMCPY memcpy