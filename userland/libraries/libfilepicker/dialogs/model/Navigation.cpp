/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <libfilepicker/model/Navigation.h>

namespace FilePicker
{

Navigation::Navigation() : _current(IO::Path::parse(""))
{
}

bool Navigation::can_go_up()
{
    return _current.length() > 0;
}

void Navigation::go_up()
{
    if (can_go_up())
    {
        clear_foreward();
        navigate(_current.dirpath(), BACKWARD);
    }
}

bool Navigation::can_go_backward()
{
    return _backward.any();
}

void Navigation::go_backward()
{
    if (can_go_backward())
    {
        navigate(_backward.pop_back(), FOREWARD);
    }
}

bool Navigation::can_go_forward()
{
    return _foreward.any();
}
    
}
