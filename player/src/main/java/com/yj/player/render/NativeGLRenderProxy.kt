package com.yj.player.render

import android.view.Surface
import com.yj.player.jni.GLRenderHelper

class NativeGLRenderProxy {
    private var nativeGLRenderHandle: Long = INIT_HANDLE

    fun setNativeGLRenderHandle(nativeGLRenderHandle: Long) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        this.nativeGLRenderHandle = nativeGLRenderHandle
    }

    private fun destroyNativeGLRenderHandle() {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.destroyGLRenderHandle(nativeGLRenderHandle)
    }

    fun init() {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.init(nativeGLRenderHandle)
    }

    fun unInit() {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.unInit(nativeGLRenderHandle)
    }

    fun onPause() {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.onPause(nativeGLRenderHandle)
    }

    fun onResume() {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.onResume(nativeGLRenderHandle)
    }

    fun setNativeRenderMode(rendMode: RendMode) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        if (rendMode == RendMode.UNKNOWN) {
            throw RuntimeException("Render mode is InValid")
        }
        GLRenderHelper.setRenderMode(nativeGLRenderHandle, rendMode)
    }

    fun setNativeFilter(filterType: FilterType) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        if (filterType == FilterType.UNKNOWN) {
            throw RuntimeException("Filter type is InValid")
        }
        GLRenderHelper.setFilter(nativeGLRenderHandle, filterType)
    }

    fun requestRender() {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.requestRender(nativeGLRenderHandle)
    }

    fun onSurfaceCreated(surface: Surface) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.onSurfaceCreated(nativeGLRenderHandle, surface)
    }

    fun onSurfaceChanged(surface: Surface, format: Int, width: Int, height: Int) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.onSurfaceChanged(nativeGLRenderHandle, surface, format, width, height)
    }

    fun onSurfaceDestroyed(surface: Surface) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.onSurfaceDestroyed(nativeGLRenderHandle, surface)
    }

    fun onDestroy() {
        destroyNativeGLRenderHandle()
    }

    companion object {
        const val INIT_HANDLE :Long = 0
    }
}