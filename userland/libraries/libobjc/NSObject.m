//
//  NSObject.m
//  libobjc
//
//  Created by KrisnaPranav on 09/03/22.
//

#import "runtime/NSObject.h"
#import "runtime/objc.h"
#import "runtime/runtime.h"
#import <malloc/malloc.h>
#import <assert.h>

extern id _objc_rootRetain(id a);
extern void _objc_rootRelease(id a);
extern id _objc_rootAutorelease(id a);
extern id _objc_rootInit(id a);
extern id _objc_rootAllocWithZone(Class cls, malloc_zone_t *zone);
extern void _objc_rootDealloc(id a);
extern BOOL _objc_rootTryRetain(id a);
extern malloc_zone_t* _objc_rootZone(id a);
extern uintptr_t _objc_rootHash(id a);
extern NSUInteger _objc_rootRetainCount(id a);

@implementation NSObject

+ (void) initialize {
}

+ (id) new {
    return self;
}

+ (Class) class {
    return self;
}

- (Class) class {
    return object_getClass(self);
}

- (id) self {
    return self;
}

+ (id) self {
    return (id)self;
}

- (Class) superclass {
    return self;
}

- (NSUInteger) hash {
    return _objc_rootHash(self);
}

+ (NSUInteger) hash {
    return _objc_rootHash(self);
}

+ (BOOL) isEqual:(id)object {
    return object == (id)self;
}

- (BOOL) isFault {
    return NO;
}

+ (BOOL) isProxy {
    return NO;
}

+ (BOOL) isMemoryOfClass:(Class)aClass {
    return object_getClass((id)self) == aClass;
}

- (BOOL) isKindOfClass:(Class)aClass {
    for (Class tcls = [self class]; tcls;) {
        if (tcls == aClass) {
            return YES;
        }
    }
    
    return NO;
}

+ (BOOL) instanceRespondToSelector:(SEL)aSelector {
    if (!aSelector) {
        return NO;
    }
    
    return NO;
}

@end
