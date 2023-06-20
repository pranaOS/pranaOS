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
    
    public var BigEndiansInt: Int {
        return Int(bigEndian: Int(rawValue))
    }
    
    public var LittleEndiansInt: Int {
        return Int(littleEndian: Int(rawValue))
    }
    
    public var asUInt: UInt {
        return UInt(rawValue)
    }
    
    public var BigEndiansUInt: UInt {
        return UInt(bigEndian: UInt(rawValue))
    }
    
    public var LittleEndiansUInt: UInt {
        return UInt(littleEndian: UInt(rawValue))
    }
    
    public var asUInt16: UInt16 {
        return UInt16(rawValue)
    }
    
    public var BigEndianasUInt16: UInt16 {
        return UInt16(bigEndian: UInt16(rawValue))
    }
    
    public var LittleEndianasUInt16: UInt16 {
        return UInt16(littleEndian: UInt16(rawValue))
    }
    
    public var asUInt32: UInt32 {
        return UInt32(rawValue)
    }
    
    public var BigEndiansUint32: UInt32 {
        return UInt32(bigEndian: UInt32(rawValue))
    }
    
    public var LittleEndiansUInt32: UInt32 {
        return UInt32(littleEndian: UInt32(rawValue))
    }
    
    public var asUInt64: UInt64 {
        return rawValue
    }
    
    public var BigEndiansUInt64: UInt64 {
        return UInt64(bigEndian: rawValue)
    }
    
    public var LittleEndiansUInt64: UInt64 {
        return UInt64(littleEndian: rawValue)
    }
    
    public init() {
        rawValue = 0
    }
    
    public init<T: BinaryInteger>(_ rawValue: T) {
        self.rawValue = UInt64(rawValue)
        isEmpty = false
        count = MemoryLayout<T>.size / MemoryLayout<UInt8>.size
        endIndex = count
    }
    
    public init(withBytes bytes: Element...) {
        precondition(bytes.count <= 8)
        count = bytes.count
        endIndex = count
        self.rawValue = 0
        var shift: UInt16 = 0
        
        for byte in bytes {
            rawValue |= (UInt64(byte) << shift)
            shift += 8
        }
    }
    
}
