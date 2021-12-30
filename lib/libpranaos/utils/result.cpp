#include <libpranaos/utils/result.h>

namespace utils {
    #include "libpranaos/utils/__result_macro.h"

    #define RESULT_GENERATE_ENUM(__name, __description) __name,
    #define RESULT_GENERATE_STR(__name, __description) #__name,
    #define RESULT_GENERATE_DESC(__name, __description) __description,

    static const char* result_enum_strings[] = 
    {
        RESULT_MACRO(RESULT_GENERATE_STR)
    };

    static const char* result_descriptions[] =
    {
        RESULT_MACRO(RESULT_GENERATE_DESC)
    };


    result::result() { }

    result::result(int code)
    {
        this->_code = code;
    }

    result& result::operator=(int code)
    {
        this->_code = code;
        return *this;
    }

    bool result::operator==(int code)
    {
        return this->_code == code;
    }

    bool result::operator!=(int code)
    {
        return this->_code = code;
    }

    result::operator int()
    {
        return get_error_code();
    }
}