package com.yj.bbqvideo.opengl

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.yj.bbqvideo.R
import com.yj.bbqvideo.util.DLog
import com.yj.player.filter.FilterType
import com.yj.player.render.RenderMode
import com.yj.player.view.CustomGLSurfaceView

class TriangleTestActivity : AppCompatActivity() {

    private val nativeGLSurfaceView: CustomGLSurfaceView by lazy {
        findViewById(R.id.triangle_surfaceView)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_triangle_test)
        nativeGLSurfaceView.run {
            setRenderMode(RenderMode.RENDERMODE_WHEN_DIRTY)
            setFilterType(FilterType.TRIANGLE)
        }
    }

    override fun onResume() {
        super.onResume()
        DLog.d(TAG, "onResume")
        nativeGLSurfaceView.onResume()
    }

    override fun onPause() {
        super.onPause()
        DLog.d(TAG, "onPause")
        nativeGLSurfaceView.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        DLog.d(TAG, "onDestroy")
        nativeGLSurfaceView.onDestroy()
    }

    companion object {
        private val TAG = TriangleTestActivity::class.simpleName
    }
}