package com.yj.player.encode

class NativeVideoEncoderProxy {
    private var nativeEncoderHandle: Long = 0

    fun setNativeVideoEncoderHandle(nativeEncoderHandle: Long) {
        this.nativeEncoderHandle = nativeEncoderHandle
    }
}