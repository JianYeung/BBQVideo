package com.yj.player.view

import android.content.Context
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.yj.player.PlayerManager
import com.yj.player.camera.Rotation
import com.yj.player.render.NativeGLRenderProxy

class NativeGLSurfaceView : SurfaceView, SurfaceHolder.Callback {

    private val nativeGLRenderProxy: NativeGLRenderProxy by lazy {
        PlayerManager.createNativeGLRenderProxy()
    }

    constructor(context: Context) : super(context) {
        holder.addCallback(this)
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        holder.addCallback(this)
    }

    constructor(context: Context?, attrs: AttributeSet?, defStyle: Int) : super(
        context,
        attrs,
        defStyle
    ) {
        holder.addCallback(this)
    }

    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int, defStyleRes: Int) : super(
        context,
        attrs,
        defStyleAttr,
        defStyleRes
    ) {
        holder.addCallback(this)
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()
        nativeGLRenderProxy.onAttachedToWindow()
    }

    override fun onDetachedFromWindow() {
        nativeGLRenderProxy.onDetachedFromWindow()
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
     * Sets the rotation of the displayed image.
     *
     * @param rotation new rotation
     */
    fun setRotation(rotation: Rotation) {
        nativeGLRenderProxy.setNativeRotation(rotation)
    }

    /**
     * Set Render Mode.
     * @param renderMode one of the RENDERMODE_X constants
     * @see RenderMode#RENDERMODE_CONTINUOUSLY
     * @see RenderMode#RENDERMODE_WHEN_DIRTY
     */
    fun setRenderMode(renderMode: RenderMode) {
        nativeGLRenderProxy.setNativeRenderMode(renderMode)
    }

    /**
     * Set Filter Type.
     * @param filterType one of the filterType constants
     */
    fun setFilterType(filterType: FilterType) {
        nativeGLRenderProxy.setNativeFilter(filterType)
    }

    /**
     *  Request render.
     */
    fun requestRender() {
        nativeGLRenderProxy.requestRender();
    }

    /**
     * Update camera preview frame with YUV format data.
     *
     * @param data   Camera preview YUV data for frame.
     * @param width  width of camera preview
     * @param height height of camera preview
     */
    fun updatePreviewFrame(data: ByteArray?, format: Int, width: Int, height: Int) {
        nativeGLRenderProxy.updatePreviewFrame(data, format, width, height)
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