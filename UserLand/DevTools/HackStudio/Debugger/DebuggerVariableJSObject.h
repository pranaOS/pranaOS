#pragma once

// includes
#include "DebuggerGlobalJSObject.h"
#include <LibDebug/DebugInfo.h>
#include <LibJS/Runtime/Object.h>

namespace HackStudio {

class DebuggerVariableJSObject final : public JS::Object {
    using Base = JS::Object;

public:
    static DebuggerVariableJSObject* create(DebuggerGlobalJSObject&, const Debug::DebugInfo::VariableInfo& variable_info);

    DebuggerVariableJSObject(const Debug::DebugInfo::VariableInfo& variable_info, JS::Object& prototype);
    virtual ~DebuggerVariableJSObject() override;

    virtual const char* class_name() const override { return m_variable_info.type_name.characters(); }

    virtual bool put(const JS::PropertyName& name, JS::Value value, JS::Value) override;
    void finish_writing_properties() { m_is_writing_properties = false; }

private:
    DebuggerGlobalJSObject& debugger_object() const;

    const Debug::DebugInfo::VariableInfo& m_variable_info;
    bool m_is_writing_properties { true };
};

}
