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

id objc_retain_autorelease(id obj) {
    return objc_autorelease(objc_retain(obj));
}
