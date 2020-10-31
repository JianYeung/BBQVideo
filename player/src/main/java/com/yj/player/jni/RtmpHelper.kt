package com.yj.player.jni

object RtmpHelper {

    fun setLiveUrl(url: String): Boolean {
        return nativeSetLiveUrl(url)
    }

    fun initVideo(width: Int, height: Int): Boolean {
        return nativeInitVideo(width, height)
    }

    fun startStream(): Boolean {
        return nativeStartStream()
    }

    fun stopStream(): Boolean {
        return nativeStopStream()
    }

    private external fun nativeSetLiveUrl(url: String): Boolean

    private external fun nativeInitVideo(width: Int, height: Int): Boolean

    private external fun nativeStartStream(): Boolean

    private external fun nativeStopStream(): Boolean
}