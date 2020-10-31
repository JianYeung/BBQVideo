package com.yj.player

import com.yj.player.decode.VideoDecoderImpl
import com.yj.player.encode.VideoEncoderImpl
import com.yj.player.jni.DecodeHelper
import com.yj.player.jni.EncodeHelper
import com.yj.player.jni.GLRenderHelper
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.rtmp.RtmpManagerImpl
import java.lang.System.loadLibrary

object PlayerManager {

    private var hardDecodeEnable = true

    init {
        loadLibrary("player-lib")
    }

    fun createNativeGLRenderProxy(): NativeGLRenderProxy {
        return GLRenderHelper.createGLRenderHandle()
    }

    fun createVideoDecoder(): VideoDecoderImpl {
        return DecodeHelper.createVideoDecoder(hardDecodeEnable)
    }

    fun createVideoEncoder(): VideoEncoderImpl {
        return EncodeHelper.createVideoEncoder(hardDecodeEnable)
    }

    fun createRtmpManager(): RtmpManagerImpl {
        return RtmpManagerImpl()
    }
}