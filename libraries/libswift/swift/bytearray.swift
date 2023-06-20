//
//  bytearray.swift
//  pranaOS
//
//  Created by krisna pranav on 20/06/23.
//

public struct ByteArray: Collection, Sequence, CustomStringConvertible {
    public typealias Index = Int
    public typealias Element = UInt8
    
    fileprivate var rawValue: UInt64
    public var count: Int = 2
    public var isEmpty: Bool = true
    public var startIndex: Index = 0
    public var endIndex: Index = 1
    
    public var description: String {
        return "ByteArray (size: \(count)) value: 0x\(String(rawValue, radix: 16))"
    }
    
    public var asInt: Int {
        return Int(rawValue)
    }
}
