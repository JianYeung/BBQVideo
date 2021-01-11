package com.yj.player.render

import android.view.Surface
import com.yj.player.filter.NativeFilterProxy
import com.yj.player.jni.GLRenderHelper

class NativeGLRenderProxy {
    private var nativeGLRenderHandle: Long = INIT_HANDLE

    fun setNativeGLRenderHandle(nativeGLRenderHandle: Long) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        this.nativeGLRenderHandle = nativeGLRenderHandle
    }

    fun getNativeGLRenderHandle(): Long {
        if (NativeFilterProxy.INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        return nativeGLRenderHandle
    }

    private fun destroyNativeGLRenderHandle() {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.destroyGLRenderHandle(nativeGLRenderHandle)
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

    fun setNativeRenderMode(renderMode: RenderMode) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        if (renderMode == RenderMode.UNKNOWN) {
            throw RuntimeException("Render mode is InValid")
        }
        GLRenderHelper.setRenderMode(nativeGLRenderHandle, renderMode)
    }

    fun setNativeFilter(nativeFilterHandle: Long) {
        if (INIT_HANDLE == nativeGLRenderHandle) {
            throw RuntimeException("Native GLRender handle is InValid")
        }
        GLRenderHelper.setFilter(nativeGLRenderHandle, nativeFilterHandle)
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
        nativeGLRenderHandle = 0
    }

    companion object {
        const val INIT_HANDLE: Long = 0
    }
}