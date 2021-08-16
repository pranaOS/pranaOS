/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Optional.h>
#include <base/TemporaryChange.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Element.h>
#include <libweb/dom/ParentNode.h>
#include <libweb/dom/ShadowRoot.h>
#include <libweb/Dump.h>
#include <libweb/layout/InitialContainingBlockBox.h>
#include <libweb/layout/Node.h>
#include <libweb/layout/TableBox.h>
#include <libweb/layout/TableCellBox.h>
#include <libweb/layout/TableRowBox.h>
#include <libweb/layout/TextNode.h>
#include <libweb/layout/TreeBuilder.h>

namespace Web::Layout {

TreeBuilder::TreeBuilder()
{
}


static Layout::Node& insertion_parent_for_inline_node(Layout::NodeWithStyle& layout_parent)
{
    if (layout_parent.is_inline() && !layout_parent.is_inline_block())
        return layout_parent;

    if (!layout_parent.has_children() || layout_parent.children_are_inline())
        return layout_parent;

    if (!layout_parent.last_child()->is_anonymous() || !layout_parent.last_child()->children_are_inline()) {
        layout_parent.append_child(layout_parent.create_anonymous_wrapper());
    }
    return *layout_parent.last_child();
}

static Layout::Node& insertion_parent_for_block_node(Layout::Node& layout_parent, Layout::Node& layout_node)
{
    if (!layout_parent.has_children()) {
        return layout_parent;
    }

    if (!layout_parent.children_are_inline()) {
        return layout_parent;
    }

    NonnullRefPtrVector<Layout::Node> children;
    while (RefPtr<Layout::Node> child = layout_parent.first_child()) {
        layout_parent.remove_child(*child);
        children.append(child.release_nonnull());
    }
    layout_parent.append_child(adopt_ref(*new BlockBox(layout_node.document(), nullptr, layout_parent.computed_values().clone_inherited_values())));
    layout_parent.set_children_are_inline(false);
    for (auto& child : children) {
        layout_parent.last_child()->append_child(child);
    }
    layout_parent.last_child()->set_children_are_inline(true);
    return layout_parent;
}

void TreeBuilder::create_layout_tree(DOM::Node& dom_node, TreeBuilder::Context& context)
{
    if (dom_node.parent_or_shadow_host() && !dom_node.parent_or_shadow_host()->layout_node())
        return;

    Optional<TemporaryChange<bool>> has_svg_root_change;

    if (dom_node.is_svg_container()) {
        has_svg_root_change.emplace(context.has_svg_root, true);
    } else if (dom_node.requires_svg_container() && !context.has_svg_root) {
        return;
    }

    auto layout_node = dom_node.create_layout_node();
    if (!layout_node)
        return;

    if (!dom_node.parent_or_shadow_host()) {
        m_layout_root = layout_node;
    } else {
        if (layout_node->is_inline()) {

            auto& insertion_point = insertion_parent_for_inline_node(*m_parent_stack.last());
            insertion_point.append_child(*layout_node);
            insertion_point.set_children_are_inline(true);
        } else {

            auto& nearest_non_inline_ancestor = [&]() -> Layout::Node& {
                for (ssize_t i = m_parent_stack.size() - 1; i >= 0; --i) {
                    if (!m_parent_stack[i]->is_inline() || m_parent_stack[i]->is_inline_block())
                        return *m_parent_stack[i];
                }
                VERIFY_NOT_REACHED();
            }();
            auto& insertion_point = insertion_parent_for_block_node(nearest_non_inline_ancestor, *layout_node);
            insertion_point.append_child(*layout_node);
            insertion_point.set_children_are_inline(false);
        }
    }

    auto* shadow_root = is<DOM::Element>(dom_node) ? verify_cast<DOM::Element>(dom_node).shadow_root() : nullptr;

    if ((dom_node.has_children() || shadow_root) && layout_node->can_have_children()) {
        push_parent(verify_cast<NodeWithStyle>(*layout_node));
        if (shadow_root)
            create_layout_tree(*shadow_root, context);
        verify_cast<DOM::ParentNode>(dom_node).for_each_child([&](auto& dom_child) {
            create_layout_tree(dom_child, context);
        });
        pop_parent();
    }
}

RefPtr<Node> TreeBuilder::build(DOM::Node& dom_node)
{
    if (dom_node.parent()) {

        for (auto* ancestor = dom_node.parent()->layout_node(); ancestor; ancestor = ancestor->parent())
            m_parent_stack.prepend(verify_cast<NodeWithStyle>(ancestor));
    }

    Context context;
    create_layout_tree(dom_node, context);

    if (auto* root = dom_node.document().layout_node())
        fixup_tables(*root);

    return move(m_layout_root);
}

template<CSS::Display display, typename Callback>
void TreeBuilder::for_each_in_tree_with_display(NodeWithStyle& root, Callback callback)
{
    root.for_each_in_inclusive_subtree_of_type<Box>([&](auto& box) {
        if (box.computed_values().display() == display)
            callback(box);
        return IterationDecision::Continue;
    });
}

void TreeBuilder::fixup_tables(NodeWithStyle& root)
{

    remove_irrelevant_boxes(root);
    generate_missing_child_wrappers(root);
    generate_missing_parents(root);
}

void TreeBuilder::remove_irrelevant_boxes(NodeWithStyle& root)
{

    NonnullRefPtrVector<Box> to_remove;

    for_each_in_tree_with_display<CSS::Display::TableColumn>(root, [&](Box& table_column) {
        table_column.for_each_child([&](auto& child) {
            to_remove.append(child);
        });
    });

    for_each_in_tree_with_display<CSS::Display::TableColumnGroup>(root, [&](Box& table_column_group) {
        table_column_group.for_each_child([&](auto& child) {
            if (child.computed_values().display() != CSS::Display::TableColumn)
                to_remove.append(child);
        });
    });

    for (auto& box : to_remove)
        box.parent()->remove_child(box);
}

static bool is_table_track(CSS::Display display)
{
    return display == CSS::Display::TableRow || display == CSS::Display::TableColumn;
}

static bool is_table_track_group(CSS::Display display)
{
    return display == CSS::Display::TableRowGroup || display == CSS::Display::TableHeaderGroup || display == CSS::Display::TableFooterGroup
        || display == CSS::Display::TableColumnGroup;
}

static bool is_not_proper_table_child(const Node& node)
{
    if (!node.has_style())
        return true;
    auto display = node.computed_values().display();
    return !is_table_track_group(display) && !is_table_track(display) && display != CSS::Display::TableCaption;
}

static bool is_not_table_row(const Node& node)
{
    if (!node.has_style())
        return true;
    auto display = node.computed_values().display();
    return display != CSS::Display::TableRow;
}

static bool is_not_table_cell(const Node& node)
{
    if (!node.has_style())
        return true;
    auto display = node.computed_values().display();
    return display != CSS::Display::TableCell;
}

template<typename Matcher, typename Callback>
static void for_each_sequence_of_consecutive_children_matching(NodeWithStyle& parent, Matcher matcher, Callback callback)
{
    NonnullRefPtrVector<Node> sequence;
    Node* next_sibling = nullptr;
    for (auto* child = parent.first_child(); child; child = next_sibling) {
        next_sibling = child->next_sibling();
        if (matcher(*child)) {
            sequence.append(*child);
        } else {
            if (!sequence.is_empty()) {
                callback(sequence, next_sibling);
                sequence.clear();
            }
        }
    }
    if (!sequence.is_empty())
        callback(sequence, nullptr);
}

template<typename WrapperBoxType>
static void wrap_in_anonymous(NonnullRefPtrVector<Node>& sequence, Node* nearest_sibling)
{
    VERIFY(!sequence.is_empty());
    auto& parent = *sequence.first().parent();
    auto computed_values = parent.computed_values().clone_inherited_values();
    static_cast<CSS::MutableComputedValues&>(computed_values).set_display(WrapperBoxType::static_display());
    auto wrapper = adopt_ref(*new WrapperBoxType(parent.document(), nullptr, move(computed_values)));
    for (auto& child : sequence) {
        parent.remove_child(child);
        wrapper->append_child(child);
    }
    if (nearest_sibling)
        parent.insert_before(move(wrapper), *nearest_sibling);
    else
        parent.append_child(move(wrapper));
}

void TreeBuilder::generate_missing_child_wrappers(NodeWithStyle& root)
{
    for_each_in_tree_with_display<CSS::Display::Table>(root, [&](auto& parent) {
        for_each_sequence_of_consecutive_children_matching(parent, is_not_proper_table_child, [&](auto sequence, auto nearest_sibling) {
            wrap_in_anonymous<TableRowBox>(sequence, nearest_sibling);
        });
    });

    for_each_in_tree_with_display<CSS::Display::TableRowGroup>(root, [&](auto& parent) {
        for_each_sequence_of_consecutive_children_matching(parent, is_not_table_row, [&](auto& sequence, auto nearest_sibling) {
            wrap_in_anonymous<TableRowBox>(sequence, nearest_sibling);
        });
    });

    for_each_in_tree_with_display<CSS::Display::TableHeaderGroup>(root, [&](auto& parent) {
        for_each_sequence_of_consecutive_children_matching(parent, is_not_table_row, [&](auto& sequence, auto nearest_sibling) {
            wrap_in_anonymous<TableRowBox>(sequence, nearest_sibling);
        });
    });
    for_each_in_tree_with_display<CSS::Display::TableFooterGroup>(root, [&](auto& parent) {
        for_each_sequence_of_consecutive_children_matching(parent, is_not_table_row, [&](auto& sequence, auto nearest_sibling) {
            wrap_in_anonymous<TableRowBox>(sequence, nearest_sibling);
        });
    });

    for_each_in_tree_with_display<CSS::Display::TableRow>(root, [&](auto& parent) {
        for_each_sequence_of_consecutive_children_matching(parent, is_not_table_cell, [&](auto& sequence, auto nearest_sibling) {
            wrap_in_anonymous<TableCellBox>(sequence, nearest_sibling);
        });
    });
}

void TreeBuilder::generate_missing_parents(NodeWithStyle&)
{
}

}
