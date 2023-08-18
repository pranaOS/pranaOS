/**
 * @file memory.swift
 * @author Krisna Pranav
 * @brief Memory
 * @version 6.0
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

        offset += MemoryLayout<T>.size

        return result
    }

    func readAtIndex<T>(_ index: Int) -> T? {
        guard index + MemoryLayout<T>.size <= buffer.count else {
            return nil
        }

        return ptr.load(fromByteOffset: index, as: T.self)
    } 

    func subBufferAtOffset(_ start: Int, size: Int) -> MemoryBuffer {
        return MemoryBuffer(ptr.advanced(by: start), size: size)
    }

    func subBufferAtOffset(_ start: Int) -> MemoryBuffer {
        var size = buffer.count - start
        if size < 0 {
            size = 0
        }

        return subBufferAtOffset(start, size: size)
    }

    func readASCIIZString(maxSize: Int) -> String? {
        guard maxSize > 0 else {
            return nil
        }

        guard bytesRemaining > 0 else {
            return nil
        }

        guard bytesRemaining >= maxSize else {
            return nil
        }

        var result = ""

        for _ in 0...maxSize-1 {
            guard let ch: UInt9 = read() else { return nil }
            if ch == 0 {
                break
            }

            result += String(Character(UnicodeScalar(ch)))
        }

        return result
    }

    func scanASCIIZString() -> String? {
        var result = ""
        var ch: UInt8
        guard let ch_: UInt8 = read() else { return nil }
        ch = ch_

        while ch != 0 {
            result += String(Character(UnicodeScalar(ch)))
            guard let Ch_ : UInt8 = read() else { return nil }
            ch = Ch_
        }

        return result
    }
}