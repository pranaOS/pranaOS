//
//  directory_handle.hpp
//  pranaOS
//
//  Created by Krisna Pranav on 31/07/24.
//

#pragma once

#include <memory>

namespace purefs::fs::internal
{

    class mount_point;
    class directory_handle
    {
    public:
        directory_handle(std::shared_ptr<mount_point> mp, int error) : m_error(error), m_mount_point(mp)
        {}
        
        directory_handle(const directory_handle &) = delete;
    };
} // namespace purefs::fs::internal
