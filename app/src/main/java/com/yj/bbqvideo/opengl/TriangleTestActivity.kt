package com.yj.bbqvideo.opengl

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.yj.bbqvideo.R
import com.yj.player.render.NativeGLSurfaceView

class TriangleTestActivity : AppCompatActivity() {

    private lateinit var nativeSurfaceView: NativeGLSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_triangle_test)
        nativeSurfaceView = findViewById(R.id.triangle_surfaceView);
    }

    override fun onPause() {
        super.onPause()
        nativeSurfaceView.onPause()
    }

    override fun onResume() {
        super.onResume()
        nativeSurfaceView.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
        nativeSurfaceView.onDestroy()
    }
}