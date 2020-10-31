package com.yj.player.render

import android.content.Context
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.yj.player.PlayerManager

class NativeGLSurfaceView(context: Context, attrs: AttributeSet) : SurfaceView(context, attrs), SurfaceHolder.Callback {

    private var nativeGLRenderProxy: NativeGLRenderProxy = PlayerManager.createNativeGLRenderProxy()

    init {
        holder.addCallback(this)
        nativeGLRenderProxy.setNativeRenderMode(RendMode.RENDERMODE_CONTINUOUSLY)
        nativeGLRenderProxy.setNativeFilter(FilterType.TRIANGLE)
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
    }

    fun onPause() {
        nativeGLRenderProxy.onPause()
    }

    fun onResume() {
        nativeGLRenderProxy.onResume()
    }

    fun onDestroy() {
        nativeGLRenderProxy.onDestroy()
    }

    /**
     * This is called immediately after the surface is first created.
     * Implementations of this should start up whatever rendering code
     * they desire.  Note that only one thread can ever draw into
     * a [Surface], so you should not draw into the Surface here
     * if your normal rendering will be in another thread.
     *
     * @param holder The SurfaceHolder whose surface is being created.
     */
    override fun surfaceCreated(holder: SurfaceHolder) {
        nativeGLRenderProxy.onSurfaceCreated(holder.surface)
    }

    /**
     * This is called immediately after any structural changes (format or
     * size) have been made to the surface.  You should at this point update
     * the imagery in the surface.  This method is always called at least
     * once, after [.surfaceCreated].
     *
     * @param holder The SurfaceHolder whose surface has changed.
     * @param format The new [PixelFormat] of the surface.
     * @param width The new width of the surface.
     * @param height The new height of the surface.
     */
    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        nativeGLRenderProxy.onSurfaceChanged(holder.surface, format, width, height)
    }

    /**
     * This is called immediately before a surface is being destroyed. After
     * returning from this call, you should no longer try to access this
     * surface.  If you have a rendering thread that directly accesses
     * the surface, you must ensure that thread is no longer touching the
     * Surface before returning from this function.
     *
     * @param holder The SurfaceHolder whose surface is being destroyed.
     */
    override fun surfaceDestroyed(holder: SurfaceHolder) {
        nativeGLRenderProxy.onSurfaceDestroyed(holder.surface)
    }
}