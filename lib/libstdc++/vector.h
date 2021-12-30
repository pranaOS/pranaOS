#pragma once

#include <initializer_list>

namespace std
{
    template <typename T>
    class vector
    {
    private:
        T* m_array = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;

    public:
        vector(size_t capacity)
        {
            m_size = 0;
            m_capacity = capacity;
            m_array = new T[m_capacity];
        }

        vector(const vector<T>& old)
        {
            clear();

            for(int i = 0; i < old.size(); i++)
            {
                push_back(old[i]);
            }
        }

        void operator=(const initializer_list<T>& l)
        {
            clear();

            for(const T& item : l)
            {
                push_back(item);
            }
        }

        vector(const initializer_list<T>& l)
        {
            operator=(l);
        }

        vector()
        {
            vector(2);
        }

        ~vector()
        {
            if(m_array != nullptr)
            {
                delete[] m_array;
            }
        }

        size_t capacity() const { return m_capacity; }
        size_t size() const { return m_size; }

        bool empty() const { return m_size == 0; }

        T* data() { return m_array; }
        const T* data() const { return m_array; }

        T& at(size_t index) { return m_array[index]; }
        const T& at(size_t index) const { return m_array[index]; }

        T& operator[](int index)
        {
            return at(index);
        }

        const T& operator[](int index) const
        {
            return at(index);
        }

        void resize(size_t size)
        {
            T* block = new T[size];

            if(m_array != nullptr)
            {
                for (size_t i = 0; i < m_size; i++)
                {
                    block[i] = m_array[i];
                }

                delete[] m_array;
            }

            m_size = size;
            m_capacity = size;
            m_array = block;
        }

        void push_back(const T& data)
        {
            resize(m_size + 1);

            m_array[m_size - 1] = data;
        }

        void push_front(const T& data)
        {
            T* block = new T[m_size + 1];

            if(m_array != nullptr)
            {
                for(int i = 0; i < m_size; i++)
                {
                    block[i + 1] = m_array[i];
                }

                if(m_array != nullptr) { delete[] m_array; }
            }

            m_capacity++;
            m_size++;
            m_array = block;
            m_array[0] = data;
        }

        void clear()
        {
            m_size = 0;
            m_capacity = 0;

            if(m_array != nullptr) delete[] m_array;
        }
    };
}
