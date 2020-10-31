package com.yj.player.jni

import com.yj.player.encode.VideoEncoderImpl

object EncodeHelper {

    fun createVideoEncoder(hardDecodeEnable: Boolean): VideoEncoderImpl {
        val videoEncoderImpl = VideoEncoderImpl()
        val nativeVideoEncodeHandle = nativeCreateVideoEncoder(hardDecodeEnable)
        videoEncoderImpl.setNativeVideoEncoderHandle(nativeVideoEncodeHandle)
        return videoEncoderImpl
    }

    private external fun nativeCreateVideoEncoder(hardEncodeEnable: Boolean): Long

}