#pragma once

namespace utils {
    class result
    {
    private:
        int _code = ERR_UNKNOWN;

    public:
        #include "libpranaos/utils/__result_macro.h"
        #define RESULT_GENERATE_ENUM(__name, __description) __name,

        enum
        {
            RESULT_MACRO(RESULT_GENERATE_ENUM)
        };

        #undef GENERATE_ENUM
        #undef GENERATE_DESC
        #undef RESULT_MACRO

        result();
        result(int code);

        result& operator=(int code);
        bool operator==(int code);
        bool operator!=(int code);

        explicit operator int();
        operator bool();

        const char* to_string();
        const char* get_description();

        int get_error_code();
    };
}