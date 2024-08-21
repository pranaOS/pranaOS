/**
 * @file commandline.cpp
 * @author Krisna Pranav
 * @brief commandline
 * @version 6.0
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/stdlib.h>
#include <kernel/commandline.h>

namespace Kernel 
{

    static char s_cmd_line[1024];
    static CommandLine* s_the;

    /**
     * @param cmd_line 
     */
    void CommandLine::early_initialize(const char* cmd_line)
    {
        if (!cmd_line)
            return;

        size_t length = strlen(cmd_line);

        if (length >= sizeof(s_cmd_line))
            length = sizeof(s_cmd_line) - 1;

        memcpy(s_cmd_line, cmd_line, length);
        s_cmd_line[length] = '\0';
    }

    /**
     * @return const CommandLine& 
     */
    const CommandLine& kernel_command_line()
    {
        ASSERT(s_the);
        return *s_the;
    }

    /// @brief: initialize cli.
    void CommandLine::initialize()
    {
        ASSERT(!s_the);
        s_the = new CommandLine(s_cmd_line);
    }

    /**
     * @param string 
     */
    CommandLine::CommandLine(const String& string)
        : m_string(string)
    {
        s_the = this;

        const auto& args = m_string.split(' ');
        m_params.ensure_capacity(args.size());

        for (auto&& str : args) {
            if (str == "") {
                continue;
            }

            auto pair = str.split_limit('=', 2);

            if (pair.size() == 1) {
                m_params.set(move(pair[0]), "");
            } else {
                m_params.set(move(pair[0]), move(pair[1]));
            }
        }
    }

    /**
     * @param key 
     * @return Optional<String> 
     */
    Optional<String> CommandLine::lookup(const String& key) const
    {
        return m_params.get(key);
    }

    /**
     * @param key 
     * @return true 
     * @return false 
     */
    bool CommandLine::contains(const String& key) const
    {
        return m_params.contains(key);
    }

} // namespace Kernel
