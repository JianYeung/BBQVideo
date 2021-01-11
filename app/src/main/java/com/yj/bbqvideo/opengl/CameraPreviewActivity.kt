package com.yj.bbqvideo.opengl

import android.os.Bundle
import android.view.View
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import com.yj.bbqvideo.R
import com.yj.bbqvideo.util.DLog
import com.yj.player.PlayerManager
import com.yj.player.camera.Camera2Loader
import com.yj.player.camera.CameraLoader
import com.yj.player.camera.Rotation
import com.yj.player.camera.doOnLayout
import com.yj.player.filter.FilterType
import com.yj.player.filter.NativeFilterProxy
import com.yj.player.view.CustomGLSurfaceView
import com.yj.player.render.RenderMode

class CameraPreviewActivity : AppCompatActivity() {
    private val customGLSurfaceView: CustomGLSurfaceView by lazy {
        findViewById(R.id.camera_preview_surfaceView)
    }

    private val filter: NativeFilterProxy by lazy {
        PlayerManager.createNativeFilterProxy(FilterType.CAMERA)
    }

    private val cameraLoader: CameraLoader by lazy {
        Camera2Loader(this)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_camera_preview)
        findViewById<ImageView>(R.id.img_switch_camera).run {
            if (!cameraLoader.hasMultipleCamera()) {
                visibility = View.GONE
            }
            setOnClickListener {
                cameraLoader.switchCamera()
            }
        }
        cameraLoader.setOnPreviewFrameListener { data, format, width, height ->
            filter.updatePreviewFrame(data, format, width, height)
            customGLSurfaceView.requestRender()
        }
        customGLSurfaceView.run {
            setRenderMode(RenderMode.RENDERMODE_WHEN_DIRTY)
            setFilter(filter)
        }
    }

    override fun onResume() {
        super.onResume()
        DLog.d(TAG, "onResume")
        customGLSurfaceView.onResume()
        customGLSurfaceView.doOnLayout {
            cameraLoader.onResume(it.width, it.height)
        }
    }

    override fun onPause() {
        super.onPause()
        DLog.d(TAG, "onPause")
        cameraLoader.onPause()
        customGLSurfaceView.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        DLog.d(TAG, "onDestroy")
        customGLSurfaceView.onDestroy()
    }

    private fun getRotation(orientation: Int): Rotation {
        DLog.d(TAG, "getRotation")
        return when (orientation) {
            90 -> Rotation.ROTATION_90
            180 -> Rotation.ROTATION_180
            270 -> Rotation.ROTATION_270
            else -> Rotation.NORMAL
        }
    }

    companion object {
        private val TAG = CameraPreviewActivity::class.simpleName
    }
}