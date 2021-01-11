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
import com.yj.player.videoPlayer.VideoPlayerError
import com.yj.player.videoPlayer.VideoPlayerStatusListener
import com.yj.player.filter.FilterType
import com.yj.player.render.RenderMode
import com.yj.player.view.CustomVideoPreviewer

class MediaCodecPreviewActivity : AppCompatActivity(), View.OnClickListener, VideoPlayerStatusListener {
    private val customVideoPreviewer: CustomVideoPreviewer by lazy {
        findViewById(R.id.video_previewer)
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
        setContentView(R.layout.activity_codec_preview)
        initView()
        customVideoPreviewer.run {
            setRenderMode(RenderMode.RENDERMODE_WHEN_DIRTY)
            setFilterType(FilterType.VIDEO)
            setPlayerStatusListener(this@MediaCodecPreviewActivity)
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
        customVideoPreviewer.onResume()
    }

    override fun onPause() {
        super.onPause()
        DLog.d(TAG, "onPause")
        customVideoPreviewer.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        DLog.d(TAG, "onDestroy")
        customVideoPreviewer.onDestroy()
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
                    customVideoPreviewer.run {
                        setDataSource(url)
                        prepare()
                    }
                    showLoading("正在解析视频URL中", false)
                }
            }
            R.id.play -> {
                customVideoPreviewer.playVideo()
            }
            R.id.pause -> {
                customVideoPreviewer.pauseVideo()
            }
            R.id.resume -> {
                customVideoPreviewer.resumeVideo()
            }
            R.id.seek -> {
                val progress = progressSeekBar.progress
                customVideoPreviewer.seekVideo(progress)
            }
            R.id.stop -> {
                customVideoPreviewer.stopVideo()
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

    override fun onPrepared() {
        hideLoading()
        setButtonEnabled(true)
        DTextUtil.showToast(this, "Video has been prepared")
    }

    override fun onProgress(presentationTime: Long) {
        DTextUtil.showToast(this, "Video presentationTime is $presentationTime s")
    }

    override fun onError(errorVideo: VideoPlayerError) {
        hideLoading()
        setButtonEnabled(false)
        DTextUtil.showToast(this, "Video play failed: error = ${errorVideo.description}")
    }

    companion object {
        private val TAG = MediaCodecPreviewActivity::class.simpleName
    }
}