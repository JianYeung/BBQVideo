package com.yj.bbqvideo.opengl

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.yj.bbqvideo.R
import com.yj.bbqvideo.util.DLog
import com.yj.player.view.FilterType
import com.yj.player.view.RenderMode
import com.yj.player.view.NativeGLSurfaceView

class TriangleTestActivity : AppCompatActivity() {

    private val nativeSurfaceView: NativeGLSurfaceView by lazy {
        findViewById(R.id.triangle_surfaceView)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_triangle_test)
        nativeSurfaceView.run {
            setRenderMode(RenderMode.RENDERMODE_WHEN_DIRTY)
            setFilterType(FilterType.TRIANGLE)
        }
    }

    override fun onResume() {
        super.onResume()
        DLog.d(TAG,"onResume")
        nativeSurfaceView.onResume()
    }

    override fun onPause() {
        super.onPause()
        DLog.d(TAG,"onPause")
        nativeSurfaceView.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        DLog.d(TAG,"onDestroy")
        nativeSurfaceView.onDestroy()
    }

    companion object {
        private val TAG = TriangleTestActivity::class.simpleName
    }
}