package com.yj.player.view

import android.content.Context
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView
import androidx.annotation.NonNull
import androidx.annotation.Nullable
import com.yj.player.PlayerManager
import com.yj.player.videoPlayer.NativeVideoPlayerStatusCallback
import com.yj.player.videoPlayer.NativeVideoPlayerProxy
import com.yj.player.videoPlayer.VideoPlayerError
import com.yj.player.videoPlayer.VideoPlayerStatusListener
import com.yj.player.filter.FilterType
import com.yj.player.filter.NativeFilterProxy
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.render.RenderMode
import com.yj.player.util.PlayerLog
import com.yj.player.util.PlayerThreadPool

class CustomVideoPreviewer : SurfaceView, SurfaceHolder.Callback, NativeVideoPlayerStatusCallback {
    private var videoPlayerStatusListener: VideoPlayerStatusListener? = null
    private var nativeFilterProxy: NativeFilterProxy? = null
    private var nativeGLRenderProxy: NativeGLRenderProxy? = null
    private var nativeVideoPlayerProxy: NativeVideoPlayerProxy? = null

    constructor(context: Context) : super(context) {
        holder.addCallback(this)
        initNativePlayerProxy()
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        holder.addCallback(this)
        initNativePlayerProxy()
    }

    constructor(context: Context?, attrs: AttributeSet?, defStyle: Int) : super(
        context,
        attrs,
        defStyle
    ) {
        holder.addCallback(this)
        initNativePlayerProxy()
    }

    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int, defStyleRes: Int) : super(
        context,
        attrs,
        defStyleAttr,
        defStyleRes
    ) {
        holder.addCallback(this)
        initNativePlayerProxy()
    }

    private fun initNativePlayerProxy() {
        nativeGLRenderProxy = PlayerManager.createNativeGLRenderProxy()
        nativeVideoPlayerProxy = PlayerManager.createNativePlayerProxy()
    }

    fun onPause() {
        nativeVideoPlayerProxy?.pause()
        nativeGLRenderProxy?.onPause()
    }

    fun onResume() {
        nativeGLRenderProxy?.onResume()
        nativeVideoPlayerProxy?.resume()
    }

    fun onDestroy() {
        videoPlayerStatusListener = null
        if (nativeVideoPlayerProxy != null) {
            nativeVideoPlayerProxy?.release()
            nativeVideoPlayerProxy = null
        }
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.onDestroy()
            nativeGLRenderProxy = null
        }
        if (nativeFilterProxy != null) {
            nativeFilterProxy?.onDestroy()
            nativeFilterProxy = null
        }
    }

    /**
     * Set Data source url.
     * @param url the url of video.
     */
    fun setDataSource(url: String) {
        nativeVideoPlayerProxy?.setDataSource(url)
    }

    /**
     * Prepare data source
     */
    fun prepare() {
        PlayerThreadPool.getInstance().submit {
            nativeVideoPlayerProxy?.prepare()
        }
    }

    /**
     * Start play video.
     */
    fun playVideo() {
        nativeVideoPlayerProxy?.start()
    }

    /**
     * Pause video.
     */
    fun pauseVideo() {
        nativeVideoPlayerProxy?.pause()
    }

    /**
     * Resume video.
     */
    fun resumeVideo() {
        nativeVideoPlayerProxy?.resume()
    }

    /**
     * Seek Video.
     */
    fun seekVideo(position: Int) {
        nativeVideoPlayerProxy?.seek(position)
    }

    /**
     * Stop video.
     */
    fun stopVideo() {
        nativeVideoPlayerProxy?.stop()
    }

    /**
     * Set Render Mode.
     * @param renderMode one of the RENDERMODE_X constants
     * @see RenderMode#RENDERMODE_CONTINUOUSLY
     * @see RenderMode#RENDERMODE_WHEN_DIRTY
     */
    fun setRenderMode(@NonNull renderMode: RenderMode) {
        nativeGLRenderProxy?.setNativeRenderMode(renderMode)
    }

    /**
     * Set Filter type.
     * @param filterType one of the FilterType object
     */
    fun setFilterType(@NonNull filterType: FilterType) {
        if (filterType == FilterType.UNKNOWN) {
            PlayerLog.d(TAG, "Filter type is NULL")
            return
        }
        nativeFilterProxy = PlayerManager.createNativeFilterProxy(filterType)
        if (nativeFilterProxy != null) {
            nativeGLRenderProxy?.setNativeFilter(nativeFilterProxy!!.getNativeFilterHandle())
            val nativeGLRenderHandle = nativeGLRenderProxy?.getNativeGLRenderHandle()
            if (nativeGLRenderHandle != null) {
                nativeVideoPlayerProxy?.setNativeRender(nativeGLRenderHandle)
            }
        }
    }

    /**
     * Set Filter.
     * @param filter one of the NativeFilterProxy object
     */
    fun setFilter(@NonNull filter: NativeFilterProxy) {
        nativeFilterProxy = filter
        nativeGLRenderProxy?.setNativeFilter(filter.getNativeFilterHandle())
        val nativeGLRenderHandle = nativeGLRenderProxy?.getNativeGLRenderHandle()
        if (nativeGLRenderHandle != null) {
            nativeVideoPlayerProxy?.setNativeRender(nativeGLRenderHandle)
        }
    }

    /**
     *  Request render.
     */
    fun requestRender() {
        nativeGLRenderProxy?.requestRender()
    }

    /**
     * Set player status listener.
     * @param videoPlayerStatusListener player status listener
     */
    fun setPlayerStatusListener(@Nullable videoPlayerStatusListener: VideoPlayerStatusListener) {
        this.videoPlayerStatusListener = videoPlayerStatusListener
        nativeVideoPlayerProxy?.setPlayerStatusCallback(this)
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
        nativeGLRenderProxy?.onSurfaceCreated(holder.surface)
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
        nativeGLRenderProxy?.onSurfaceChanged(holder.surface, format, width, height)
        nativeVideoPlayerProxy?.setSurface(holder.surface, width, height)
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
        nativeGLRenderProxy?.onSurfaceDestroyed(holder.surface)
    }

    /**
     *
     */
    override fun onPrepared() {
        PlayerThreadPool.getInstance().submit {
            videoPlayerStatusListener?.onPrepared()
        }
    }

    /**
     * @param presentationTime
     */
    override fun onProgress(presentationTime: Long) {
        PlayerThreadPool.getInstance().submit {
            videoPlayerStatusListener?.onProgress(presentationTime)
        }
    }

    /**
     * @param errorCode
     */
    override fun onError(errorCode: Int) {
        PlayerThreadPool.getInstance().submit {
            videoPlayerStatusListener?.onError(VideoPlayerError.getError(errorCode))
        }
    }

    companion object {
        val TAG: String = CustomVideoPreviewer::class.java.simpleName
    }
}