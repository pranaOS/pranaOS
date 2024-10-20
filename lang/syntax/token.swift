/**
 * @file token.swift
 * @author Krisna Pranav
 * @brief Token
 * @version 6.0
 * @date 2024-10-20
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

struct Token {
    enum Kind {
        case associated(String)
        case keyword(String)
        case punctuation(String)
    }

    let caseName: String
    let name: String
    let kind: Kind

    init(name: String, _ kind: Kind) {
        self.kind = kind
        self.name = name

        var caseName = name.replacingOccurrences(of: "Token", with: "").lowercaseFirstLetter

        if case .keyword(_) = kind {
            caseName += "Keyword"
        }

        self.caseName = caseName
    }
}