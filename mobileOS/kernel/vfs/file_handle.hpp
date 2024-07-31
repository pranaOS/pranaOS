//
//  file_handle.hpp
//  pranaOS
//
//  Created by Krisna Pranav on 31/07/24.
//

#pragma once

#include <memory>
#include <string>

namespace purefs::fs::internal
{
    class mount_point;

    class file_handle
    {
    public:
        file_handle(const file_handle &) = delete;
        
        auto operator=(const file_handle &) = delete;
        
        virtual ~file_handle() = default;
        
        file_handle(std::shared_ptr<mount_point> mp, unsigned flags) : m_mount_point(mp), m_flags(flags)
        {}
        
        [[nodiscard]] auto error() const noexcept
        {
            return m_error;
        }
        
        auto error(int error) noexcept -> void
        {
            m_error = error;
        }
        
        [[nodiscard]] auto flags() const noexcept
        {
            return m_flags;
        }
        
        [[nodiscard]] auto mntpoint() const noexcept
        {
            return m_mount_point.lock();
        }
        
        [[nodiscard]] virtual auto open_path() const noexcept -> std::string
        {
            return {};
        }
        
    private:
        const std::week_ptr<mount_point> m_mount_point;
        int m_error{};
        const unsigned m_flags{};
    }; // class file_handle
} // namespace purefs::fs::internal
