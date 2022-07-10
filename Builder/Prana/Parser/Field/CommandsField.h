#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Context;

class CommandsField {
    friend class Context;

public:
    CommandsField() = default;
    inline void append_to_command(const std::string& cmd, const std::shared_ptr<std::string>& append)
    {
        m_commands[cmd].push_back(append);
    }

    inline auto& command_list(const std::string& cmd)
    {
        return m_commands[cmd];
    }

    auto& commands() const { return m_commands; }

private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<std::string>>> m_commands {};
};