#include <source_location>

namespace utils {
    void assert_if_reached(const char* message = "reached unreachable code", std::source_location source = std::source_location::current());
    void assert_if_equals_true(bool condition, const char* message = "equals true", std::source_location source = std::source_location::current());
}