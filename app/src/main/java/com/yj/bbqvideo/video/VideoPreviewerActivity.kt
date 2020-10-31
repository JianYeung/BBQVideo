package com.yj.bbqvideo.video

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.yj.bbqvideo.R
import com.yj.player.preview.IVideoPreviewer
import com.yj.player.preview.VideoPreviewerImpl

class VideoPreviewerActivity : AppCompatActivity(), SurfaceHolder.Callback {

    private lateinit var surfaceView: SurfaceView
    private var videoPreviewer: IVideoPreviewer? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_video_previewer)
        initView()
    }

    private fun initView() {
        surfaceView = findViewById(R.id.video_previewer)
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()
        surfaceView.holder.addCallback(this)
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        surfaceView.holder.removeCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        videoPreviewer = VideoPreviewerImpl()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        if (videoPreviewer != null) {
            videoPreviewer?.setSurface(holder.surface, width, height)
        }
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        if (videoPreviewer != null) {
            videoPreviewer?.release()
            videoPreviewer = null
        }
    }
}