/**
 * @file getpassword.cpp
 * @author Krisna Pranav
 * @brief get password
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <libcore/getpassword.h>

namespace Core 
{
    /**
     * @param prompt 
     * @return Result<String, int> 
     */
    Result<String, int> get_password(const StringView& prompt)
    {
        fwrite(prompt.characters_without_null_termination(), sizeof(char), prompt.length(), stdout);
        fflush(stdout);

        struct termios original;
        tcgetattr(STDIN_FILENO, &original);

        struct termios no_echo = original;
        no_echo.c_lflag &= ~ECHO;

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &no_echo) < 0) {
            return errno;
        }

        char* password = nullptr;

        size_t n = 0;

        int ret = getline(&password, &n, stdin);

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);

        putchar('\n');

        if (ret < 0) {
            return errno;
        }

        String s(password);

        free(password);

        return s;
    } // Result<String, int> get_password
} // namespace Core