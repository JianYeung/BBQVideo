package com.yj.bbqvideo.opengl

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Button
import com.yj.bbqvideo.R

class OpenGLTestActivity : AppCompatActivity(), View.OnClickListener {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_opengl_test)
        findViewById<Button>(R.id.btn_triangle_test).setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        when(v?.id) {
            R.id.btn_triangle_test -> {
                intent = Intent(this, TriangleTestActivity::class.java)
                startActivity(intent)
            }
        }
    }

}