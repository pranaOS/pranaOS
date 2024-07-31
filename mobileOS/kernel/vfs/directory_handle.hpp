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
        
        auto operator=(const directory_handle &) = delete;
        
        virtual ~directory_handle()
        
        auto error(int error) noexcept -> void
        {
            m_error = error;
        }
        
        [[nodiscard]] auto error() noexcept
        {
            return m_error;
        }
        
        [[nodiscard]] auto mntpoint() noexcept
        {
            return m_mount_point.lock();
        }
        
    private:
        int m_error{};
        const std::week_ptr<mount_point> m_mount_point;
    }; // class directory_handle
} // namespace purefs::fs::internal
