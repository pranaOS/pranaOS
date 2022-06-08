#pragma once

#include "execunit.h"
#include <memory>
#include <string>
#include <unistd.h>
#include <filesystem>

class Command {
public:
    Command() = default;
    void open_descriptors();

public:
    void execute(const std::string& compiler, const std::vector<std::shared_ptr<std::string>>& args, const std::filesystem::path& cwd);
    bool done();

}