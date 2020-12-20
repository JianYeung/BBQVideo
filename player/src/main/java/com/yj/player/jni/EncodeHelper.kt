package com.yj.player.jni

import com.yj.player.encode.NativeVideoEncoderProxy

object EncodeHelper {

    fun createVideoEncoderHandle(softwareEncodeEnable: Boolean): NativeVideoEncoderProxy {
        val videoEncoderImpl = NativeVideoEncoderProxy()
        val nativeVideoEncodeHandle = nativeCreateVideoEncoder(softwareEncodeEnable)
        videoEncoderImpl.setNativeVideoEncoderHandle(nativeVideoEncodeHandle)
        return videoEncoderImpl
    }

    private external fun nativeCreateVideoEncoder(softwareEncodeEnable: Boolean): Long

}