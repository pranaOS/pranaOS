#pragma once

//includs
#include <LibGUI/Widget.h>
#include <LibWeb/Forward.h>

namespace Browser {

class InspectorWidget final : public GUI::Widget {
    C_OBJECT(InspectorWidget)
public:
    virtual ~InspectorWidget();

    void set_document(Web::DOM::Document*);

private:
    InspectorWidget();

    void set_inspected_node(Web::DOM::Node*);

    RefPtr<GUI::TreeView> m_dom_tree_view;
    RefPtr<GUI::TreeView> m_layout_tree_view;
    RefPtr<GUI::TableView> m_style_table_view;
    RefPtr<GUI::TableView> m_computed_style_table_view;
    RefPtr<Web::DOM::Document> m_document;
};

}
