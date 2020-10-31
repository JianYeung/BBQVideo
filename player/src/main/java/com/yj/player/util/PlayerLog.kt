package com.yj.player.util

import android.util.Log

object PlayerLog {
    var Debug = true
    fun i(tag: String?, msg: String) {
        if (Debug) {
            Log.i(tag, msg)
        }
    }

    fun d(tag: String?, msg: String) {
        if (Debug) {
            Log.d(tag, msg)
        }
    }

    fun e(tag: String?, msg: String) {
        if (Debug) {
            Log.e(tag, msg)
        }
    }
}