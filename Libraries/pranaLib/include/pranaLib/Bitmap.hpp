//
//  Bitmap.hpp
//  Libraries
//
//  Created by Pranav on 28/06/22.
//

#pragma once

#include "Common.hpp"

#define BITMAP_NULL -1

class Bitmap {
public:
    Bitmap() = default;
    Bitmap(uintptr_t location, size_t size);
    Bitmap(size_t size);
    ~Bitmap();

    Bitmap(const Bitmap&);
    Bitmap& operator=(const Bitmap&);
    Bitmap(Bitmap&&);
    Bitmap& operator=(Bitmap&&);

    size_t size() const;
    size_t memory_size() const;

    bool operator[](size_t index);
    void set_true(size_t index);
};

