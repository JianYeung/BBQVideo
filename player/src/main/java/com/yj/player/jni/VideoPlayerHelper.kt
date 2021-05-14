package com.yj.player.jni

import android.view.Surface
import com.yj.player.videoPlayer.NativeVideoPlayerProxy
import com.yj.player.videoPlayer.NativeVideoPlayerStatusCallback

object VideoPlayerHelper {

    fun createVideoPlayerHandle(softwareDecodeEnable: Boolean): NativeVideoPlayerProxy {
        val playerImpl = NativeVideoPlayerProxy()
        val nativePlayerHandle = nativeCreateVideoPlayerHandle(softwareDecodeEnable)
        playerImpl.setNativeVideoPlayerHandle(nativePlayerHandle)
        return playerImpl
    }

    fun releaseVideoPlayerHandle(nativePlayerHandle: Long) {
        nativeReleaseVideoPlayerHandle(nativePlayerHandle)
    }

    fun setSurface(nativePlayerHandle: Long, surface: Surface, width: Int, height: Int) {
        nativeSetSurface(nativePlayerHandle, surface, width, height)
    }

    fun setRender(nativePlayerHandle: Long, nativeGLRenderHandle: Long) {
        nativeSetRender(nativePlayerHandle, nativeGLRenderHandle)
    }

    fun setDataSource(nativePlayerHandle: Long, url: String) {
        nativeSetDataSource(nativePlayerHandle, url)
    }

    fun setCpuIds(nativePlayerHandle: Long, cpuIds: IntArray) {
        nativeSetCpuIds(nativePlayerHandle, cpuIds)
    }

    fun setPlayerStatusCallback(nativePlayerHandle: Long, nativeVideoPlayerStatusCallback: NativeVideoPlayerStatusCallback) {
        nativeSetPlayerStatusCallback(nativePlayerHandle, nativeVideoPlayerStatusCallback)
    }

    fun prepare(nativePlayerHandle: Long) {
        nativePrepare(nativePlayerHandle)
    }

    fun start(nativePlayerHandle: Long) {
        nativeStart(nativePlayerHandle)
    }

    fun stop(nativePlayerHandle: Long) {
        nativeStop(nativePlayerHandle)
    }

    fun resume(nativePlayerHandle: Long) {
        nativeResume(nativePlayerHandle)
    }

    fun pause(nativePlayerHandle: Long) {
        nativePause(nativePlayerHandle)
    }

    fun seek(nativePlayerHandle: Long, position: Int) {
        nativeSeek(nativePlayerHandle, position)
    }

    private external fun nativeCreateVideoPlayerHandle(softwareDecodeEnable: Boolean): Long

    private external fun nativeReleaseVideoPlayerHandle(nativePlayerHandle: Long)

    private external fun nativeSetSurface(nativePlayerHandle: Long, surface: Surface, width: Int, height: Int)

    private external fun nativeSetRender(nativePlayerHandle: Long, nativeGLRenderHandle: Long)

    private external fun nativeSetPlayerStatusCallback(nativePlayerHandle: Long, nativeVideoPlayerStatusCallback: NativeVideoPlayerStatusCallback)

    private external fun nativeSetDataSource(nativePlayerHandle: Long, url: String)

    private external fun nativeSetCpuIds(nativePlayerHandle: Long, cpuIds: IntArray)

    private external fun nativePrepare(nativePlayerHandle: Long)

    private external fun nativeStart(nativePlayerHandle: Long)

    private external fun nativeStop(nativePlayerHandle: Long)

    private external fun nativeResume(nativePlayerHandle: Long)

    private external fun nativePause(nativePlayerHandle: Long)

    private external fun nativeSeek(nativePlayerHandle: Long, position: Int)
}