/**
 * @file disk.hpp
 * @author Krisna Pranav
 * @brief disk
 * @version 6.0
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2021 - 2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "defs.hpp"
#include "partition.hpp"
#include <vector>

namespace purefs::blkdev
{
    class disk
    {
      public:
        disk() = default;
        disk(const disk &) = delete;
        auto operator=(const disk &) = delete;
        virtual ~disk() = default;
        
        virtual auto probe(unsigned flags) -> int = 0;
        
        virtual auto cleanup() -> int;
        
        virtual auto write(const void *buf, sector_t lba, std::size_t count, hwpart_t hwpart) -> int = 0;
        
        virtual auto read(void *buf, sector_t lba, std::size_t count, hwpart_t hwpart) -> int = 0;
        
        virtual auto erase(sector_t lba, std::size_t count, hwpart_t hwpart) -> int;
        
        virtual auto sync() -> int;
        
        virtual auto pm_control(pm_state target_state) -> int;
        
        virtual auto pm_read(pm_state &current_state) -> int;
        
        [[nodiscard]] virtual auto status() const -> media_status = 0;
        
        [[nodiscard]] virtual auto get_info(info_type what, hwpart_t hwpart) const -> scount_t = 0;
        
        [[nodiscard]] auto partitions() -> std::vector<partition> &
        {
            return m_partitions;
        }
        
    private:
        std::vector<partition> m_partitions;
    }; // class disk

} // namespace purefs::blkdev
