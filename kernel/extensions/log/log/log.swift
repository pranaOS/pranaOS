//
//  log.swift
//  log
//
//  Created by krisna pranav on 19/06/23.
//

public protocol Loggable {
    var Name: String { get }
}

public enum LogLevel: CustomStringConvertible {
    case Verbose
    case Debug
    case Info
    case Warning
    case Error
    
    public var description: String {
        get {
            switch self {
            case .Verbose:
                return "Verbose"
            case .Debug:
                return "Debug"
            case .Info:
                return "Info"
            case .Warning:
                return "Warning"
            case .Error:
                return "Error"
            }
        }
    }
}

extension Loggable {
    public func Log(_ items: String..., level: LogLevel, seperator: String = "", terminator: String = "\n") -> Void {
        let color: UInt32
        let isConfigurationValid: Bool
        let item = items.map {"\($0)"}.joined(separator: seperator)
        switch level {
        case .Info:
            color = 0x0000A5FF
            isConfigurationValid = true
        case .Warning:
            color = 0x00FFAE42
            isConfigurationValid = true
        case .Error:
            color = 0x00ED4337
            isConfigurationValid = true
        case .Verbose:
            color = OldColor
            #if DEBUG
                isConfigurationValid = true
            #else
                isConfigurationValid = false
            #endif
        case .Debug:
            color = 0x0009D400
            #if DEBUG
                isConfigurationValid = true
            #else
                isConfigurationValid = false
            #endif
        }
        print(Name + " " + "[" level.description + "]" + ":", item, seperator: seperator, terminator: terminator)
    }
}
