package com.yj.player.jni

import android.view.Surface
import com.yj.player.camera.Rotation
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.view.FilterType
import com.yj.player.view.RenderMode

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

    fun init(glRenderHandle: Long) {
        nativeInit(glRenderHandle)
    }

    fun unInit(glRenderHandle: Long) {
        nativeUnInit(glRenderHandle)
    }

    fun onPause(glRenderHandle: Long) {
        nativePause(glRenderHandle)
    }

    fun onResume(glRenderHandle: Long) {
        nativeResume(glRenderHandle)
    }

    fun onAttachedToWindow(glRenderHandle: Long) {
        nativeAttachedToWindow(glRenderHandle)
    }

    fun onDetachedFromWindow(glRenderHandle: Long) {
        nativeDetachedFromWindow(glRenderHandle)
    }

    fun setRenderMode(glRenderHandle: Long, renderMode: RenderMode) {
        nativeSetRenderMode(glRenderHandle, renderMode.value)
    }

    fun setFilter(glRenderHandle: Long, filterType: FilterType) {
        nativeSetFilter(glRenderHandle, filterType.value)
    }

    fun setRotation(glRenderHandle: Long, rotation: Rotation) {
        nativeSetRotation(glRenderHandle, rotation.value)
    }

    fun updatePreviewFrame(glRenderHandle: Long, data: ByteArray?, format: Int, width: Int, height: Int) {
        nativeUpdatePreviewFrame(glRenderHandle, data, format, width, height)
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

    private external fun nativeInit(glRenderHandle: Long)

    private external fun nativeUnInit(glRenderHandle: Long)

    private external fun nativePause(glRenderHandle: Long)

    private external fun nativeResume(glRenderHandle: Long)

    private external fun nativeAttachedToWindow(glRenderHandle: Long)

    private external fun nativeDetachedFromWindow(glRenderHandle: Long)

    private external fun nativeSetRotation(glRenderHandle: Long, rotation: Int)

    private external fun nativeSetRenderMode(glRenderHandle: Long, renderMode: Int)

    private external fun nativeSetFilter(glRenderHandle: Long, filterType: Int)

    private external fun nativeUpdatePreviewFrame(glRenderHandle: Long, data: ByteArray?, format: Int, width: Int, height: Int)

    private external fun nativeRequestRender(glRenderHandle: Long)

    private external fun nativeSurfaceCreated(glRenderHandle: Long, surface: Surface)

    private external fun nativeSurfaceChanged(glRenderHandle: Long,surface: Surface, format: Int, width: Int, height: Int)

    private external fun nativeSurfaceDestroyed(glRenderHandle: Long, surface: Surface)
}