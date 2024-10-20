/**
 * @file child.swift
 * @author Krisna Pranav
 * @brief Children
 * @version 6.0
 * @date 2024-10-20
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

struct Child {
    let name: String
    let kind: String
    let isOptional: Bool

    var isToken: Bool {
        return kind.contains("Token")
    }

    var kindCaseName: String {
        return kind.lowercaseFirstLetter
    }

    init(_ name: String, kind: String, isOptional: Bool = false) {
        self.kind = kind
        self.name = name
        self.isOptional = isOptional
    }
}