/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Format.h>
#include <base/Forward.h>
#include <base/Noncopyable.h>
#include <libjs/Forward.h>

namespace JS {

class Cell {
    BASE_MAKE_NONCOPYABLE(Cell);
    BASE_MAKE_NONMOVABLE(Cell);

public:
    virtual void initialize(GlobalObject&) { }
    virtual ~Cell() { }

    bool is_marked() const { return m_mark; }
    void set_marked(bool b) { m_mark = b; }

    enum class State {
        Live,
        Dead,
    };

    State state() const { return m_state; }
    void set_state(State state) { m_state = state; }

    virtual const char* class_name() const = 0;

    class Visitor {
    public:
        void visit(Cell* cell)
        {
            if (cell)
                visit_impl(*cell);
        }
        void visit(Value);

    protected:
        virtual void visit_impl(Cell&) = 0;
        virtual ~Visitor() = default;
    };

    virtual bool is_environment() const { return false; }
    virtual void visit_edges(Visitor&) { }

    Heap& heap() const;
    VM& vm() const;

protected:
    Cell() { }

private:
    bool m_mark : 1 { false };
    State m_state : 7 { State::Live };
};

}

template<>
struct Base::Formatter<JS::Cell> : Base::Formatter<FormatString> {
    void format(FormatBuilder& builder, const JS::Cell* cell)
    {
        if (!cell)
            Formatter<FormatString>::format(builder, "Cell{nullptr}");
        else
            Formatter<FormatString>::format(builder, "{}({})", cell->class_name(), cell);
    }
};
