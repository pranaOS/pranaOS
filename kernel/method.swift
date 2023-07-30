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
            
        }
    }
}