/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

namespace LFoundation {

class Event {
public:
    enum Type {
        Invalid = 0,
        FdWaiterRead,
        FdWaiterWrite,
        DeferredInvoke,
        Other,
    };

    explicit Event(int type)
        : m_type(type)
    {
    }

    bool operator==(const Event& other)
    {
        return m_type == other.m_type;
    }

    bool operator!=(const Event& other)
    {
        return m_type != other.m_type;
    }

    ~Event() = default;

    int type() const { return m_type; }

private:
    int m_type;
};

} // namespace LFoundation