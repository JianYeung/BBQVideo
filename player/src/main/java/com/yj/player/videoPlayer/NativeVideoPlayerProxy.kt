package com.yj.player.videoPlayer

import android.view.Surface
import com.yj.player.jni.VideoPlayerHelper

class NativeVideoPlayerProxy {
    private var nativeVideoPlayerHandle: Long = 0

    fun setNativeVideoPlayerHandle(nativeVideoPlayerHandle: Long) {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        this.nativeVideoPlayerHandle = nativeVideoPlayerHandle
    }

    private fun destroyNativeVideoPlayerHandle() {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.releaseVideoPlayerHandle(nativeVideoPlayerHandle)
    }

    fun setSurface(surface: Surface, width: Int, height: Int) {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.setSurface(nativeVideoPlayerHandle, surface, width, height)
    }

    fun setNativeRender(nativeGLRenderHandle: Long) {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.setRender(nativeVideoPlayerHandle, nativeGLRenderHandle)
    }

    fun setPlayerStatusCallback(nativeVideoPlayerStatusCallback: NativeVideoPlayerStatusCallback) {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.setPlayerStatusCallback(nativeVideoPlayerHandle, nativeVideoPlayerStatusCallback)
    }

    fun setDataSource(url: String) {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.setDataSource(nativeVideoPlayerHandle, url)
    }

    fun prepare() {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.prepare(nativeVideoPlayerHandle)
    }

    fun start() {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.start(nativeVideoPlayerHandle)
    }

    fun resume() {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.resume(nativeVideoPlayerHandle)
    }

    fun pause() {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.pause(nativeVideoPlayerHandle)
    }

    fun seek(position: Int) {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.seek(nativeVideoPlayerHandle, position)
    }

    fun stop() {
        if (INIT_HANDLE == nativeVideoPlayerHandle) {
            throw RuntimeException("Native Video Player handle is InValid")
        }
        VideoPlayerHelper.stop(nativeVideoPlayerHandle)
    }

    fun release() {
        destroyNativeVideoPlayerHandle()
        nativeVideoPlayerHandle = 0
    }

    companion object {
        const val INIT_HANDLE: Long = 0
    }
}