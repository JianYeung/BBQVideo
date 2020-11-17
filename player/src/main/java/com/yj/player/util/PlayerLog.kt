package com.yj.player.util

import android.util.Log

object PlayerLog {
    private var DEBUG = true
    private val TAG = "PlayerLog"

    fun i(tag: String?, msg: String) {
        if (DEBUG) {
            if (tag == null) {
                Log.i(TAG, msg)
            } else {
                Log.i(tag, msg)
            }
        }
    }

    fun i(tag: String?, msg: String, throwable: Throwable) {
        if (DEBUG) {
            if (tag == null) {
                Log.i(TAG, msg, throwable)
            } else {
                Log.i(tag, msg, throwable)
            }
        }
    }

    fun d(tag: String?, msg: String) {
        if (DEBUG) {
            if (tag == null) {
                Log.d(TAG, msg)
            } else {
                Log.d(tag, msg)
            }
        }
    }

    fun d(tag: String?, msg: String, throwable: Throwable) {
        if (DEBUG) {
            if (tag == null) {
                Log.d(TAG, msg, throwable)
            } else {
                Log.d(tag, msg, throwable)
            }
        }
    }

    fun e(tag: String?, msg: String) {
        if (DEBUG) {
            if (tag == null) {
                Log.e(TAG, msg)
            } else {
                Log.e(tag, msg)
            }
        }
    }

    fun e(tag: String?, msg: String, throwable: Throwable) {
        if (DEBUG) {
            if (tag == null) {
                Log.e(TAG, msg, throwable)
            } else {
                Log.e(tag, msg, throwable)
            }
        }
    }
}