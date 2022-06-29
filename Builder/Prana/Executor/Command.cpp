#include "command.h"
#include <array>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h> 

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
    m_exit_status = 0;
    m_std_out.clear();
    m_std_err.clear();

    m_command_pid = fork();
    if (m_command_pid < 0) {
        exit(1);
    }

    if (m_command_pid == 0) {
        if (dup2(m_out_fds[write_ptr], STDOUT_FILENO) < 0) {
            exit(1);
        }
        if (dup2(m_err_fds[write_ptr], STDERR_FILENO) < 0) {
            exit(1);
        }
        std::filesystem::current_path(cwd);
        execvp(cmd_args[0], cmd_args.data());
    }
}

bool Command::done() {
    if (m_done) {
        return true;
    }

    int status = 0;
    int res = waitpid(m_command_pid, &status, WNOHANG);

    if (res == 0) {
        return false;
    }

    if (res < 0) {
        m_exit_status = errno;
        m_done = true;
        return true;
    }

    if (WIFEXITED(status)) {
        m_exit_status = WEXITSTATUS(status);
    }

    auto buffer = std::array<char, 256>();

    while (true) {
        int out_bytes = read(m_out_fds[read_ptr], buffer.data(), buffer.size());
        if (out_bytes <= 0) {
            break;
        }
        m_std_out.append(buffer.data(), out_bytes);
    }

    while (true) {
        int err_bytes = read(m_err_fds[read_ptr], buffer.data(), buffer.size());
        if (err_bytes <= 0) {
            break;
        }
        m_std_err.append(buffer.data(), err_bytes);
    }

    m_done = true;
    return true;
}