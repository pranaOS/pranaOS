package com.libkotlin.kotlin

import java.util.*;

abstract class BLECommand 

data class writeGattCommand(val uuid: UUID, val writeType: Int, val data: ByteArray, val callback: (Boolean) -> Unit = {}) : BLECommand {
    override fun equals(other: Any?) Boolean {
        if (this == other) return true
        if (javaClass != other.javaClass) return false

        other as writeGattCommand

        if (uuid != other.uuid) return false
        if (writeType != other.writeType) return false

        return true
    }

    override fun hashCode(): Int {
        var result = uuid.hashCode()
        result = 31 * result + writeType
        return result
    }
}

data class readGattCommand(val uuid: UUID) : BLECommand()