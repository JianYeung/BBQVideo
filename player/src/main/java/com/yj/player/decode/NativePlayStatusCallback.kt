package com.yj.player.decode

interface NativePlayStatusCallback {
    fun onPrepared(duration: Long)

    fun onProgress(presentationTime: Long)

    fun onError(errorCode: Int)
}