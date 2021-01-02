package com.yj.player.view

import android.content.Context
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView
import androidx.annotation.NonNull
import androidx.annotation.Nullable
import com.yj.player.PlayerManager
import com.yj.player.decode.NativePlayStatusCallback
import com.yj.player.decode.NativeVideoDecoderProxy
import com.yj.player.listener.PlayError
import com.yj.player.listener.PlayStatusListener
import com.yj.player.render.NativeFilterProxy
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.render.RenderMode
import com.yj.player.util.PlayerThreadPool

class NativeVideoPreviewer : SurfaceView, SurfaceHolder.Callback, NativePlayStatusCallback {
    private var playStatusListener: PlayStatusListener? = null
    private var nativeFilterProxy: NativeFilterProxy? = null
    private var nativeGLRenderProxy: NativeGLRenderProxy? = null
    private var nativeVideoDecoderProxy: NativeVideoDecoderProxy? = null

    constructor(context: Context) : super(context) {
        holder.addCallback(this)
        initNativePreviewerProxy()
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        holder.addCallback(this)
        initNativePreviewerProxy()
    }

    constructor(context: Context?, attrs: AttributeSet?, defStyle: Int) : super(
        context,
        attrs,
        defStyle
    ) {
        holder.addCallback(this)
        initNativePreviewerProxy()
    }

    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int, defStyleRes: Int) : super(
        context,
        attrs,
        defStyleAttr,
        defStyleRes
    ) {
        holder.addCallback(this)
        initNativePreviewerProxy()
    }

    private fun initNativePreviewerProxy() {
        nativeGLRenderProxy = PlayerManager.createNativeGLRenderProxy()
        nativeVideoDecoderProxy = PlayerManager.createNativeVideoDecoderProxy()
    }

    fun onPause() {
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.pause()
        }
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.onPause()
        }
    }

    fun onResume() {
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.onResume()
        }
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.resume()
        }
    }

    fun onDestroy() {
        playStatusListener = null
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.release()
            nativeVideoDecoderProxy = null
        }
        if (nativeFilterProxy != null) {
            nativeFilterProxy?.onDestroy()
            nativeFilterProxy = null
        }
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.onDestroy()
            nativeGLRenderProxy = null
        }
    }

    /**
     * Set Data source url.
     * @param url the url of video.
     */
    fun setDataSource(url: String) {
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.setDataSource(url)
        }
    }

    /**
     * Prepare data source
     */
    fun prepare() {
        if (nativeVideoDecoderProxy != null) {
            PlayerThreadPool.getInstance().submit {
                nativeVideoDecoderProxy?.prepare()
            }
        }
    }

    /**
     * Start play video.
     */
    fun playVideo() {
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.start()
        }
    }

    /**
     * Pause video.
     */
    fun pauseVideo() {
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.pause()
        }
    }

    /**
     * Resume video.
     */
    fun resumeVideo() {
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.resume()
        }
    }

    /**
     * Seek Video.
     */
    fun seekVideo(position: Int) {
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.seek(position)
        }
    }

    /**
     * Stop video.
     */
    fun stopVideo() {
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.stop()
        }
    }

    /**
     * Set Render Mode.
     * @param renderMode one of the RENDERMODE_X constants
     * @see RenderMode#RENDERMODE_CONTINUOUSLY
     * @see RenderMode#RENDERMODE_WHEN_DIRTY
     */
    fun setRenderMode(@NonNull renderMode: RenderMode) {
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.setNativeRenderMode(renderMode)
        }
    }

    /**
     * Set Filter.
     * @param filter one of the NativeFilterProxy object
     */
    fun setFilter(@NonNull filter: NativeFilterProxy) {
        nativeFilterProxy = filter
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.setNativeFilter(filter.getNativeFilterHandle())
        }
        if (nativeVideoDecoderProxy != null) {
            val nativeGLRenderHandle = nativeGLRenderProxy?.getNativeGLRenderHandle()
            if (nativeGLRenderHandle != null) {
                nativeVideoDecoderProxy?.setNativeRender(nativeGLRenderHandle)
            }
        }
    }

    /**
     *  Request render.
     */
    fun requestRender() {
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.requestRender()
        }
    }

    /**
     * Set player status listener.
     * @param playStatusListener player status listener
     */
    fun setPlayStatusListener(@Nullable playStatusListener: PlayStatusListener) {
        this.playStatusListener = playStatusListener
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.setPlayStatusCallback(this)
        }
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
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.onSurfaceCreated(holder.surface)
        }
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
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.onSurfaceChanged(holder.surface, format, width, height)
        }
        if (nativeVideoDecoderProxy != null) {
            nativeVideoDecoderProxy?.setSurface(holder.surface, width, height)
        }
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
        if (nativeGLRenderProxy != null) {
            nativeGLRenderProxy?.onSurfaceDestroyed(holder.surface)
        }
    }

    /**
     * @param duration
     */
    override fun onPrepared(duration: Long) {
        if (playStatusListener != null) {
            PlayerThreadPool.getInstance().submit {
                if (playStatusListener != null) {
                    playStatusListener?.onPrepared(duration)
                }
            }
        }
    }

    /**
     * @param presentationTime
     */
    override fun onProgress(presentationTime: Long) {
        if (playStatusListener != null) {
            PlayerThreadPool.getInstance().submit {
                if (playStatusListener != null) {
                    playStatusListener?.onProgress(presentationTime)
                }
            }
        }
    }

    /**
     * @param errorCode
     */
    override fun onError(errorCode: Int) {
        if (playStatusListener != null) {
            PlayerThreadPool.getInstance().submit {
                if (playStatusListener != null) {
                    playStatusListener?.onError(PlayError.getError(errorCode))
                }
            }
        }
    }
}