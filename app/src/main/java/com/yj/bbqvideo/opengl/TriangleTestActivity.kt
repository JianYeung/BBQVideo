package com.yj.bbqvideo.opengl

import android.os.Bundle
import android.view.SurfaceHolder
import androidx.appcompat.app.AppCompatActivity
import com.yj.bbqvideo.R
import com.yj.player.PlayerManager
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.render.FilterType
import com.yj.player.render.NativeGLSurfaceView
import com.yj.player.render.RendMode

class TriangleTestActivity : AppCompatActivity() {

    private lateinit var surfaceView: NativeGLSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_triangle_test)
        surfaceView = findViewById(R.id.triangle_surfaceView);
    }

    override fun onPause() {
        super.onPause()
        surfaceView.onPause()
    }

    override fun onResume() {
        super.onResume()
        surfaceView.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
    }
}