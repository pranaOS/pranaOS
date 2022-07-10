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

public:
    bool fetched() const { return m_fetched; }
    void fetch() { m_fetched = true; }
    int exit_status() const { return m_exit_status; }

    std::string& std_out() { return m_std_out; }
    const std::string& std_out() const { return m_std_out; }

    std::string& std_err() { return m_std_err; }
    const std::string& std_err() const { return m_std_err; }

    auto executable_unit() { return m_executable_unit; }
    void set_executable_unit(const std::shared_ptr<ExecutableUnit>& unit) { m_executable_unit = unit; }

private:
    std::shared_ptr<ExecUnit> m_executable_unit {};
    int m_command_pid { -1 };
    bool m_done { true };
    bool m_fetched { true };
    int8_t m_exit_status {};

    int m_out_fds[2] {};
    int m_err_fds[2] {};
    constexpr static auto read_ptr = 0, write_ptr = 1;

    std::string m_std_out {};
    std::string m_std_err {};
};