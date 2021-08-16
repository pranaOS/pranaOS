/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/OwnPtr.h>
#include <base/String.h>

namespace AK {
class BufferStream;
}

namespace IPC {

class Message;
struct MessageBuffer;

class Stub {
public:
    virtual ~Stub();

    virtual u32 magic() const = 0;
    virtual String name() const = 0;
    virtual OwnPtr<MessageBuffer> handle(const Message&) = 0;

protected:
    Stub();

private:
    String m_name;
};

}
