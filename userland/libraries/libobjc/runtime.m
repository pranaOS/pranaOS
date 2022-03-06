//
//  runtime.m
//  libobjc
//
//  Created by KrisnaPranav on 06/03/22.
//

#include "runtime/NSObject.h"
#include "runtime/runtime.h"
#include "runtime/objc.h"

void objc_release(id);

id objc_autorelease(id);
id objc_retain(id);

/**
 * @brief objc autorelease
 */
id objc_retain_autorelease(id obj) {
    return objc_autorelease(objc_retain(obj));
}

/**
 * @brief retain autorelease
 */
id objc_retainAutorelease(id obj) {
    return objc_retain_autorelease(obj);
}

__attribute__((noinline))
static id objc_retainAutoreleaseandReturn(id obj) {
    return objc_retainAutorelease(obj);
}


/**
 * @brief: retian autorelease and returns value
 */
id objc_retainAutoreleaseReturnvalue(id obj) {
    return objc_retainAutoreleaseandReturn(obj);
}
