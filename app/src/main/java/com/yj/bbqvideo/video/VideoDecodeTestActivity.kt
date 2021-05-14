package com.yj.bbqvideo.video

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.yj.bbqvideo.R
import com.yj.bbqvideo.util.DLog

class VideoDecodeTestActivity : AppCompatActivity(), View.OnClickListener{
    private var hasInternetPermission = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_video_decode_test)
        checkPermissions()
        findViewById<Button>(R.id.btn_codec_decode_preview).setOnClickListener(this)
        findViewById<Button>(R.id.btn_ffmpeg_decode_preview).setOnClickListener(this)
    }

    /**
     * Called when a view has been clicked.
     *
     * @param v The view that was clicked.
     */
    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.btn_codec_decode_preview -> {
                intent = Intent(this, MediaCodecPreviewActivity::class.java)
                startActivity(intent)
            }
            R.id.btn_ffmpeg_decode_preview -> {
                intent = Intent(this, FFMpegPreviewActivity::class.java)
                startActivity(intent)
            }
        }
    }

    private fun checkPermissions() {
        DLog.d(TAG, "checkPermissions")
        if (!hasInternetPermission()) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.INTERNET), REQUEST_INTERNET
            )
        } else {
            hasInternetPermission = true
        }
    }
    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        grantResults: IntArray
    ) {
        if (requestCode == REQUEST_INTERNET
            && grantResults[0] == PackageManager.PERMISSION_GRANTED
        ) {
            DLog.d(TAG, "Activity has been granted internet permission")
            hasInternetPermission = true
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    private fun hasInternetPermission(): Boolean {
        return ContextCompat.checkSelfPermission(
            this,
            Manifest.permission.INTERNET
        ) == PackageManager.PERMISSION_GRANTED
    }

    companion object {
        private val TAG = VideoDecodeTestActivity::class.simpleName
        private const val REQUEST_INTERNET = 1
    }
}