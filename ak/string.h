/*
 * Copyright (c) 2020 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#pragma once

#include "types.h"
#include "list.h"
#include "memoperator.h"

namespace ak {
    #define isalpha(c) (((unsigned)c|32)-'a' < 26)

    class String {
    public:
        static int strlen(const char* str);
        static bool strcmp(const char* strA, const char* strB);
        static bool strncmp(const char* s1, const char* s2, int n);
        static int indexOf(const char* str, char c, ak::uint32_t skip = 0);
        static bool contains(const char* str, char c);
        static List<char*> split(const char* str, char d);
        static char* uppercase(char* str);
        static char* lowercase(char* str);
        static char uppercase(char c);
        static char lowercase(char c);
        static char* strcpy(char *s1, const char *s2);
        static char* strncpy(char *s1, const char *s2, unsigned int n);
    };
}