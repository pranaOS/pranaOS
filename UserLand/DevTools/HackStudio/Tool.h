#pragma once

// includes
#include <AK/Noncopyable.h>
#include <LibGUI/Forward.h>

namespace HackStudio {

class FormEditorWidget;

class Tool {
    AK_MAKE_NONCOPYABLE(Tool);
    AK_MAKE_NONMOVABLE(Tool);

public:
    virtual ~Tool() { }

    virtual void on_mousedown(GUI::MouseEvent&) = 0;
    virtual void on_mouseup(GUI::MouseEvent&) = 0;
    virtual void on_mousemove(GUI::MouseEvent&) = 0;
    virtual void on_keydown(GUI::KeyEvent&) = 0;
    virtual void on_second_paint(GUI::Painter&, GUI::PaintEvent&) { }

    virtual const char* class_name() const = 0;

    virtual void attach() { }
    virtual void detach() { }

protected:
    explicit Tool(FormEditorWidget& editor)
        : m_editor(editor)
    {
    }

    FormEditorWidget& m_editor;
};

}
