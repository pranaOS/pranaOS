//
//  Function.hpp
//  Libraries
//
//  Created by Pranav on 28/06/22.
//

#pragma once

#include "Common.hpp"
#include "Runtime.hpp"

template <typename>
class Function;

template <typename Result, typename... Args>
class Function<Result(Args...)> {
public:
    Function() = default;
    Function(nullptr_t) { }

    template <typename Functor>
    Function(Functor functor)
        : m_invoke_ptr(reinterpret_cast<invoke_ptr_t>(invoke<Functor>))
        , m_create_ptr(reinterpret_cast<create_ptr_t>(create<Functor>))
        , m_destroy_ptr(reinterpret_cast<destroy_ptr_t>(destroy<Functor>))
        , m_storage_size(sizeof(Functor))
        , m_storage_amount(sizeof(Functor))
    {
        if (m_storage_size < stack_size) {
            m_storage = &m_stack;
        } else {
            m_storage = malloc(m_storage_size);
        }
        m_create_ptr(m_storage, &functor);
    }

    ~Function()
    {
        m_destroy_ptr(m_storage);
        if (m_storage != &m_stack) {
            free(m_storage);
        }
    }

    Function(const Function& other)
    {
        if (other.m_storage != nullptr) {
            m_invoke_ptr = other.m_invoke_ptr;
            m_create_ptr = other.m_create_ptr;
            m_destroy_ptr = other.m_destroy_ptr;

            m_storage_size = other.m_storage_size;
            m_storage_amount = other.m_storage_amount;

            if (m_storage_size < stack_size) {
                m_storage = &m_stack;
            } else {
                m_storage = malloc(other.m_storage_size);
            }

            m_create_ptr(m_storage, other.m_storage);
        }
    }

    Function& operator=(const Function& other)
    {
        if (m_storage != nullptr) {
            m_destroy_ptr(m_storage);
        }

        if (other.m_storage != nullptr) {
            if (other.m_storage_size < stack_size) {
                m_storage = m_stack;
            } else {
                if (m_storage_amount < other.m_storage_size) {
                    if (m_storage != m_stack) {
                        free(m_storage);
                    }
                    m_storage = malloc(other.m_storage_size);
                    m_storage_amount = other.m_storage_size;
                }
            }

            m_storage_size = other.m_storage_size;

            m_invoke_ptr = other.m_invoke_ptr;
            m_create_ptr = other.m_create_ptr;
            m_destroy_ptr = other.m_destroy_ptr;

            m_create_ptr(m_storage, other.m_storage);
        }

        return *this;
    }

    operator bool() const
    {
        return m_invoke_ptr != nullptr;
    }

    Result operator()(Args&&... args) const
    {
        return m_invoke_ptr(m_storage, forward<Args>(args)...);
    }

    template <typename Functor>
    static Result invoke(Functor* functor, Args&&... args)
    {
        return (*functor)(forward<Args>(args)...);
    }

    template <typename Functor>
    static void create(Functor* destination, Functor* source)
    {
        new (destination) Functor(*source);
    }

    template <typename Functor>
    static void destroy(Functor* functor)
    {
        functor->~Functor();
    }

private:
    using invoke_ptr_t = Result (*)(void*, Args&&...);
    using create_ptr_t = void (*)(void*, void*);
    using destroy_ptr_t = void (*)(void*);

    invoke_ptr_t m_invoke_ptr {};
    create_ptr_t m_create_ptr {};
    destroy_ptr_t m_destroy_ptr {};

    void* m_storage {};
    size_t m_storage_size {};
    size_t m_storage_amount {};

    static constexpr size_t stack_size = 64;
    char m_stack[stack_size];
};
