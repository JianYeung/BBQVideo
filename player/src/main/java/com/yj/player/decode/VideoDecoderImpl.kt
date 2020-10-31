package com.yj.player.decode

import android.view.Surface
import com.yj.player.jni.DecodeHelper

class VideoDecoderImpl {
    private var nativeDecoderHandle: Long = 0

    fun setNativeVideoDecoderHandle(nativeDecoderHandle: Long) {
        this.nativeDecoderHandle = nativeDecoderHandle
    }

    fun setSource(url: String) {
        DecodeHelper.setSource(nativeDecoderHandle, url)
    }

    fun setSurface(surface: Surface, width: Int, height: Int) {
        DecodeHelper.setSurface(nativeDecoderHandle, surface, width, height)
    }

    fun release() {
        DecodeHelper.release(nativeDecoderHandle)
    }
}