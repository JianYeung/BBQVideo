package com.yj.bbqvideo.opengl

import android.os.Bundle
import android.view.SurfaceHolder
import androidx.appcompat.app.AppCompatActivity
import com.yj.bbqvideo.R
import com.yj.player.PlayerManager
import com.yj.player.render.NativeGLRenderProxy
import com.yj.player.render.FilterType
import com.yj.player.render.RendMode

class TriangleTestActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_triangle_test)
    }

    override fun onPause() {
        super.onPause()
    }

    override fun onResume() {
        super.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
    }
}