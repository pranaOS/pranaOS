/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libio/MemoryReader.h>
#include <libio/MemoryWriter.h>
#include <libio/Scanner.h>
#include <libio/Write.h>
#include <libutils/String.h>
#include <libutils/Vector.h>

namespace IO
{

struct Path
{
private:
    bool _absolute = false;
    Vector<String> _elements{};

public:
    static constexpr int PARENT_SHORTHAND = 1;

    bool absolute() const { return _absolute; }
    bool relative() const { return !_absolute; }

    size_t length() const { return _elements.count(); }

    static Path parse(const String &string, int flags = 0)
    {
        return parse(string.cstring(), string.length(), flags);
    }

    static Path parse(const char *path, int flags = 0)
    {
        return parse(path, strlen(path), flags);
    }

    static Path parse(const char *path, size_t size, int flags)
    {
        IO::MemoryReader memory{path, size};
        IO::Scanner scan{memory};

        bool absolute = false;

        if (scan.skip(PATH_SEPARATOR))
        {
            absolute = true;
        }

        auto parse_element = [](auto &scan) {
            IO::MemoryWriter memory;

            while (!scan.skip(PATH_SEPARATOR) &&
                   !scan.ended())
            {
                IO::write(memory, scan.next());
            }

            return memory.string();
        };

        auto parse_shorthand = [](auto &scan) {
            Vector<String> elements{};

            scan.skip_word("..");
            elements.push("..");

            while (scan.skip('.'))
            {
                elements.push("..");
            }

            scan.skip('/');

            return elements;
        };

        Vector<String> elements{};

        while (!scan.ended())
        {
            if ((flags & PARENT_SHORTHAND) && scan.peek_is_word(".."))
            {
                elements.push_back_many(parse_shorthand(scan));
            }
            else
            {
                auto el = parse_element(scan);

                if (el->size() > 0)
                {
                    elements.push_back(el);
                }
            }
        }

        return {absolute, std::move(elements)};
    }

};

}