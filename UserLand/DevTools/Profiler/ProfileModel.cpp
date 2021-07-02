/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "ProfileModel.h"
#include "Profile.h"
#include <AK/StringBuilder.h>
#include <LibGUI/FileIconProvider.h>
#include <ctype.h>
#include <stdio.h>

namespace Profiler {

ProfileModel::ProfileModel(Profile& profile)
    : m_profile(profile)
{
    m_user_frame_icon.set_bitmap_for_size(16, Gfx::Bitmap::load_from_file("/res/icons/16x16/inspector-object.png"));
    m_kernel_frame_icon.set_bitmap_for_size(16, Gfx::Bitmap::load_from_file("/res/icons/16x16/inspector-object-red.png"));
}

ProfileModel::~ProfileModel()
{

}

GUI::ModelIndex ProfileModel::index(int row, int column, const GUI::ModelIndex& parent) const
{
    if (!parent.is_valid()) {
        if (m_profile.roots().is_empty())
            return {};
        return create_index(row, column, m_profile.roots().at(row).ptr());
    }
    auto& remote_parent = *static_cast<ProfileNode*>(parent.internal_data());
    return create_index(row, column, remote_parent.children().at(row).ptr());
}


GUI::ModelIndex ProfileModel::parent_index(const GUI::ModelIndex& index) const
{
    if (!index.is_valid())
        return {};
    auto& node = *static_cast<ProfileNode*>(index.internal_data());
    if (!node.parent())
        return {};

    if (!node.parent()->parent()) {
        for (size_t row = 0; row < m_profile.roots().size(); ++row) {
            if (m_profile.roots()[row].ptr() == node.parent()) {
                return create_index(row, index.column(), node.parent());
            }
        }
        VERIFY_NOT_REACHED();
        return {};
    }

    for (size_t row = 0; row < node.parent()->parent()->children().size(); ++row) {
        if (node.parent()->parent()->children()[row].ptr() == node.parent())
            return create_index(row, index.column(), node.parent());
    }

    VERIFY_NOT_REACHED();
    return {};
}


}