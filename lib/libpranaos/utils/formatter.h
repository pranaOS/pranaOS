#pragma once

#include <cstdint>
#include <string>
#include <list>
#include <utility>

namespace utils {
    class formatter
    {
    private:
        struct argument
        {
        private:
            uintptr_t _type = 0;
            void* _data = nullptr;

            void* (*_recreate)(void* other) = nullptr;

            template <typename T>
            static uintptr_t get_typeid()
            {
                static int unused;

                return reinterpret_cast<uintptr_t>(&unused);
            }

        public:
            template <typename T>
            argument(const T& data)
            {
                set(data);
            }

            argument(const argument& other)
            {
                _recreate = other._recreate;
                if(_recreate != nullptr) _data = _recreate(other._data);
                _type = other._type;
            }

            argument() { }

            const void* get_raw() const { return _data; }

            template <typename T>
            const T as() const
            {
                return *(const T*)get_raw();
            }

            template <typename T>
            void set(T data)
            {
                _recreate = [](void* other) { return (void*)new T(*(T*)other); };
                _data = new T{std::forward<T>(data)};
                _type = get_typeid<T>();
            }

            template <typename T>
            bool is_type() const
            {
                return get_typeid<T>() == _type;
            }

            ~argument()
            {
                if(_data != nullptr) delete (char*)_data;
            }
        };
        

        struct variable
        {
            std::string name;
            std::string contents;
        };

        std::list<variable> _variables;

    private:
        template <typename T, typename... Args>
        void create_dynamic_arg_list(std::list<argument>* args, T& var, Args... arg_list)
        {
            argument arg(var);

            args->push_back(arg);

            create_dynamic_arg_list(args, arg_list...);
        }

        void create_dynamic_arg_list(std::list<argument>* args)
        {
        }

        std::string format(std::string fmt, std::list<argument> args);

    public:
        void add_variable(const char* name, const char* value);

        template <typename... Args>
        std::string format_string(std::string fmt, Args... arg_list)
        {
            std::list<argument> args;

            create_dynamic_arg_list(&args, arg_list...);

            return format(fmt, std::list(args));
        }
    };
}