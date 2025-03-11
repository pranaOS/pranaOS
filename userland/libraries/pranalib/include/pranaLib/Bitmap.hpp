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
    void set_false(size_t index);
    size_t find_first_zero();
    size_t occupy_sequential(size_t size);
    void fill();
    void clear();

    static Bitmap wrap(uintptr_t location, size_t size);

private:
    bool m_self_created {}; 
    size_t m_size { 0 }; 
    size_t m_memory_size { 0 }; 
    uint32_t* m_array { nullptr };
};