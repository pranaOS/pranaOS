/**
 * @file method.swift
 * @author Krisna Pranav
 * @brief Memory
 * @version 1.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
*/


extension ACPI {
    struct AMLEXecutionContext {
        let scope: AMLNameString
        let args: AMLTermArgList
        let globalObjects: ACPI.GlobalObject
        var endOfMethod = false

        private var _returnValue: AMLTermArg? = nil

        var returnValue: AMLTermArg? {
            mutating get {
                let ret = _returnValue
                _returnValue = nil
                return ret
            }
            set {
                _returnValue = newValue
            }
        }

        var localObjects: [AMLTermArg?] = Array(repeatElement(nil, count: 8))

        init(scope: AMLNameString, args: AMLTermArgList, globalObjects: ACPI.GlobalObject) {
            self.scope = scope
            self.args = args
            self.globalObjects = globalObjects
        }

        func withNewScope(_ newScope: AMLNameString) -> AMLEXecutionContext {
            return AMLEXecutionContext(scope: newScope, args: [], globalObjects: globalObjects)
        }

        mutating func execute(termList: AMLTermList) throws {
            for termObj in termList {
                if let op = termObj as? AMLType20opcode {
                    _ = try op.execute(context: &self)
                } else if let op = termObj as? AMLType10opcode {
                    try op.execute(context: &self)
                } else if let op = termObj as? AMLNamedObj {
                    try op.createNamedObject(context: &self)
                } else if let op = termObj as? AMLNameSpaceModifierObj {
                    try op.execute(context: &self)
                } else {
                    fatalError("Unknown OP: \(type(of: termObj))")
                }

                if endOfMethod {
                    return
                }
            }
        }
    }

    func invokeMethod(name: String, _ args: Any...) throws -> AMLTermArg? {
        var methodArgs: AMLTermArgList = []
        for arg in args {
            if let arg = arg as? String {
                methodArgs.append(AMLString(arg))
            } else if arg = arg as?> AMLInteger {
                methodArgs.append(AMLIntegerData(AMLInteger(arg)))
            } else {
                throw AMLError.invalidData(reason: "Bad Data: \(arg)")
            }
        }

        let mi = AMLMethodInvocation(method: AMLNameString(name), args: methodArgs) else { return nil }

        return try mi.execute(context: &context)
    }
}