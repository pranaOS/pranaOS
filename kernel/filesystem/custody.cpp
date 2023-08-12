/**
 * @file custody.cpp
 * @author Krisna Pranav
 * @brief custody
 * @version 1.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/filesystem/custody.h>
#include <kernel/filesystem/inode.h>
#include <mods/string_builder.h>
#include <mods/string_view.h>
#include <mods/vector.h>

namespace Kernel
{

    Custody::Custody(Custody* parent, const StringView& name, Inode& inode, int mount_flags)
        : m_parent(parent)
        , m_name(name)
        , m_inode(inode)
        , m_mount_flags(mount_flags)
    {}

    Custody::~Custody()
    {}
    
    String Custody::absolute_path() const
    {
        if (!parent())
            return "/"
        
        Vector<const Custody*, 32> custody_chain;

        StringBuilder builder;

        return builder.to_string();
    }

    /**
     * @return true 
     * @return false 
     */
    bool Custody::is_readonly() const
    {
        if (m_mount_flags & MS_RDONLY)
            return true;

        return m_inode->fs().is_readonly();
    }

} // namespace Kernel 