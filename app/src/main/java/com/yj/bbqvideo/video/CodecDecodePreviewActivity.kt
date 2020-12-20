package com.yj.bbqvideo.video

import android.os.Bundle
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.AppCompatEditText
import androidx.appcompat.widget.AppCompatSeekBar
import com.yj.bbqvideo.R
import com.yj.bbqvideo.util.DLog
import com.yj.player.PlayerManager
import com.yj.player.render.FilterType
import com.yj.player.render.NativeFilterProxy
import com.yj.player.render.RenderMode
import com.yj.player.view.NativeVideoPreviewer

class CodecDecodePreviewActivity : AppCompatActivity(), View.OnClickListener {
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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_codec_decode_preview)
        initView()
        nativeVideoPreviewer.run {
            setRenderMode(RenderMode.RENDERMODE_WHEN_DIRTY)
            setFilter(filter)
        }
    }

    private fun initView() {
        findViewById<Button>(R.id.confirm).setOnClickListener(this)
        findViewById<Button>(R.id.play).setOnClickListener(this)
        findViewById<Button>(R.id.pause).setOnClickListener(this)
        findViewById<Button>(R.id.resume).setOnClickListener(this)
        findViewById<Button>(R.id.seek).setOnClickListener(this)
        findViewById<Button>(R.id.stop).setOnClickListener(this)
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
            R.id.confirm -> {
                val url = urlEditText.text.toString()
                nativeVideoPreviewer.setDataSource(url)
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

    companion object {
        private val TAG = CodecDecodePreviewActivity::class.simpleName
    }
}