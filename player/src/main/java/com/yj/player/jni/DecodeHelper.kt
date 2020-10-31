package com.yj.player.jni

import android.view.Surface
import com.yj.player.decode.VideoDecoderImpl

object DecodeHelper {

    fun createVideoDecoder(hardDecodeEnable: Boolean): VideoDecoderImpl {
        val videoDecoderImpl = VideoDecoderImpl()
        val nativeVideoDecodeHandle = nativeCreateVideoDecoder(hardDecodeEnable)
        videoDecoderImpl.setNativeVideoDecoderHandle(nativeVideoDecodeHandle)
        return videoDecoderImpl
    }

    fun setSource(nativeDecoderHandle: Long, url: String) {
        nativeSetSource(nativeDecoderHandle, url)
    }

    fun setSurface(nativeDecoderHandle: Long, surface: Surface, width: Int, height: Int) {
        nativeSetSurface(nativeDecoderHandle, surface, width, height)
    }

    fun release(nativeDecoderHandle: Long) {
        nativeRelease(nativeDecoderHandle)
    }

    private external fun nativeCreateVideoDecoder(hardDecodeEnable: Boolean): Long

    private external fun nativeSetSource(nativeDecoderHandle: Long, url: String)

    private external fun nativeSetSurface(nativeDecoderHandle: Long, surface: Surface, width: Int, height: Int)

    private external fun nativeRelease(nativeDecoderHandle: Long)
}