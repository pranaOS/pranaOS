#pragma once

#include <memory>
#include <string>
#include <vector>
#include <filesystem>

enum class Operation {
    Compile,
    Link,
    Archive,
};

class Context;

struct ExecUnit {
    Operation op {};
    Context* ctx {};
    std::shared_ptr<std::string> callee {};
    std::string src {};
    std::shared_ptr<std::string> binary;
    std::vector<std::shared_ptr<std::string>> args {};
    std::filesystem::path cwd {};
};