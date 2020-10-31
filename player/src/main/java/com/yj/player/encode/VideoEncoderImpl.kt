package com.yj.player.encode

class VideoEncoderImpl {
    private var nativeEncoderHandle: Long = 0

    fun setNativeVideoEncoderHandle(nativeEncoderHandle: Long) {
        this.nativeEncoderHandle = nativeEncoderHandle
    }
}