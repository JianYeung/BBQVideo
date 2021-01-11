package com.yj.player.jni

import com.yj.player.filter.FilterType
import com.yj.player.filter.NativeFilterProxy

object FilterHelper {

    fun createFilterHandle(filterType: FilterType): NativeFilterProxy {
        val nativeFilterProxy = NativeFilterProxy()
        val nativeFilterHandle = nativeCreateFilterHandle(filterType.value)
        nativeFilterProxy.setNativeFilterHandle(nativeFilterHandle)
        return nativeFilterProxy
    }

    fun destroyFilterHandle(filterHandle: Long) {
        nativeDestroyFilterHandle(filterHandle)
    }

    fun updatePreviewFrame(glRenderHandle: Long, data: ByteArray?, format: Int, width: Int, height: Int) {
        nativeUpdatePreviewFrame(glRenderHandle, data, format, width, height)
    }

    private external fun nativeCreateFilterHandle(filterType: Int):Long

    private external fun nativeDestroyFilterHandle(glRenderHandle: Long)

    private external fun nativeUpdatePreviewFrame(glRenderHandle: Long, data: ByteArray?, format: Int, width: Int, height: Int)
}