/**
 * @file custody.h
 * @author Krisna Pranav
 * @brief custody
 * @version 6.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/forward.h>
#include <kernel/heap/slaballocator.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/string.h>

namespace Kernel 
{
    class Custody : public RefCounted<Custody> 
    {
        MAKE_SLAB_ALLOCATED(Custody)
    public:

        /**
         * @param parent 
         * @param name 
         * @param inode 
         * @param mount_flags 
         * @return NonnullRefPtr<Custody> 
         */
        static NonnullRefPtr<Custody> create(Custody* parent, const StringView& name, Inode& inode, int mount_flags)
        {
            return adopt(*new Custody(parent, name, inode, mount_flags));
        }

        /// @brief Destroy the Custody object
        ~Custody();

        /**
         * @return Custody* 
         */
        Custody* parent() 
        { 
            return m_parent.ptr(); 
        }

        /**
         * @return const Custody* 
         */
        const Custody* parent() const 
        { 
            return m_parent.ptr(); 
        }

        /**
         * @return Inode& 
         */
        Inode& inode() 
        { 
            return *m_inode; 
        }

        /**
         * @return const Inode& 
         */
        const Inode& inode() const 
        { 
            return *m_inode; 
        }

        /**
         * @return const String& 
         */
        const String& name() const 
        { 
            return m_name; 
        }

        /**
         * @return String 
         */
        String absolute_path() const;

        /**
         * @return int 
         */
        int mount_flags() const 
        { 
            return m_mount_flags; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_readonly() const;

    private:

        /**
         * @param parent 
         * @param name 
         * @param mount_flags 
         */
        Custody(Custody* parent, const StringView& name, Inode&, int mount_flags);

        RefPtr<Custody> m_parent;

        String m_name;

        NonnullRefPtr<Inode> m_inode;

        int m_mount_flags { 0 };

    }; // class Custody 

} // namespace Kernel
