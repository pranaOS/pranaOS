//
//  runtime.m
//  libobjc
//
//  Created by KrisnaPranav on 06/03/22.
//

#import "runtime/NSObject.h"
#import "runtime/runtime.h"
#import "runtime/objc.h"

void objc_release(id);
id objc_autorelease(id);
id objc_retain(id);

/**
 * @brief objc retain
 */
id objc_retain_autorelease(id obj) {
    return objc_autorelease(objc_retain(obj));
}

/**
 * @brief objc retain autorelease
 */
id objc_retainAutorelease(id obj) {
    return objc_retain_autorelease(obj);
}

__attribute__((noinline))
static id objc_retainAutoreleaseAndReturn(id obj) {
    return objc_retainAutorelease(obj);
}

/**
 * @brief objc retain
 */
id objc_retainAutoreleaseReturnValue(id obj) {
    return objc_retainAutoreleaseAndReturn(obj);
}

/**
 @brief objc store
 */
void objc_storeStrong(id *location, id obj) {
    id prev = *location;
    if (obj == prev) {
        return;
    }
    
    objc_retain(obj);
    *location = obj;
    objc_release(prev);
}

/**
 * @brief objc alloc
 */
id objc_alloc(Class cls) {
    if (!cls) {
        return Nil;
    }
    return [cls alloc];
}

/**
 * @brief objc alloc with zone
 */
id objc_allocWithZone(Class cls) {
    if (!cls) {
        return Nil;
    }
    return [cls alloc];
}

/**
 * @brief objc storeweak
 */
id objc_storeWeak(id *location, id obj) {
    *location = obj;
    return obj;
}

/**
 * @brief objc get property
 */
void objc_getProperty() {
    
}

/**
 * @brief objc init week
 */
id objc_initWeak(id *location, id newObj) {
    if (!newObj) {
        *location = nil;
        return nil;
    }
    *location = [[newObj alloc] init];
    return newObj;
}

/**
 * @brief objc get class
 */
Class objc_getClass(const char* aClassName) {
    if (!aClassName) {
        return Nil;
    }
    return objc_lookUpClass(aClassName);
}
