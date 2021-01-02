package com.yj.bbqvideo.video

import android.os.Bundle
import android.text.TextUtils
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.AppCompatEditText
import androidx.appcompat.widget.AppCompatSeekBar
import com.yj.bbqvideo.R
import com.yj.bbqvideo.util.DLog
import com.yj.bbqvideo.util.DTextUtil
import com.yj.player.PlayerManager
import com.yj.player.listener.PlayError
import com.yj.player.listener.PlayStatusListener
import com.yj.player.render.FilterType
import com.yj.player.render.NativeFilterProxy
import com.yj.player.render.RenderMode
import com.yj.player.view.NativeVideoPreviewer

class CodecDecodePreviewActivity : AppCompatActivity(), View.OnClickListener, PlayStatusListener {
    private val nativeVideoPreviewer: NativeVideoPreviewer by lazy {
        findViewById(R.id.video_previewer)
    }

    private val filter: NativeFilterProxy by lazy {
        PlayerManager.createNativeFilterProxy(FilterType.VIDEO)
    }

    private val urlEditText: AppCompatEditText by lazy {
        findViewById(R.id.url_edit)
    }

    private val progressSeekBar: AppCompatSeekBar by lazy {
        findViewById(R.id.progress_seek_bar)
    }

    private val prepareBtn: Button by lazy {
        findViewById(R.id.prepare)
    }

    private val playBtn: Button by lazy {
        findViewById(R.id.play)
    }

    private val pauseBtn: Button by lazy {
        findViewById(R.id.pause)
    }

    private val resumeBtn: Button by lazy {
        findViewById(R.id.resume)
    }

    private val seekBtn: Button by lazy {
        findViewById(R.id.seek)
    }

    private val stopBtn: Button by lazy {
        findViewById(R.id.stop)
    }

    private var loadingDialog: LoadingDialog? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_codec_decode_preview)
        initView()
        nativeVideoPreviewer.run {
            setRenderMode(RenderMode.RENDERMODE_WHEN_DIRTY)
            setFilter(filter)
            setPlayStatusListener(this@CodecDecodePreviewActivity)
        }
    }

    private fun initView() {
        prepareBtn.setOnClickListener(this)
        playBtn.setOnClickListener(this)
        pauseBtn.setOnClickListener(this)
        resumeBtn.setOnClickListener(this)
        seekBtn.setOnClickListener(this)
        stopBtn.setOnClickListener(this)
    }

    override fun onResume() {
        super.onResume()
        DLog.d(TAG, "onResume")
        nativeVideoPreviewer.onResume()
    }

    override fun onPause() {
        super.onPause()
        DLog.d(TAG, "onPause")
        nativeVideoPreviewer.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        DLog.d(TAG, "onDestroy")
        nativeVideoPreviewer.onDestroy()
    }

    /**
     * Called when a view has been clicked.
     *
     * @param v The view that was clicked.
     */
    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.prepare -> {
                val url = urlEditText.text.toString()
                if (TextUtils.isEmpty(url)) {
                    DTextUtil.showToast(this, "URL is NULL")
                } else {
                    nativeVideoPreviewer.run {
                        setDataSource(url)
                        prepare()
                    }
                    showLoading("正在解析视频URL中", false)
                }
            }
            R.id.play -> {
                nativeVideoPreviewer.playVideo()
            }
            R.id.pause -> {
                nativeVideoPreviewer.pauseVideo()
            }
            R.id.resume -> {
                nativeVideoPreviewer.resumeVideo()
            }
            R.id.seek -> {
                val progress = progressSeekBar.progress
                nativeVideoPreviewer.seekVideo(progress)
            }
            R.id.stop -> {
                nativeVideoPreviewer.stopVideo()
            }
        }
    }

    private fun showLoading(msg: String, touch: Boolean) {
        runOnUiThread {
            if (loadingDialog == null) {
                loadingDialog = LoadingDialog()
            } else {
                loadingDialog?.dismiss()
            }
            loadingDialog?.setMsg(msg)
                ?.setOnTouchOutside(touch)
                ?.show(supportFragmentManager, "loading")
        }
    }

    private fun hideLoading() {
        runOnUiThread {
            if (loadingDialog != null) {
                loadingDialog?.dismiss()
            }
        }
    }

    private fun setButtonEnabled(enabled: Boolean) {
        runOnUiThread {
            playBtn.isEnabled = enabled
            pauseBtn.isEnabled = enabled
            resumeBtn.isEnabled = enabled
            seekBtn.isEnabled = enabled
            stopBtn.isEnabled = enabled
        }
    }

    override fun onPrepared(duration: Long) {
        hideLoading()
        setButtonEnabled(true)
        DTextUtil.showToast(this, "Video has been prepared, video duration is $duration s")
    }

    override fun onProgress(presentationTime: Long) {
        DTextUtil.showToast(this, "Video presentationTime is $presentationTime s")
    }

    override fun onError(error: PlayError) {
        hideLoading()
        setButtonEnabled(false)
        DTextUtil.showToast(this, "Video play failed: error = ${error.description}")
    }

    companion object {
        private val TAG = CodecDecodePreviewActivity::class.simpleName
    }
}