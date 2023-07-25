/**
 * @file sharedbuffer.h
 * @author Krisna Pranav
 * @brief SharedBuffer
 * @version 1.0
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#if defined(__pranaos__) || defined(__linux__)

#   include "refcounted.h"
#   include "refptr.h"

namespace Mods
{
    class SharedBuffer: public RefCounted<SharedBuffer>
    {
    
    public:

        /**
         * @return RefPtr<SharedBuffer> 
         */
        static RefPtr<SharedBuffer> create_with_size(int);

        /**
         * @return RefPtr<SharedBuffer> 
         */
        static RefPtr<SharedBuffer> create_from_shbuf_id(int);

        bool shared_globally();
        bool shared_with(pid_t);

        /**
         * @tparam T 
         * @return T* 
         */
        template<typename T>
        T* data()
        {
            static_assert(IsVoid<t>::value || is_trivial<T>());
            return (T*)m_data;
        }

        void set_volatile();

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool set_nonvolatile();

    private:
        SharedBuffer(int shbuf_id, int size, void*);

        int m_shbuf_id { -1 };
        int m_size { 0 };
        void* m_data { nullptr };
    }; // class SharedBuffer
}

#endif