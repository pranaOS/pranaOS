/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <kernel/filesystem/Custody.h>
#include <kernel/filesystem/VirtualFileSystem.h>
#include <kernel/KLexicalPath.h>
#include <kernel/Process.h>

namespace Kernel {

static void update_intermediate_node_permissions(UnveilNode& root_node, UnveilAccess new_permissions)
{
    for (auto& entry : root_node.children()) {
        auto& node = static_cast<UnveilNode&>(*entry.value);
        if (node.was_explicitly_unveiled())
            continue;
        node.set_metadata({ node.path(), new_permissions, node.was_explicitly_unveiled() });
        update_intermediate_node_permissions(node, new_permissions);
    }
}

KResultOr<FlatPtr> Process::sys$unveil(Userspace<const Syscall::SC_unveil_params*> user_params)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    Syscall::SC_unveil_params params;
    if (!copy_from_user(&params, user_params))
        return EFAULT;

    if (!params.path.characters && !params.permissions.characters) {
        m_veil_state = VeilState::Locked;
        return 0;
    }

    if (m_veil_state == VeilState::Locked)
        return EPERM;

    if (!params.path.characters || !params.permissions.characters)
        return EINVAL;

    if (params.permissions.length > 5)
        return EINVAL;

    auto path_or_error = get_syscall_path_argument(params.path);
    if (path_or_error.is_error())
        return path_or_error.error();
    auto& path = *path_or_error.value();

    if (path.is_empty() || !path.view().starts_with('/'))
        return EINVAL;

    OwnPtr<KString> permissions;
    {
        auto permissions_or_error = try_copy_kstring_from_user(params.permissions);
        if (permissions_or_error.is_error())
            return permissions_or_error.error();
        permissions = permissions_or_error.release_value();
    }

    // Let's work out permissions first...
    unsigned new_permissions = 0;
    for (const char permission : permissions->view()) {
        switch (permission) {
        case 'r':
            new_permissions |= UnveilAccess::Read;
            break;
        case 'w':
            new_permissions |= UnveilAccess::Write;
            break;
        case 'x':
            new_permissions |= UnveilAccess::Execute;
            break;
        case 'c':
            new_permissions |= UnveilAccess::CreateOrRemove;
            break;
        case 'b':
            new_permissions |= UnveilAccess::Browse;
            break;
        default:
            return EINVAL;
        }
    }

    RefPtr<Custody> parent_custody; 
    OwnPtr<KString> new_unveiled_path;
    auto custody_or_error = VirtualFileSystem::the().resolve_path_without_veil(path.view(), root_directory(), &parent_custody);
    if (!custody_or_error.is_error()) {
        new_unveiled_path = custody_or_error.value()->try_create_absolute_path();
        if (!new_unveiled_path)
            return ENOMEM;
    } else if (custody_or_error.error() == -ENOENT && parent_custody && (new_permissions & UnveilAccess::CreateOrRemove)) {
        auto parent_custody_path = parent_custody->try_create_absolute_path();
        if (!parent_custody_path)
            return ENOMEM;
        new_unveiled_path = KLexicalPath::try_join(parent_custody_path->view(), KLexicalPath::basename(path.view()));
        if (!new_unveiled_path)
            return ENOMEM;
    } else {

        return custody_or_error.error();
    }

    auto path_parts = KLexicalPath::parts(new_unveiled_path->view());
    auto it = path_parts.begin();
    auto& matching_node = m_unveiled_paths.traverse_until_last_accessible_node(it, path_parts.end());
    if (it.is_end()) {

        if (matching_node.was_explicitly_unveiled()) {
            if (new_permissions & ~matching_node.permissions())
                return EPERM;
        }


        if (matching_node.permissions() != new_permissions)
            update_intermediate_node_permissions(matching_node, (UnveilAccess)new_permissions);

        matching_node.set_metadata({ matching_node.path(), (UnveilAccess)new_permissions, true });
        m_veil_state = VeilState::Dropped;
        return 0;
    }

    matching_node.insert(
        it,
        path_parts.end(),
        { new_unveiled_path->view(), (UnveilAccess)new_permissions, true },
        [](auto& parent, auto& it) -> Optional<UnveilMetadata> {
            auto path = String::formatted("{}/{}", parent.path(), *it);
            return UnveilMetadata { path, parent.permissions(), false };
        });

    VERIFY(m_veil_state != VeilState::Locked);
    m_veil_state = VeilState::Dropped;
    return 0;
}

}
