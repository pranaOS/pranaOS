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
    
}
