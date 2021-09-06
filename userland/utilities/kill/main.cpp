/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (argc <= 2) {
        return 0;
    }

    printf("%d %d", atoi(argv[1]), atoi(argv[2]));
    kill(atoi(argv[1]), atoi(argv[2]));
    return 0;
}