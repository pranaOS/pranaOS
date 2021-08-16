/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include "Generator.h"
#include "PassManager.h"
#include <libjs/bytecode/Label.h>
#include <libjs/bytecode/Register.h>
#include <libjs/Forward.h>
#include <libjs/heap/Cell.h>
#include <libjs/heap/Handle.h>
#include <libjs/runtime/Exception.h>
#include <libjs/runtime/Value.h>

namespace JS::Bytecode {

using RegisterWindow = Vector<Value>;

class Interpreter {
public:
    explicit Interpreter(GlobalObject&);
    ~Interpreter();

    static Interpreter* current();

    GlobalObject& global_object() { return m_global_object; }
    VM& vm() { return m_vm; }

    Value run(Bytecode::Executable const&, Bytecode::BasicBlock const* entry_point = nullptr);

    ALWAYS_INLINE Value& accumulator() { return reg(Register::accumulator()); }
    Value& reg(Register const& r) { return registers()[r.index()]; }
    [[nodiscard]] RegisterWindow snapshot_frame() const { return m_register_windows.last(); }

    void enter_frame(RegisterWindow const& frame)
    {
        ++m_manually_entered_frames;
        m_register_windows.append(make<RegisterWindow>(frame));
    }
    void leave_frame()
    {
        VERIFY(m_manually_entered_frames);
        --m_manually_entered_frames;
        m_register_windows.take_last();
    }

    void jump(Label const& label)
    {
        m_pending_jump = &label.block();
    }
    void do_return(Value return_value) { m_return_value = return_value; }

    void enter_unwind_context(Optional<Label> handler_target, Optional<Label> finalizer_target);
    void leave_unwind_context();
    void continue_pending_unwind(Label const& resume_label);

    Executable const& current_executable() { return *m_current_executable; }

    enum class OptimizationLevel {
        Default,
        __Count,
    };
    static Bytecode::PassManager& optimization_pipeline(OptimizationLevel = OptimizationLevel::Default);

private:
    RegisterWindow& registers() { return m_register_windows.last(); }

    static Base::Array<OwnPtr<PassManager>, static_cast<UnderlyingType<Interpreter::OptimizationLevel>>(Interpreter::OptimizationLevel::__Count)> s_optimization_pipelines;

    VM& m_vm;
    GlobalObject& m_global_object;
    NonnullOwnPtrVector<RegisterWindow> m_register_windows;
    Optional<BasicBlock const*> m_pending_jump;
    Value m_return_value;
    size_t m_manually_entered_frames { 0 };
    Executable const* m_current_executable { nullptr };
    Vector<UnwindInfo> m_unwind_contexts;
    Handle<Exception> m_saved_exception;
};

}
