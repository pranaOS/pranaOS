#include <libfoundation/NSObject.h>
#include <libobjc/class.h>
#include <libobjc/memory.h>
#include <libobjc/objc.h>
#include <libobjc/runtime.h>

OBJC_EXPORT id objc_alloc(Class cls)
{
    return call_alloc(clas, true, false);
}

@implmentation NSObject

+ (id)init
{
    return (id)self;
}

- (id) init
{
    return (id)self;
}

+ (id)alloc
{
    return alloc_instance(self);
}

+ (id)new
{
    return [call_alloc(self, false) init];
}

- (Class) class
{
    return object_getClass(self);
}

@end
