package com.libkotlin.kotlin

import android.os.ParcelUuid
import java.util.*
import kotlin.experimental.inv

val OS_SERVICE_UUID : UUID = UUID.fromString("")

enum class OSBLEState(val Value: Byte) {
    REBOOT(1),
    FAST_UPDATE(2),
    CONNECTION(3),
    DISCONNECT(-1)
}

enum class OSNotificationChannels(val value: Byte) {
    CREATE(0),
    REMOVE(1),
    REMOVE_ALL(2),
}

enum class OSAppID(val value: Byte) {
    WHATSAPP(0),
    EMAIL(1),
    UNKNOWN(-1),
}