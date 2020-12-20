package com.yj.player.view

import android.content.Context
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView
import androidx.annotation.NonNull
import com.yj.player.PlayerManager
import com.yj.player.decode.NativeVideoDecoderProxy
import com.yj.player.render.NativeFilterProxy
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.render.RenderMode

class NativeVideoPreviewer : SurfaceView, SurfaceHolder.Callback {
    private var nativeFilterProxy: NativeFilterProxy? = null
    private val nativeGLRenderProxy: NativeGLRenderProxy by lazy {
        PlayerManager.createNativeGLRenderProxy()
    }

    private val nativeVideoDecoderProxy: NativeVideoDecoderProxy by lazy {
        PlayerManager.createNativeVideoDecoderProxy()
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
        super.onDetachedFromWindow()
        nativeGLRenderProxy.onDetachedFromWindow()
    }

    fun onPause() {
        nativeVideoDecoderProxy.pause()
        nativeGLRenderProxy.onPause()
    }

    fun onResume() {
        nativeGLRenderProxy.onResume()
        nativeVideoDecoderProxy.resume()
    }

    fun onDestroy() {
        nativeVideoDecoderProxy.release()
        nativeFilterProxy?.onDestroy()
        nativeGLRenderProxy.onDestroy()
    }

    /**
     * Set Data source.
     * @param url the url of video.
     */
    fun setDataSource(url: String) {
        nativeVideoDecoderProxy.setDataSource(url)
    }

    /**
     * Start play video.
     */
    fun playVideo() {
        nativeVideoDecoderProxy.start()
    }

    /**
     * Pause video.
     */
    fun pauseVideo() {
        nativeVideoDecoderProxy.pause()
    }

    /**
     * Resume video.
     */
    fun resumeVideo() {
        nativeVideoDecoderProxy.resume()
    }

    /**
     * Seek Video.
     */
    fun seekVideo(position: Int) {
        nativeVideoDecoderProxy.seek(position)
    }

    /**
     * Stop video.
     */
    fun stopVideo() {
        nativeVideoDecoderProxy.stop()
    }

    /**
     * Set Render Mode.
     * @param renderMode one of the RENDERMODE_X constants
     * @see RenderMode#RENDERMODE_CONTINUOUSLY
     * @see RenderMode#RENDERMODE_WHEN_DIRTY
     */
    fun setRenderMode(@NonNull renderMode: RenderMode) {
        nativeGLRenderProxy.setNativeRenderMode(renderMode)
    }

    /**
     * Set Filter.
     * @param filter one of the NativeFilterProxy object
     */
    fun setFilter(@NonNull filter: NativeFilterProxy) {
        nativeFilterProxy = filter
        nativeGLRenderProxy.setNativeFilter(filter.getNativeFilterHandle())
        nativeVideoDecoderProxy.setNativeRender(nativeGLRenderProxy.getNativeGLRenderHandle())
    }

    /**
     *  Request render.
     */
    fun requestRender() {
        nativeGLRenderProxy.requestRender()
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
        nativeVideoDecoderProxy.setSurface(holder.surface, width, height)
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