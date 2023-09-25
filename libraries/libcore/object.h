/**
 * @file object.h
 * @author Krisna Pranav
 * @brief object
 * @version 6.0
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/forward.h>
#include <mods/hashmap.h>
#include <mods/intrusivelist.h>
#include <mods/noncopyable.h>
#include <mods/string.h>
#include <mods/typecast.h>
#include <mods/weakable.h>
#include <libcore/forward.h>
#include <libcore/property.h>

namespace Core
{

    class RPCClient;

    enum class TimerShouldFireWhenNotVisible
    {
        No = 0,
        Yes,
    };  

    #define C_OBJECT(klass)

    class Object : public RefCounted<Object>, public Weakable<Object>
    {
    public:
        virtual ~Object();

        virtual const char* class_name() const = 0;

        virtual void event(Core::Event&);

        /**
         * @return const String& 
         */
        const String& name() const
        {
            return m_name;
        }

        /**
         * @param name 
         */
        void set_name(const StringView& name)
        {
            m_name = name;
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        void for_each_child(Callback callback)
        {
            for (auto& child : m_children) {
                if (callback(child) == IterationDecision::Break)
                    return;
            }
        }

        /**
         * @tparam T 
         * @tparam Callback 
         * @param callback 
         */
        template<typename T, typename Callback>
        void for_each_child_of_type(Callback callback)

    protected:
        /**
         * @param parent 
         * @param is_widget 
         */
        explicit Object(Object* parent = nullptr, bool is_widget = false);

        /**
         * @param name 
         * @param getter 
         * @param setter 
         */
        void register_property(const String& name, Function<JsonValue()> getter, Function<bool(const JsonValue&)> setter = nullptr);

    private:
        Object* m_parent { nullptr };
        String m_name;
        
        int m_timer_id { 0 };
        unsigned m_inspector_count { 0 };
        bool m_widget { false };

        HashMap<String, NonnullOwnPtr<Property>> m_properties;
        NonnullRefPtrVector<Object> m_children;
    }; // class Object

} // namespace Core