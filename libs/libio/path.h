#pragma once

// includes
#include <libabi/filesystem.h>
#include <libio/memoryreader.h>
#include <libio/memorywriter.h>
#include <libio/scanner.h>
#include <libio/write.h>
#include <libutils/string.h>
#include <libutils/vector.h>

namespace IO
{

struct Path
{
private:
    bool _absolute = false;
    Vector<String> _elements{};

public:
    static constexpr int PARENT_SHORTHAND = 1;

    bool absolute() const
    {
        return _absolute;
    }

    bool relative() const
    {
        return !_absolute;
    }

    size_t length() const
    {
        return _elements.count();
    }

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

        return {absolute, std::move(_elements)};
    }

    static Path join(String left, String right)
    {
        return join(parse(left), parse(right));
    }

    Path()
    {
    }

    Path(const Path &other) : _absolute{other.absolute()}, _elements{other._elements}
    {
    }

};

}