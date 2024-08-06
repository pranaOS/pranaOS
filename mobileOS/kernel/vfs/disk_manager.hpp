/**
 * @file disk_manager.hpp
 * @author Krisna Pranav
 * @brief DiskManager
 * @version 6.0
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "defs.hpp"
#include "partition.hpp"
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace cpp_freertos
{
    class MutexRecursive;
} // namespace cpp_freertos

namespace purefs::blkdev
{
    class disk;

    class disk_manager
    {
      public:
        /**
         * @brief Construct a new disk manager object
         *
         */
        disk_manager(const disk_manager&) = delete;

        /**
         * @return disk_manager&
         */
        auto operator=(const disk_manager&) -> disk_manager& = delete;

        disk_manager();
        ~disk_manager();

        /**
         * @param disk
         * @param device_name
         * @param flags
         * @return int
         */
        auto register_device(std::shared_ptr<disk> disk, std::string_view device_name, unsigned flags = 0) -> int;

        /**
         * @param device_name
         * @return int
         */
        auto unregister_device(std::string_view device_name) -> int;

        /**
         * @param dfd
         * @param buf
         * @param lba
         * @param count
         * @return int
         */
        auto write(disk_fd dfd, const void* buf, sector_t lba, std::size_t count) -> int;

        /**
         * @param device_name
         * @param buf
         * @param lba
         * @param count
         * @return int
         */
        auto write(std::string_view device_name, const void* buf, sector_t lba, std::size_t count) -> int;

        /**
         * @param dfd
         * @param buf
         * @param lba
         * @param count
         * @return int
         */
        auto read(disk_fd dfd, void* buf, sector_t lba, std::size_t count) -> int;

        /**
         * @param device_name
         * @param buf
         * @param lba
         * @param count
         * @return int
         */
        auto read(std::string_view device_name, void* buf, sector_t lba, std::size_t count) -> int;

        /**
         * @param dfd
         * @param lba
         * @param count
         * @return int
         */
        auto erase(disk_fd dfd, sector_t lba, std::size_t count) -> int;

        /**
         * @param device_name
         * @param lba
         * @param count
         * @return int
         */
        auto erase(std::string_view device_name, sector_t lba, std::size_t count) -> int;

        /**
         * @param dfd
         * @return int
         */
        auto sync(disk_fd dfd) -> int;

        /**
         * @param device_name
         * @return int
         */
        auto sync(std::string_view device_name) -> int;

        /**
         * @param dfd
         * @param target_state
         * @return int
         */
        auto pm_control(disk_fd dfd, pm_state target_state) -> int;

        /**
         * @param device_name
         * @param target_state
         * @return int
         */
        auto pm_control(std::string_view device_name, pm_state target_state) -> int;

        /**
         * @param target_state
         * @return int
         */
        auto pm_control(pm_state target_state) -> int;

        /**
         * @param dfd
         * @param current_state
         * @return int
         */
        auto pm_read(disk_fd dfd, pm_state& current_state) -> int;

        /**
         * @param device_name
         * @param current_state
         * @return int
         */
        auto pm_read(std::string_view device_name, pm_state& current_state) -> int;

        /**
         * @param dfd
         * @return media_status
         */
        [[nodiscard]] auto status(disk_fd dfd) const -> media_status;

        /**
         * @param device_name
         * @return media_status
         */
        [[nodiscard]] auto status(std::string_view device_name) const -> media_status;

        /**
         * @param dfd
         * @return std::vector<partition>
         */
        [[nodiscard]] auto partitions(disk_fd dfd) const -> std::vector<partition>;

        /**
         * @param device_name
         * @return std::vector<partition>
         */
        [[nodiscard]] auto partitions(std::string_view device_name) const -> std::vector<partition>;

        /**
         * @param dfd
         * @return std::optional<partition>
         */
        [[nodiscard]] auto partition_info(disk_fd dfd) const -> std::optional<partition>;

        /**
         * @param device_name
         * @return std::optional<partition>
         */
        [[nodiscard]] auto partition_info(std::string_view device_name) const -> std::optional<partition>;

        /**
         * @param dfd
         * @param what
         * @return scount_t
         */
        [[nodiscard]] auto get_info(disk_fd dfd, info_type what) const -> scount_t;

        /**
         * @param device_name
         * @param what
         * @return scount_t
         */
        [[nodiscard]] auto get_info(std::string_view device_name, info_type what) const -> scount_t;

        /**
         * @param dfd
         * @return int
         */
        auto reread_partitions(disk_fd dfd) -> int;

        /**
         * @param device_name
         * @return int
         */
        auto reread_partitions(std::string_view device_name) -> int;

        /**
         * @param device_name
         * @return disk_fd
         */
        auto device_handle(std::string_view device_name) const -> disk_fd;

        /**
         * @param disk
         * @return disk_fd
         */
        static auto disk_handle_from_partition_handle(disk_fd disk) -> disk_fd;

      private:
        /**
         * @param device
         * @return std::tuple<std::string_view, part_t>
         */
        static auto parse_device_name(std::string_view device) -> std::tuple<std::string_view, part_t>;

        /**
         * @param disk
         * @param part_lba
         * @param count
         * @return scount_t
         */
        static auto part_lba_to_disk_lba(disk_fd disk, sector_t part_lba, size_t count) -> scount_t;

      private:
        std::unordered_map<std::string, std::shared_ptr<disk>> m_dev_map;
        std::unique_ptr<cpp_freertos::MutexRecursive> m_lock;
    }; // class disk_manager
} // namespace purefs::blkdev
