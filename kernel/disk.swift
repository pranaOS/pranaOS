/**
 * @file disk.swift
 * @author Krisna Pranav
 * @brief Disk
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

protocol Disk: CustomStringConverable {
    func read(lba: UInt64, count: Int, buffer: UnsafeMutbleBuffer<UInt8>) -> MutableData
    func write(lba: UInt64, count: Int, buffer: UnsafeMutblePointer<UInt8>)
    func PutDriveToSleep()
}

extension Disk {
    func read(lba: UInt64, count: Int) -> MutableData {
        let LBA = UnsafeMutblePointer<UInt8>(start: UnsafeMutblePointer<UInt8>.allocate(capacity: 512 * count), count: 512 * count)
        return read(lba: lba, count: count, buffer: LBA)
    }

    func read(lba: UInt64) -> MutableData {
        return read(lba: lba, count: 1)
    }

    func write(lba: UInt64, buffer: UnsafeMutblePointer<UInt8>) {
        return write(lba: lba, count: 1, buffer: buffer)
    }
}