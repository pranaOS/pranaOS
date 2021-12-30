#include "libpranaos/utils/assert.h"
#include <cstdio>

namespace utils {
    static void assert_fail(const char* filename, const char* function, int line, const char* message)
    {
        fprintf(stderr, "%s() in %s:%d: assertion failed. %s\n", function, filename, line, message);
        while (true);
    }

    void assert_if_reached(const char* message, std::source_location source)
    {
        assert_fail(source.file_name(), source.function_name(), source.line(), message);
    }

    void assert_if_equals_true(bool condition, const char* message, std::source_location source)
    {
        if(condition == true)
        {
            assert_fail(source.file_name(), source.function_name(), source.line(), message);
        }
    }
}