//
//  osobject.cpp
//  libcpp
//
//  Created by KrisnaPranav on 11/03/22.
//

#include "osobject.h"

namespace OsObject {
    class object {
    public:
        /**
         * @brief list append
         */
        void listappend(list_t* list, delement_t* elem) {
            elem->next = 0;
            list_t headnew;
        }
        
    protected:
        ~object();
        object();
    };
}
