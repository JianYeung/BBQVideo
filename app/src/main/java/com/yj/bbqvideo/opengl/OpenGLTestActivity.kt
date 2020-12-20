package com.yj.bbqvideo.opengl

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Button
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.yj.bbqvideo.R
import com.yj.bbqvideo.util.DLog

class OpenGLTestActivity : AppCompatActivity(), View.OnClickListener {
    private var hasCameraPermission = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_opengl_test)
        checkPermissions()
        findViewById<Button>(R.id.btn_triangle_test).setOnClickListener(this)
        findViewById<Button>(R.id.btn_camera_preview).setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.btn_triangle_test -> {
                intent = Intent(this, TriangleTestActivity::class.java)
                startActivity(intent)
            }
            R.id.btn_camera_preview -> {
                if (!hasCameraPermission) {
                    return
                }
                intent = Intent(this, CameraPreviewActivity::class.java)
                startActivity(intent)
            }
        }
    }

    private fun checkPermissions() {
        DLog.d(TAG, "checkPermissions")
        if (!hasCameraPermission()) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.CAMERA), REQUEST_CAMERA
            )
        } else {
            hasCameraPermission = true
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        grantResults: IntArray
    ) {
        if (requestCode == REQUEST_CAMERA
            && grantResults[0] == PackageManager.PERMISSION_GRANTED
        ) {
            DLog.d(TAG, "Activity has been granted camera permission")
            hasCameraPermission = true
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    private fun hasCameraPermission(): Boolean {
        return ContextCompat.checkSelfPermission(
            this,
            Manifest.permission.CAMERA
        ) == PackageManager.PERMISSION_GRANTED
    }

    companion object {
        private val TAG = OpenGLTestActivity::class.simpleName
        private const val REQUEST_CAMERA = 1
    }
}