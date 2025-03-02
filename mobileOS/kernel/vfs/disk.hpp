/**
 * @file disk.hpp
 * @author Krisna Pranav
 * @brief disk
 * @version 6.0
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2021 - 2025 pranaOS Developers, Krisna Pranav
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

        /**
         * @brief Construct a new disk object
         *
         */
        disk(const disk&) = delete;

        /**
         * @return auto
         */
        auto operator=(const disk&) = delete;

        /**
         * @brief Destroy the disk object
         *
         */
        virtual ~disk() = default;

        /**
         * @param flags
         * @return int
         */
        virtual auto probe(unsigned flags) -> int = 0;

        /**
         * @return int
         */
        virtual auto cleanup() -> int;

        /**
         * @param buf
         * @param lba
         * @param count
         * @param hwpart
         * @return int
         */
        virtual auto write(const void* buf, sector_t lba, std::size_t count, hwpart_t hwpart) -> int = 0;

        /**
         * @param buf
         * @param lba
         * @param count
         * @param hwpart
         * @return int
         */
        virtual auto read(void* buf, sector_t lba, std::size_t count, hwpart_t hwpart) -> int = 0;

        /**
         * @param lba
         * @param count
         * @param hwpart
         * @return int
         */
        virtual auto erase(sector_t lba, std::size_t count, hwpart_t hwpart) -> int;

        /**
         * @return int
         */
        virtual auto sync() -> int;

        /**
         * @param target_state
         * @return int
         */
        virtual auto pm_control(pm_state target_state) -> int;

        /**
         * @param current_state
         * @return int
         */
        virtual auto pm_read(pm_state& current_state) -> int;

        /**
         * @return media_status
         */
        [[nodiscard]] virtual auto status() const -> media_status = 0;

        /**
         * @param what
         * @param hwpart
         * @return scount_t
         */
        [[nodiscard]] virtual auto get_info(info_type what, hwpart_t hwpart) const -> scount_t = 0;

        /**
         * @return std::vector<partition>&
         */
        [[nodiscard]] auto partitions() -> std::vector<partition>&
        {
            return m_partitions;
        }

        auto clear_partitions() -> void
        {
            m_partitions.clear();
        }

      private:
        std::vector<partition> m_partitions;
    }; // class disk
} // namespace purefs::blkdev
