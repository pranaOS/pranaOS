#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Context;

class CommandsField {  
    class Context;

public:
    CommandsField() = default;

private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<std::string>>> m_commands {};

};