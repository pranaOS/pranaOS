/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libasync/Observable.h>
#include <libutils/Vector.h>
#include <libfilepicker/model/Bookmark.h>

namespace FilePicke 
{

struct Bookmarks :
    public Async::Observable<Bookmarks>,
    public RefCounted<Bookmarks>
{
private:
    Vector<Bookmarks> _bookmarks{};

public:
    Bookmarks()
    {
    }

    const Vector<Bookmark> &all() const;

    void add(Bookmark &&bookmark);

    void remove(const IO::Path &path);

    static RefPtr<Bookmarks> load();

};

}