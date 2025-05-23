/**
 * @file object.cpp
 * @author Krisna Pranav
 * @brief object
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/badge.h>
#include <mods/jsonobject.h>
#include <libcore/event.h>
#include <libcore/eventloop.h>
#include <libcore/object.h>
#include <stdio.h>

namespace Core 
{
    
    /**
     * @return IntrusiveList<&Object::m_all_objects_list_node>& 
     */
    IntrusiveList<&Object::m_all_objects_list_node>& Object::all_objects()
    {
        static IntrusiveList<&Object::m_all_objects_list_node> objects;
        return objects;
    }

    /**
     * @brief Construct a new Object::Object object
     * 
     * @param parent 
     */
    Object::Object(Object* parent)
        : m_parent(parent)
    {
        all_objects().append(*this);
        if (m_parent)
            m_parent->add_child(*this);

        REGISTER_READONLY_STRING_PROPERTY("class_name", class_name);
        REGISTER_STRING_PROPERTY("name", name, set_name);

        register_property(
            "address", [this] { return FlatPtr(this); },
            [](auto&) { return false; });

        register_property(
            "parent", [this] { return FlatPtr(this->parent()); },
            [](auto&) { return false; });
    }

    /**
     * @brief Destroy the Object::Object object
     * 
     */
    Object::~Object()
    {
        auto children = move(m_children);

        for (auto& child : children)
            child.m_parent = nullptr;

        all_objects().remove(*this);
        stop_timer();
        if (m_parent)
            m_parent->remove_child(*this);
    }

    /**
     * @param event 
     */
    void Object::event(Core::Event& event)
    {
        switch (event.type()) {
        case Core::Event::Timer:
            return timer_event(static_cast<TimerEvent&>(event));
        case Core::Event::ChildAdded:
        case Core::Event::ChildRemoved:
            return child_event(static_cast<ChildEvent&>(event));
        case Core::Event::Invalid:
            VERIFY_NOT_REACHED();
            break;
        case Core::Event::Custom:
            return custom_event(static_cast<CustomEvent&>(event));
        default:
            break;
        }
    }   

    /**
     * @param object 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Object::try_add_child(Object& object)
    {
        VERIFY(!object.parent() || object.parent() == this);
        TRY(m_children.try_append(object));
        object.m_parent = this;
        Core::ChildEvent child_event(Core::Event::ChildAdded, object);
        event(child_event);

        return {};
    }

    /**
     * @param object 
     */
    void Object::add_child(Object& object)
    {
        MUST(try_add_child(object));
    }

    /**
     * @param new_child 
     * @param before_child 
     */
    void Object::insert_child_before(Object& new_child, Object& before_child)
    {
        VERIFY(!new_child.parent() || new_child.parent() == this);
        new_child.m_parent = this;
        m_children.insert_before_matching(new_child, [&](auto& existing_child) { return existing_child.ptr() == &before_child; });
        Core::ChildEvent child_event(Core::Event::ChildAdded, new_child, &before_child);
        event(child_event);
    }

    /**
     * @param object 
     */
    void Object::remove_child(Object& object)
    {
        for (size_t i = 0; i < m_children.size(); ++i) {
            if (m_children.ptr_at(i).ptr() == &object) {
                NonnullRefPtr<Object> protector = object;
                object.m_parent = nullptr;
                m_children.remove(i);
                Core::ChildEvent child_event(Core::Event::ChildRemoved, object);
                event(child_event);
                return;
            }
        }
        VERIFY_NOT_REACHED();
    }


    void Object::remove_all_children()
    {
        while (!m_children.is_empty())
            m_children.first().remove_from_parent();
    }

    void Object::timer_event(Core::TimerEvent&)
    {
    }

    void Object::child_event(Core::ChildEvent&)
    {
    }

    void Object::custom_event(CustomEvent&)
    {
    }

    /**
     * @param ms 
     * @param fire_when_not_visible 
     */
    void Object::start_timer(int ms, TimerShouldFireWhenNotVisible fire_when_not_visible)
    {
        if (m_timer_id) {
            dbgln("{} {:p} already has a timer!", class_name(), this);
            VERIFY_NOT_REACHED();
        }

        m_timer_id = Core::EventLoop::register_timer(*this, ms, true, fire_when_not_visible);
    }

    void Object::stop_timer()
    {
        if (!m_timer_id)
            return;
        bool success = Core::EventLoop::unregister_timer(m_timer_id);
        VERIFY(success);
        m_timer_id = 0;
    }

    /**
     * @param indent 
     */
    void Object::dump_tree(int indent)
    {
        for (int i = 0; i < indent; ++i) {
            out(" ");
        }
        out("{}{{{:p}}}", class_name(), this);
        if (!name().is_null())
            out(" {}", name());
        outln();

        for_each_child([&](auto& child) {
            child.dump_tree(indent + 2);
            return IterationDecision::Continue;
        });
    }

    /**
     * @param invokee 
     */
    void Object::deferred_invoke(Function<void()> invokee)
    {
        Core::deferred_invoke([invokee = move(invokee), strong_this = NonnullRefPtr(*this)] { invokee(); });
    }

    /**
     * @param json 
     */
    void Object::save_to(JsonObject& json)
    {
        for (auto& it : m_properties) {
            auto& property = it.value;
            json.set(property->name(), property->get());
        }
    }

    /**
     * @param name 
     * @return JsonValue 
     */
    JsonValue Object::property(String const& name) const
    {
        auto it = m_properties.find(name);

        if (it == m_properties.end())
            return JsonValue();

        return it->value->get();
    }

    /**
     * @param name 
     * @param value 
     * @return true 
     * @return false 
     */
    bool Object::set_property(String const& name, JsonValue const& value)
    {
        auto it = m_properties.find(name);

        if (it == m_properties.end())
            return false;

        return it->value->set(value);
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Object::is_ancestor_of(Object const& other) const
    {
        if (&other == this)
            return false;

        for (auto* ancestor = other.parent(); ancestor; ancestor = ancestor->parent()) {
            if (ancestor == this)
                return true;
        }

        return false;
    }

    /**
     * @param e 
     * @param stay_within 
     */
    void Object::dispatch_event(Core::Event& e, Object* stay_within)
    {
        VERIFY(!stay_within || stay_within == this || stay_within->is_ancestor_of(*this));
        auto* target = this;
        do {
            if (target->m_event_filter && !target->m_event_filter(e))
                return;
            target->event(e);
            target = target->parent();

            if (target == stay_within) {
                return;
            }

        } while (target && !e.is_accepted());
    }

    /**
     * @return true 
     * @return false 
     */
    bool Object::is_visible_for_timer_purposes() const
    {
        if (parent())
            return parent()->is_visible_for_timer_purposes();

        return true;
    }

    void Object::increment_inspector_count(Badge<InspectorServerConnection>)
    {
        ++m_inspector_count;
        if (m_inspector_count == 1)
            did_begin_inspection();
    }

    void Object::decrement_inspector_count(Badge<InspectorServerConnection>)
    {
        --m_inspector_count;
        if (!m_inspector_count)
            did_end_inspection();
    }

    /**
     * @param name 
     * @param getter 
     * @param setter 
     */
    void Object::register_property(String const& name, Function<JsonValue()> getter, Function<bool(JsonValue const&)> setter)
    {
        m_properties.set(name, make<Property>(name, move(getter), move(setter)));
    }

    /**
     * @param filter 
     */
    void Object::set_event_filter(Function<bool(Core::Event&)> filter)
    {
        m_event_filter = move(filter);
    }

    static HashMap<StringView, ObjectClassRegistration*>& object_classes()
    {
        static HashMap<StringView, ObjectClassRegistration*> s_map;
        return s_map;
    }

    /**
     * @brief Construct a new ObjectClassRegistration::ObjectClassRegistration object
     * 
     * @param class_name 
     * @param factory 
     * @param parent_class 
     */
    ObjectClassRegistration::ObjectClassRegistration(StringView class_name, Function<RefPtr<Object>()> factory, ObjectClassRegistration* parent_class)
        : m_class_name(class_name)
        , m_factory(move(factory))
        , m_parent_class(parent_class)
    {
        object_classes().set(class_name, this);
    }

    /**
     * @param base_class 
     * @return true 
     * @return false 
     */
    bool ObjectClassRegistration::is_derived_from(ObjectClassRegistration const& base_class) const
    {
        if (&base_class == this)
            return true;

        if (!m_parent_class)
            return false;

        return m_parent_class->is_derived_from(base_class);
    }

    /**
     * @param callback 
     */
    void ObjectClassRegistration::for_each(Function<void(ObjectClassRegistration const&)> callback)
    {
        for (auto& it : object_classes()) {
            callback(*it.value);
        }
    }

    /**
     * @param class_name 
     * @return ObjectClassRegistration const* 
     */
    ObjectClassRegistration const* ObjectClassRegistration::find(StringView class_name)
    {
        return object_classes().get(class_name).value_or(nullptr);
    }
    
} // namespace Core
