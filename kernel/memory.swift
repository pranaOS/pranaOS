/**
 * @file memory.swift
 * @author Krisna Pranav
 * @brief Memory
 * @version 1.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

typealias Data = UnsafeBufferPointer<UInt8>
typealias MutableData = UnsafeMutableBufferPointer<UInt8>

final class MemoryBuffer {
    let ptr: UnsafeRawPointer
    let buffer: UnsafeBufferPointer<UInt8>

    var offset: Int = 0
    var bytesRemaining: Int {
        return (buffer.count - offset)
    }

    init(_ basePtr: UnsafeRawPointer, size: Int) {
        ptr = basePtr
        let bufferPtr = ptr.bindMemory(to: UInt8.self, capacity: size)
        buffer = UnsafeBufferPointer(start: bufferPtr, count: size)
    }

    init(_ baseAddr: UInt, size: Int) {
        ptr = UnsafeRawPointer(bitPattern: baseAddr)!
        let bufferPtr = ptr.bindMemory(to: UInt8.self, capacity: size)
        buffer = UnsafeBufferPointer(start: bufferPtr, count: size)
    }

    func read<T>() -> T? {
        guard bytesRemaining > 0 else {
            return nil
        }

        guard bytesRemaining >= MemoryLayout<T>.size else {
            return nil
        }

        let result = ptr.advanced(by: offset).assumingMemoryBound(to: T.self).pointee
        return result
    }
}