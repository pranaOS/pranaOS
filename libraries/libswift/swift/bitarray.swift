public struct BitArray {
    fileprivate var rawValue: UInt64
    
    public var description: String {
        return String(rawValue, radix: 2)
    }
    
    public init() {
        rawValue = 0
    }
}
