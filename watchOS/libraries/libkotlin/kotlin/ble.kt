package com.libkotlin.kotlin

import java.util.*;

abstract class BLECommand 

data class WriteGattCommand(val uuid: UUID, val writeType: Int, val data: ByteArray, val callback: (Boolean) -> Unit = {}) : BLECommand {
    
}