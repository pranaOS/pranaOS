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

    class SharedBuffer : public RefCounted<SharedBuffer> 
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
        ~SharedBuffer();

        bool share_globally();

        /**
         * @return true 
         * @return false 
         */
        bool share_with(pid_t);

        /**
         * @return int 
         */
        int shbuf_id() const 
        { 
            return m_shbuf_id; 
        }
        
        void seal();

        /**
         * @return int 
         */
        int size() const 
        { 
            return m_size; 
        }

        /**
         * @tparam T 
         * @return T* 
         */
        template<typename T>
        T* data()
        {
            static_assert(IsVoid<T>::value || is_trivial<T>());
            return (T*)m_data;
        }

        /**
         * @tparam T 
         * @return const T* 
         */
        template<typename T>
        const T* data() const
        {
            static_assert(IsVoid<T>::value || is_trivial<T>());
            return (const T*)m_data;
        }

        void set_volatile();

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool set_nonvolatile();

    private:
        /**
         * @param shbuf_id 
         * @param size 
         */
        SharedBuffer(int shbuf_id, int size, void*);

        int m_shbuf_id { -1 };
        int m_size { 0 };

        /// @brief m_data
        void* m_data { nullptr };
    }; // class SharedBuffer

} // namespace Mods

using Mods::SharedBuffer;

#endif
