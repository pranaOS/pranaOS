/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libcompression/CRC.h>
#include <libio/Reader.h>

namespace IO
{

struct CRCReader :
    public Reader
{
private:
    Reader &_reader;
    Compression::CRC _crc;

public:
    CRCReader(Reader &reader, uint32_t crc = 0) : _reader{reader}, _crc{crc}
    {
    }

    inline uint32_t checksum()
    {
        return _crc.checksum();
    }

};

}