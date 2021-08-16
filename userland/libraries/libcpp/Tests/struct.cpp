/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

struct MyStruct
{
    int x;
    struct s* next;
};

int foo()
{
    MyStruct s;
    printf("%d\n", s.x);
    return s.x;
}
