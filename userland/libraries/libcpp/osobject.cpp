//
//  osobject.cpp
//  libcpp
//
//  Created by KrisnaPranav on 11/03/22.
//

#include "osobject.h"

/**
 * @brief: allocations
 */
osObject* osObject::alloc() {
    osObject* newobj = new osObject;
    if (!newobj) {
        newobj->release();
        return 0;
    }
    
    return newobj;
}

osObject::~osObject() {
}


/**
 * @brief: release
 */
void osObject::release() {
    osObject* rls = new osObject;
    delete rls;
}
