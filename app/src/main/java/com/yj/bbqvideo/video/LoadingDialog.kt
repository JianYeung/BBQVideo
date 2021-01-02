package com.yj.bbqvideo.video

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.Window
import android.widget.TextView
import androidx.annotation.Nullable
import androidx.fragment.app.DialogFragment
import com.yj.bbqvideo.R

/**
 * 等待弹框
 */
class LoadingDialog : DialogFragment() {
    private var msg = "正在加载"
    private var onTouchOutside = true
    private var textView: TextView? = null

    @Nullable
    override fun onCreateView(
        inflater: LayoutInflater,
        @Nullable container: ViewGroup?,
        @Nullable savedInstanceState: Bundle?
    ): View {
        dialog?.window?.setBackgroundDrawableResource(R.color.transparent)
        dialog?.requestWindowFeature(Window.FEATURE_NO_TITLE)
        dialog?.setCanceledOnTouchOutside(onTouchOutside)
        val loadingView: View = inflater.inflate(R.layout.dialog_loading, container)
        textView = loadingView.findViewById(R.id.textView)
        textView?.text = msg
        return loadingView
    }

    fun setMsg(msg: String): LoadingDialog {
        this.msg = msg
        if (textView != null) {
            textView?.text = msg
        }
        return this
    }

    fun setOnTouchOutside(onTouchOutside: Boolean): LoadingDialog {
        this.onTouchOutside = onTouchOutside
        if (dialog != null) {
            dialog?.setCanceledOnTouchOutside(onTouchOutside)
        }
        return this
    }
}
