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

#include <libcore/getpassword.h>
#include <libcore/system.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

namespace Core 
{
    /**
     * @brief Get the password object
     * 
     * @param prompt 
     * @return ErrorOr<SecretString> 
     */
    ErrorOr<SecretString> get_password(StringView prompt)
    {
        TRY(Core::System::write(STDOUT_FILENO, prompt.bytes()));

        auto original = TRY(Core::System::tcgetattr(STDIN_FILENO));

        termios no_echo = original;
        no_echo.c_lflag &= ~ECHO;

        TRY(Core::System::tcsetattr(STDIN_FILENO, TCSAFLUSH, no_echo));

        char* password = nullptr;
        size_t n = 0;

        auto line_length = getline(&password, &n, stdin);
        auto saved_errno = errno;

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
        putchar('\n');

        if (line_length < 0)
            return Error::from_errno(saved_errno);

        VERIFY(line_length != 0);

        password[line_length - 1] = '\0';

        return SecretString::take_ownership(password, line_length);
    }
} // namespace Core
