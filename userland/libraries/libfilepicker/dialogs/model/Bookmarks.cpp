/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libio/File.h>
#include <libjson/Json.h>
#include <libfilepicker/model/Bookmarks.h>

namespace FilePicker
{

const Vector<Bookmark> &Bookmarks::all() const 
{
    return _bookmarks;
}

void Bookmarks::add(Bookmarks &&bookmark)
{
    _bookmarks.push_back(bookmark);
    did_update();
}

}