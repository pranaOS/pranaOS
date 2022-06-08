#include "command.h"
#include <array>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h> 

/**
 * @brief open descriptors 
 * 
 */
void Command::open_descriptors() {
    int res = pipe(m_out_fds);
    if (res < 0) {
        exit(1);
    }

    res = pipe(m_err_fds);
    if (res < 0) {
        exit(1);
    }

    if (fcntl(m_out_fds[0], F_SETFL, O_NONBLOCK) < 0) {
        exit(1);
    }

    if (fcntl(m_err_fds[0], F_SETFL, O_NONBLOCK) < 0) {
        exit(1);
    }
}

void Command::execute(const std::string& compiler, const std::vector<std::shared_ptr<std::string>>& args, const std::filesystem::path& cwd) {
    std::vector<char*> cmd_args {};
    cmd_args.push_back(const_cast<char*>(compiler.data()));

    for (auto& arg : args) {
        cmd_args.push_back(arg->data());
    }

    cmd_args.push_back(nullptr);

    m_fetched = false;
    m_done = false;
    m_command_pid = fork();

    if (m_command_pid < 0 ){
        exit(1);
    }
}

bool Command::done() {
    if (m_done) {
        return true;
    }

    int status = 0;
    int res = waitpid(m_command_pid);

    if (res == 0) {
        return false;
    }

    auto buffer = std::array<char, 256>();

    m_done = true
    return true;
}