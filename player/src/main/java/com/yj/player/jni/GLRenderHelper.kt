package com.yj.player.jni

import android.view.Surface
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.render.RenderMode

object GLRenderHelper {

    fun createGLRenderHandle(): NativeGLRenderProxy {
        val nativeGLRenderProxy = NativeGLRenderProxy()
        val nativeGLRenderHandle = nativeCreateGLRenderHandle()
        nativeGLRenderProxy.setNativeGLRenderHandle(nativeGLRenderHandle)
        return nativeGLRenderProxy
    }

    fun destroyGLRenderHandle(glRenderHandle: Long) {
        nativeDestroyGLRenderHandle(glRenderHandle)
    }

    fun onPause(glRenderHandle: Long) {
        nativePause(glRenderHandle)
    }

    fun onResume(glRenderHandle: Long) {
        nativeResume(glRenderHandle)
    }

    fun setRenderMode(glRenderHandle: Long, renderMode: RenderMode) {
        nativeSetRenderMode(glRenderHandle, renderMode.value)
    }

    fun setFilter(glRenderHandle: Long, filterHandle: Long) {
        nativeSetFilter(glRenderHandle, filterHandle)
    }

    fun requestRender(glRenderHandle: Long) {
        nativeRequestRender(glRenderHandle)
    }

    fun onSurfaceCreated(glRenderHandle: Long, surface: Surface) {
        nativeSurfaceCreated(glRenderHandle, surface)
    }

    fun onSurfaceChanged(glRenderHandle: Long, surface: Surface, format: Int, width: Int, height: Int) {
        nativeSurfaceChanged(glRenderHandle, surface, format, width, height)
    }

    fun onSurfaceDestroyed(glRenderHandle: Long, surface: Surface) {
        nativeSurfaceDestroyed(glRenderHandle, surface)
    }

    private external fun nativeCreateGLRenderHandle():Long

    private external fun nativeDestroyGLRenderHandle(glRenderHandle: Long)

    private external fun nativePause(glRenderHandle: Long)

    private external fun nativeResume(glRenderHandle: Long)

    private external fun nativeSetRenderMode(glRenderHandle: Long, renderMode: Int)

    private external fun nativeSetFilter(glRenderHandle: Long, filterHandle: Long)

    private external fun nativeRequestRender(glRenderHandle: Long)

    private external fun nativeSurfaceCreated(glRenderHandle: Long, surface: Surface)

    private external fun nativeSurfaceChanged(glRenderHandle: Long,surface: Surface, format: Int, width: Int, height: Int)

    private external fun nativeSurfaceDestroyed(glRenderHandle: Long, surface: Surface)
}