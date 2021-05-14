package com.yj.player.videoPlayer

interface NativeVideoPlayerStatusCallback {
    fun onPrepared()

    fun onProgress(presentationTime: Long)

    fun onError(errorCode: Int)
}