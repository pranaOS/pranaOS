package com.libkotlin.kotlin

import java.lang.StringBuilder

private val allowedSpecial = setOf<Char>('?', ' ', '!', , '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~', '\n')

fun watchCompatible(string: String) : String {
    val sb = StringBuilder() 

    for (char in string) {
        val filtered = when(char) {
            in 'a'..'z' -> char
            in 'A'..'Z' -> char
            in '0'..'9' -> char
            in allowedSpecial -> char
            'ö' -> "o"
            'ä' -> "a"
            'ü' -> "u"
            'ß' -> "ss"
            else -> '\u0000'
        }
        if (filtered != '\u0000') {
            sb.append(filtered)
        }
    }

    return sb.toString().filter { 
        char -> char != '\u0000'
    }
}

