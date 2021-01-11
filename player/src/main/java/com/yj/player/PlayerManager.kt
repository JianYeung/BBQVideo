package com.yj.player

import com.yj.player.jni.FilterHelper
import com.yj.player.jni.GLRenderHelper
import com.yj.player.jni.VideoPlayerHelper
import com.yj.player.jni.EncodeHelper
import com.yj.player.filter.FilterType
import com.yj.player.filter.NativeFilterProxy
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.videoPlayer.NativeVideoPlayerProxy
import com.yj.player.encode.NativeVideoEncoderProxy
import com.yj.player.rtmp.RtmpManagerImpl
import java.lang.System.loadLibrary

object PlayerManager {

    private var softwareDecodeEnable = false
    private var softwareEncodeEnable = true

    init {
        loadLibrary("player-lib")
    }

    fun createNativeFilterProxy(filterType: FilterType): NativeFilterProxy {
        return FilterHelper.createFilterHandle(filterType)
    }

    fun createNativeGLRenderProxy(): NativeGLRenderProxy {
        return GLRenderHelper.createGLRenderHandle()
    }

    fun createNativePlayerProxy(): NativeVideoPlayerProxy {
        return VideoPlayerHelper.createVideoPlayerHandle(softwareDecodeEnable)
    }

    fun createNativeVideoEncoderProxy(): NativeVideoEncoderProxy {
        return EncodeHelper.createVideoEncoderHandle(softwareEncodeEnable)
    }

    fun createRtmpManager(): RtmpManagerImpl {
        return RtmpManagerImpl()
    }
}