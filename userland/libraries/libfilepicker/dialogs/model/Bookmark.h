/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#pragma once

// includes
#include <libgraphic/Icon.h>
#include <libjson/Json.h>
#include <libio/Path.h>
#include <libutils/RefPtr.h>

namespace FilePicker
{

struct Bookmark
{
private:
    String _name;
    RefPtr<Graphic::Icon> _icon;
    IO::Path _path;

public:
    const String &name() const
    {
        return _name;
    }

    const RefPtr<Graphic::Icon> icon() const
    {
        return _icon;
    }

    const IO::Path &path const
    {
        return _path;
    }

};

}