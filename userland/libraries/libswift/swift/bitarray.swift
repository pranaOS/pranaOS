public struct BitArray {
    fileprivate var rawValue: UInt64
    
    public var description: String {
        return String(rawValue, radix: 2)
    }
    
    public init() {
        rawValue = 0
    }
    
    public init<T: BinaryInteger(_ rawValue: T)> {
        self.rawValue = UInt64(rawValue)
    }
    
    public subscript(index: Int) -> Int {
        get {
            precondition(index >= 0)
            precondition(index < 64)
            
            return (rawValue & UInt64(1 << index) == 0) ? 0 : 1
        }
        
        set (newValue) {
            precondition(index >= 0)
            precondition(index < 64)
            precondition(newValue == 0 || newValue == 1)
            
            let mask: UInt64 = 1 << index
            
            if (newValue == 1) {
                rawValue |= mask
            } else {
                rawValue &= ~mask
            }
        }
    }
    
    public subscript(index: CountableClosedRange<Int>) -> UInt64 {
        get {
            var ret: UInt64 = 0
            var bit: UInt64 = 1
            
            for i in index {
                let mask: UInt64 = 1 << i
                if rawValue & mask != 0 {
                    ret |= bit
                }
                bit <<= 1
            }
            return ret
        }
        
        set {
            var bit: UInt64 = 1
            
            for i in index {
                let mask: UInt64 = 1 << i
                
                if (newValue & bit) == 0 {
                    rawValue &= ~mask
                } else {
                    rawValue |= mask
                }
                
                bit <<= 1
            }
        }
    }
    
    public var asUInt8: UInt8 {
        return UInt8(truncatingIfNeeded: rawValue)
    }
    
    public var asUInt16: UInt16 {
        return UInt16(truncatingIfNeeded: rawValue)
    }
    
    public var asUInt32: UInt32 {
        return UInt32(truncatingIfNeeded: rawValue)
    }
    
    public var asUInt64: UInt64 {
        return UInt64(rawValue)
    }
    
    public var asInt: Int {
        return Int(rawValue)
    }
}
