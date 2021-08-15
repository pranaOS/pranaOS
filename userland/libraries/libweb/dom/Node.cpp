/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/StringBuilder.h>
#include <libjs/AST.h>
#include <libjs/runtime/FunctionObject.h>
#include <libweb/bindings/EventWrapper.h>
#include <libweb/bindings/NodeWrapper.h>
#include <libweb/bindings/NodeWrapperFactory.h>
#include <libweb/dom/Comment.h>
#include <libweb/dom/DocumentType.h>
#include <libweb/dom/Element.h>
#include <libweb/dom/ElementFactory.h>
#include <libweb/dom/Event.h>
#include <libweb/dom/EventDispatcher.h>
#include <libweb/dom/EventListener.h>
#include <libweb/dom/Node.h>
#include <libweb/dom/ProcessingInstruction.h>
#include <libweb/dom/ShadowRoot.h>
#include <libweb/html/HTMLAnchorElement.h>
#include <libweb/layout/InitialContainingBlockBox.h>
#include <libweb/layout/Node.h>
#include <libweb/layout/TextNode.h>
#include <libweb/Origin.h>

namespace Web::DOM {

Node::Node(Document& document, NodeType type)
    : EventTarget(static_cast<Bindings::ScriptExecutionContext&>(document))
    , m_document(&document)
    , m_type(type)
{
    if (!is_document())
        m_document->ref_from_node({});
}

Node::~Node()
{
    VERIFY(m_deletion_has_begun);
    if (layout_node() && layout_node()->parent())
        layout_node()->parent()->remove_child(*layout_node());

    if (!is_document())
        m_document->unref_from_node({});
}

const HTML::HTMLAnchorElement* Node::enclosing_link_element() const
{
    for (auto* node = this; node; node = node->parent()) {
        if (is<HTML::HTMLAnchorElement>(*node) && verify_cast<HTML::HTMLAnchorElement>(*node).has_attribute(HTML::AttributeNames::href))
            return verify_cast<HTML::HTMLAnchorElement>(node);
    }
    return nullptr;
}

const HTML::HTMLElement* Node::enclosing_html_element() const
{
    return first_ancestor_of_type<HTML::HTMLElement>();
}

const HTML::HTMLElement* Node::enclosing_html_element_with_attribute(const FlyString& attribute) const
{
    for (auto* node = this; node; node = node->parent()) {
        if (is<HTML::HTMLElement>(*node) && verify_cast<HTML::HTMLElement>(*node).has_attribute(attribute))
            return verify_cast<HTML::HTMLElement>(node);
    }
    return nullptr;
}

String Node::text_content() const
{
    StringBuilder builder;
    for (auto* child = first_child(); child; child = child->next_sibling()) {
        builder.append(child->text_content());
    }
    return builder.to_string();
}

void Node::set_text_content(const String& content)
{
    if (is_text()) {
        verify_cast<Text>(this)->set_data(content);
    } else {
        remove_all_children();
        append_child(document().create_text_node(content));
    }

    set_needs_style_update(true);
    document().invalidate_layout();
}

RefPtr<Layout::Node> Node::create_layout_node()
{
    return nullptr;
}

void Node::invalidate_style()
{
    for_each_in_inclusive_subtree_of_type<Element>([&](auto& element) {
        element.set_needs_style_update(true);
        return IterationDecision::Continue;
    });
    document().schedule_style_update();
}

bool Node::is_link() const
{
    return enclosing_link_element();
}

bool Node::dispatch_event(NonnullRefPtr<Event> event)
{
    return EventDispatcher::dispatch(*this, event);
}

String Node::child_text_content() const
{
    if (!is<ParentNode>(*this))
        return String::empty();

    StringBuilder builder;
    verify_cast<ParentNode>(*this).for_each_child([&](auto& child) {
        if (is<Text>(child))
            builder.append(verify_cast<Text>(child).text_content());
    });
    return builder.build();
}

Node* Node::root()
{
    Node* root = this;
    while (root->parent())
        root = root->parent();
    return root;
}

Node* Node::shadow_including_root()
{
    auto node_root = root();
    if (is<ShadowRoot>(node_root))
        return verify_cast<ShadowRoot>(node_root)->host()->shadow_including_root();
    return node_root;
}

bool Node::is_connected() const
{
    return shadow_including_root() && shadow_including_root()->is_document();
}

Element* Node::parent_element()
{
    if (!parent() || !is<Element>(parent()))
        return nullptr;
    return verify_cast<Element>(parent());
}

const Element* Node::parent_element() const
{
    if (!parent() || !is<Element>(parent()))
        return nullptr;
    return verify_cast<Element>(parent());
}

ExceptionOr<void> Node::ensure_pre_insertion_validity(NonnullRefPtr<Node> node, RefPtr<Node> child) const
{
    if (!is<Document>(this) && !is<DocumentFragment>(this) && !is<Element>(this))
        return DOM::HierarchyRequestError::create("Can only insert into a document, document fragment or element");

    if (node->is_host_including_inclusive_ancestor_of(*this))
        return DOM::HierarchyRequestError::create("New node is an ancestor of this node");

    if (child && child->parent() != this)
        return DOM::NotFoundError::create("This node is not the parent of the given child");


    if (!is<DocumentFragment>(*node) && !is<DocumentType>(*node) && !is<Element>(*node) && !is<Text>(*node) && !is<Comment>(*node) && !is<ProcessingInstruction>(*node))
        return DOM::HierarchyRequestError::create("Invalid node type for insertion");

    if ((is<Text>(*node) && is<Document>(this)) || (is<DocumentType>(*node) && !is<Document>(this)))
        return DOM::HierarchyRequestError::create("Invalid node type for insertion");

    if (is<Document>(this)) {
        if (is<DocumentFragment>(*node)) {
            auto node_element_child_count = verify_cast<DocumentFragment>(*node).child_element_count();
            if ((node_element_child_count > 1 || node->has_child_of_type<Text>())
                || (node_element_child_count == 1 && (has_child_of_type<Element>() || is<DocumentType>(child.ptr())/* FIXME: or child is non-null and a doctype is following child. */))) {
                return DOM::HierarchyRequestError::create("Invalid node type for insertion");
            }
        } else if (is<Element>(*node)) {
            if (has_child_of_type<Element>() || is<DocumentType>(child.ptr()) /* FIXME: or child is non-null and a doctype is following child. */)
                return DOM::HierarchyRequestError::create("Invalid node type for insertion");
        } else if (is<DocumentType>(*node)) {
            if (has_child_of_type<DocumentType>() /* FIXME: or child is non-null and an element is preceding child */ || (!child && has_child_of_type<Element>()))
                return DOM::HierarchyRequestError::create("Invalid node type for insertion");
        }
    }

    return {};
}

void Node::insert_before(NonnullRefPtr<Node> node, RefPtr<Node> child, bool suppress_observers)
{
    NonnullRefPtrVector<Node> nodes;
    if (is<DocumentFragment>(*node))
        nodes = verify_cast<DocumentFragment>(*node).child_nodes();
    else
        nodes.append(node);

    auto count = nodes.size();
    if (count == 0)
        return;

    if (is<DocumentFragment>(*node)) {
        node->remove_all_children(true);
    }

    if (child) {

    }


    for (auto& node_to_insert : nodes) { 
        document().adopt_node(node_to_insert);

        if (!child)
            TreeNode<Node>::append_child(node_to_insert);
        else
            TreeNode<Node>::insert_before(node_to_insert, child);


        node_to_insert.for_each_in_inclusive_subtree([&](Node& inclusive_descendant) {
            inclusive_descendant.inserted();
            if (inclusive_descendant.is_connected()) {

            }

            return IterationDecision::Continue;
        });
    }

    if (!suppress_observers) {

    }

    children_changed();
}

ExceptionOr<NonnullRefPtr<Node>> Node::pre_insert(NonnullRefPtr<Node> node, RefPtr<Node> child)
{
    auto validity_result = ensure_pre_insertion_validity(node, child);
    if (validity_result.is_exception())
        return validity_result.exception();

    auto reference_child = child;
    if (reference_child == node)
        reference_child = node->next_sibling();

    insert_before(node, reference_child);
    return node;
}

ExceptionOr<NonnullRefPtr<Node>> Node::pre_remove(NonnullRefPtr<Node> child)
{
    if (child->parent() != this)
        return DOM::NotFoundError::create("Child does not belong to this node");

    child->remove();

    return child;
}

ExceptionOr<NonnullRefPtr<Node>> Node::append_child(NonnullRefPtr<Node> node)
{
    return pre_insert(node, nullptr);
}

void Node::remove(bool suppress_observers)
{
    auto* parent = TreeNode<Node>::parent();
    VERIFY(parent);


    parent->remove_child(*this);

    removed_from(parent);

    for_each_in_subtree([&](Node& descendant) {
        descendant.removed_from(nullptr);


        return IterationDecision::Continue;
    });

    if (!suppress_observers) {

    }

    parent->children_changed();
}

ExceptionOr<NonnullRefPtr<Node>> Node::replace_child(NonnullRefPtr<Node> node, NonnullRefPtr<Node> child)
{
    // NOTE: This differs slightly from ensure_pre_insertion_validity.
    if (!is<Document>(this) && !is<DocumentFragment>(this) && !is<Element>(this))
        return DOM::HierarchyRequestError::create("Can only insert into a document, document fragment or element");

    if (node->is_host_including_inclusive_ancestor_of(*this))
        return DOM::HierarchyRequestError::create("New node is an ancestor of this node");

    if (child->parent() != this)
        return DOM::NotFoundError::create("This node is not the parent of the given child");


    if (!is<DocumentFragment>(*node) && !is<DocumentType>(*node) && !is<Element>(*node) && !is<Text>(*node) && !is<Comment>(*node) && !is<ProcessingInstruction>(*node))
        return DOM::HierarchyRequestError::create("Invalid node type for insertion");

    if ((is<Text>(*node) && is<Document>(this)) || (is<DocumentType>(*node) && !is<Document>(this)))
        return DOM::HierarchyRequestError::create("Invalid node type for insertion");

    if (is<Document>(this)) {
        if (is<DocumentFragment>(*node)) {
            auto node_element_child_count = verify_cast<DocumentFragment>(*node).child_element_count();
            if ((node_element_child_count > 1 || node->has_child_of_type<Text>())
                || (node_element_child_count == 1 && (first_child_of_type<Element>() != child /* FIXME: or a doctype is following child. */))) {
                return DOM::HierarchyRequestError::create("Invalid node type for insertion");
            }
        } else if (is<Element>(*node)) {
            if (first_child_of_type<Element>() != child /* FIXME: or a doctype is following child. */)
                return DOM::HierarchyRequestError::create("Invalid node type for insertion");
        } else if (is<DocumentType>(*node)) {
            if (first_child_of_type<DocumentType>() != node /* FIXME: or an element is preceding child */)
                return DOM::HierarchyRequestError::create("Invalid node type for insertion");
        }
    }

    auto reference_child = child->next_sibling();
    if (reference_child == node)
        reference_child = node->next_sibling();


    if (child->parent()) {

        child->remove(true);
    }


    insert_before(node, reference_child, true);


    return child;
}

NonnullRefPtr<Node> Node::clone_node(Document* document, bool clone_children)
{
    if (!document)
        document = m_document;
    RefPtr<Node> copy;
    if (is<Element>(this)) {
        auto& element = *verify_cast<Element>(this);
        auto element_copy = DOM::create_element(*document, element.local_name(), element.namespace_() /* FIXME: node’s namespace prefix, and node’s is value, with the synchronous custom elements flag unset */);
        element.for_each_attribute([&](auto& name, auto& value) {
            element_copy->set_attribute(name, value);
        });
        copy = move(element_copy);
    } else if (is<Document>(this)) {
        auto document_ = verify_cast<Document>(this);
        auto document_copy = Document::create(document_->url());
        document_copy->set_encoding(document_->encoding());
        document_copy->set_content_type(document_->content_type());
        document_copy->set_origin(document_->origin());
        document_copy->set_quirks_mode(document_->mode());

        copy = move(document_copy);
    } else if (is<DocumentType>(this)) {
        auto document_type = verify_cast<DocumentType>(this);
        auto document_type_copy = adopt_ref(*new DocumentType(*document));
        document_type_copy->set_name(document_type->name());
        document_type_copy->set_public_id(document_type->public_id());
        document_type_copy->set_system_id(document_type->system_id());
        copy = move(document_type_copy);
    } else if (is<Text>(this)) {
        auto text = verify_cast<Text>(this);
        auto text_copy = adopt_ref(*new Text(*document, text->data()));
        copy = move(text_copy);
    } else if (is<Comment>(this)) {
        auto comment = verify_cast<Comment>(this);
        auto comment_copy = adopt_ref(*new Comment(*document, comment->data()));
        copy = move(comment_copy);
    } else if (is<ProcessingInstruction>(this)) {
        auto processing_instruction = verify_cast<ProcessingInstruction>(this);
        auto processing_instruction_copy = adopt_ref(*new ProcessingInstruction(*document, processing_instruction->data(), processing_instruction->target()));
        copy = move(processing_instruction_copy);
    } else if (is<DocumentFragment>(this)) {
        auto document_fragment_copy = adopt_ref(*new DocumentFragment(*document));
        copy = move(document_fragment_copy);
    } else {
        dbgln("clone_node() not implemented for NodeType {}", (u16)m_type);
        TODO();
    }


    cloned(*copy, clone_children);

    if (clone_children) {
        for_each_child([&](auto& child) {
            copy->append_child(child.clone_node(document, true));
        });
    }
    return copy.release_nonnull();
}

ExceptionOr<NonnullRefPtr<Node>> Node::clone_node_binding(bool deep)
{
    if (is<ShadowRoot>(*this))
        return NotSupportedError::create("Cannot clone shadow root");
    return clone_node(nullptr, deep);
}

void Node::set_document(Badge<Document>, Document& document)
{
    if (m_document == &document)
        return;
    document.ref_from_node({});
    m_document->unref_from_node({});
    m_document = &document;
}

bool Node::is_editable() const
{
    return parent() && parent()->is_editable();
}

JS::Object* Node::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

void Node::removed_last_ref()
{
    if (is<Document>(*this)) {
        verify_cast<Document>(*this).removed_last_ref();
        return;
    }
    m_deletion_has_begun = true;
    delete this;
}

void Node::set_layout_node(Badge<Layout::Node>, Layout::Node* layout_node) const
{
    if (layout_node)
        m_layout_node = layout_node->make_weak_ptr();
    else
        m_layout_node = nullptr;
}

EventTarget* Node::get_parent(const Event&)
{
    return parent();
}

void Node::set_needs_style_update(bool value)
{
    if (m_needs_style_update == value)
        return;
    m_needs_style_update = value;

    if (m_needs_style_update) {
        for (auto* ancestor = parent(); ancestor; ancestor = ancestor->parent()) {
            //dbgln("{}", ancestor->node_name());
            ancestor->m_child_needs_style_update = true;
        }
        document().schedule_style_update();
    }
}

void Node::inserted()
{
    set_needs_style_update(true);
}

ParentNode* Node::parent_or_shadow_host()
{
    if (is<ShadowRoot>(*this))
        return verify_cast<ShadowRoot>(*this).host();
    return verify_cast<ParentNode>(parent());
}

NonnullRefPtrVector<Node> Node::child_nodes() const
{
    NonnullRefPtrVector<Node> nodes;

    for_each_child([&](auto& child) {
        nodes.append(child);
    });

    return nodes;
}

void Node::remove_all_children(bool suppress_observers)
{
    while (RefPtr<Node> child = first_child())
        child->remove(suppress_observers);
}

u16 Node::compare_document_position(RefPtr<Node> other)
{
    enum Position : u16 {
        DOCUMENT_POSITION_EQUAL = 0,
        DOCUMENT_POSITION_DISCONNECTED = 1,
        DOCUMENT_POSITION_PRECEDING = 2,
        DOCUMENT_POSITION_FOLLOWING = 4,
        DOCUMENT_POSITION_CONTAINS = 8,
        DOCUMENT_POSITION_CONTAINED_BY = 16,
        DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC = 32,
    };

    if (this == other)
        return DOCUMENT_POSITION_EQUAL;

    Node* node1 = other.ptr();
    Node* node2 = this;

    VERIFY(node1->type() != NodeType::ATTRIBUTE_NODE && node2->type() != NodeType::ATTRIBUTE_NODE);

    if ((node1 == nullptr || node2 == nullptr) || (node1->root() != node2->root()))
        return DOCUMENT_POSITION_DISCONNECTED | DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC | (node1 > node2 ? DOCUMENT_POSITION_PRECEDING : DOCUMENT_POSITION_FOLLOWING);

    if (node1->is_ancestor_of(*node2))
        return DOCUMENT_POSITION_CONTAINS | DOCUMENT_POSITION_PRECEDING;

    if (node2->is_ancestor_of(*node1))
        return DOCUMENT_POSITION_CONTAINED_BY | DOCUMENT_POSITION_FOLLOWING;

    if (node1->is_before(*node2))
        return DOCUMENT_POSITION_PRECEDING;
    else
        return DOCUMENT_POSITION_FOLLOWING;
}

bool Node::is_host_including_inclusive_ancestor_of(const Node& other) const
{
    return is_inclusive_ancestor_of(other) || (is<DocumentFragment>(other.root()) && verify_cast<DocumentFragment>(other.root())->host() && is_inclusive_ancestor_of(*verify_cast<DocumentFragment>(other.root())->host().ptr()));
}

RefPtr<Document> Node::owner_document() const
{
    if (is_document())
        return nullptr;
    return m_document;
}

void Node::serialize_tree_as_json(JsonObjectSerializer<StringBuilder>& object) const
{
    object.add("name", node_name().view());
    if (is_document()) {
        object.add("type", "document");
    } else if (is_element()) {
        object.add("type", "element");

        auto element = static_cast<DOM::Element const*>(this);
        if (element->has_attributes()) {
            auto attributes = object.add_object("attributes");
            element->for_each_attribute([&attributes](auto& name, auto& value) {
                attributes.add(name, value);
            });
        }
    } else if (is_text()) {
        object.add("type", "text");

        auto text_node = static_cast<DOM::Text const*>(this);
        object.add("text", text_node->data());
    }

    if (has_child_nodes()) {
        auto children = object.add_array("children");
        for_each_child([&children](DOM::Node& child) {
            JsonObjectSerializer<StringBuilder> child_object = children.add_object();
            child.serialize_tree_as_json(child_object);
        });
    }
}

bool Node::is_scripting_disabled() const
{
    return !document().browsing_context();
}

bool Node::contains(RefPtr<Node> other) const
{
    return other && other->is_inclusive_descendant_of(*this);
}

}
