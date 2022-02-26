//
//  osruntime.h
//  libcpp
//
//  Created by KrisnaPranav on 26/02/22.
//

#ifndef osruntime_h_
#define osruntime_h_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

class osRuntime {
public:
    static void* osMalloc(size_t size);
    static void osFree(void* addr);
};

typedef int osReturn;

#endif /* osruntime_h */
