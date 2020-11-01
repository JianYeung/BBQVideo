package com.yj.bbqvideo.util

import android.content.Context
import android.os.Handler
import android.os.Looper
import android.widget.TextView
import android.widget.Toast

object DTextUtil {
    private val handler = Handler(Looper.getMainLooper())

    fun showToast(context: Context, msg: String) {
        handler.post {
            Toast.makeText(context.applicationContext, msg, Toast.LENGTH_SHORT).show()
        }
    }

    fun showTextView(msg: String, textView: TextView) {
        handler.post {
            textView.text = msg
        }
    }
}