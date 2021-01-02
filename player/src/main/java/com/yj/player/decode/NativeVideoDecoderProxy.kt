package com.yj.player.decode

import android.view.Surface
import com.yj.player.jni.DecodeHelper

class NativeVideoDecoderProxy {
    private var nativeDecoderHandle: Long = 0

    fun setNativeVideoDecoderHandle(nativeDecoderHandle: Long) {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        this.nativeDecoderHandle = nativeDecoderHandle
    }

    private fun destroyNativeVideoDecoderHandle() {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.releaseDecoderHandle(nativeDecoderHandle)
    }

    fun setSurface(surface: Surface, width: Int, height: Int) {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.setSurface(nativeDecoderHandle, surface, width, height)
    }

    fun setNativeRender(nativeGLRenderHandle: Long) {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.setRender(nativeDecoderHandle, nativeGLRenderHandle)
    }

    fun setPlayStatusCallback(nativePlayStatusCallback: NativePlayStatusCallback) {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.setPlayStatusCallback(nativeDecoderHandle, nativePlayStatusCallback)
    }

    fun setDataSource(url: String) {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.setDataSource(nativeDecoderHandle, url)
    }

    fun prepare() {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.prepare(nativeDecoderHandle)
    }

    fun start() {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.startDecode(nativeDecoderHandle)
    }

    fun resume() {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.resumeDecode(nativeDecoderHandle)
    }

    fun pause() {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.pauseDecode(nativeDecoderHandle)
    }

    fun seek(position: Int) {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.seekDecode(nativeDecoderHandle, position)
    }

    fun stop() {
        if (INIT_HANDLE == nativeDecoderHandle) {
            throw RuntimeException("Native Decoder handle is InValid")
        }
        DecodeHelper.stopDecode(nativeDecoderHandle)
    }

    fun release() {
        destroyNativeVideoDecoderHandle()
        nativeDecoderHandle = 0
    }

    companion object {
        const val INIT_HANDLE: Long = 0
    }
}