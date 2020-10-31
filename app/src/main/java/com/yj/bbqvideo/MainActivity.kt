package com.yj.bbqvideo

import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.yj.bbqvideo.opengl.OpenGLTestActivity
import com.yj.bbqvideo.video.VideoPreviewerActivity

class MainActivity : AppCompatActivity(), View.OnClickListener  {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        findViewById<Button>(R.id.btn_open_gl_test).setOnClickListener(this)
        findViewById<Button>(R.id.btn_video_previewer).setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.btn_open_gl_test -> {
                intent = Intent(this, OpenGLTestActivity::class.java)
                startActivity(intent)
            }

            R.id.btn_video_previewer -> {
                intent = Intent(this, VideoPreviewerActivity::class.java)
                startActivity(intent)
            }
        }
    }

}
