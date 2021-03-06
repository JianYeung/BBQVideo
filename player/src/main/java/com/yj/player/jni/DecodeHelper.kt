package com.yj.player.jni

import android.view.Surface
import com.yj.player.decode.NativeVideoDecoderProxy
import com.yj.player.decode.NativePlayStatusCallback

object DecodeHelper {

    fun createVideoDecoderHandle(softwareDecodeEnable: Boolean): NativeVideoDecoderProxy {
        val videoDecoderImpl = NativeVideoDecoderProxy()
        val nativeVideoDecodeHandle = nativeCreateVideoDecoderHandle(softwareDecodeEnable)
        videoDecoderImpl.setNativeVideoDecoderHandle(nativeVideoDecodeHandle)
        return videoDecoderImpl
    }

    fun releaseDecoderHandle(nativeDecoderHandle: Long) {
        nativeReleaseVideoDecoderHandle(nativeDecoderHandle)
    }

    fun setSurface(nativeDecoderHandle: Long, surface: Surface, width: Int, height: Int) {
        nativeSetSurface(nativeDecoderHandle, surface, width, height)
    }

    fun setRender(nativeDecoderHandle: Long, nativeGLRenderHandle: Long) {
        nativeSetRender(nativeDecoderHandle, nativeGLRenderHandle)
    }

    fun setDataSource(nativeDecoderHandle: Long, url: String) {
        nativeSetDataSource(nativeDecoderHandle, url)
    }

    fun prepare(nativeDecoderHandle: Long) {
        nativePrepare(nativeDecoderHandle)
    }

    fun setPlayStatusCallback(nativeDecoderHandle: Long, nativePlayStatusCallback: NativePlayStatusCallback) {
        nativeSetPlayStatusCallback(nativeDecoderHandle, nativePlayStatusCallback)
    }

    fun startDecode(nativeDecoderHandle: Long) {
        nativeStartDecode(nativeDecoderHandle)
    }

    fun stopDecode(nativeDecoderHandle: Long) {
        nativeStopDecode(nativeDecoderHandle)
    }

    fun resumeDecode(nativeDecoderHandle: Long) {
        nativeResumeDecode(nativeDecoderHandle)
    }

    fun pauseDecode(nativeDecoderHandle: Long) {
        nativePauseDecode(nativeDecoderHandle)
    }

    fun seekDecode(nativeDecoderHandle: Long, position: Int) {
        nativeSeekDecode(nativeDecoderHandle, position)
    }

    private external fun nativeCreateVideoDecoderHandle(softwareDecodeEnable: Boolean): Long

    private external fun nativeReleaseVideoDecoderHandle(nativeDecoderHandle: Long)

    private external fun nativeSetSurface(nativeDecoderHandle: Long, surface: Surface, width: Int, height: Int)

    private external fun nativeSetRender(nativeDecoderHandle: Long, nativeGLRenderHandle: Long)

    private external fun nativeSetPlayStatusCallback(nativeDecoderHandle: Long, nativePlayStatusCallback: NativePlayStatusCallback)

    private external fun nativeSetDataSource(nativeDecoderHandle: Long, url: String)

    private external fun nativePrepare(nativeDecoderHandle: Long)

    private external fun nativeStartDecode(nativeDecoderHandle: Long)

    private external fun nativeStopDecode(nativeDecoderHandle: Long)

    private external fun nativeResumeDecode(nativeDecoderHandle: Long)

    private external fun nativePauseDecode(nativeDecoderHandle: Long)

    private external fun nativeSeekDecode(nativeDecoderHandle: Long, position: Int)
}