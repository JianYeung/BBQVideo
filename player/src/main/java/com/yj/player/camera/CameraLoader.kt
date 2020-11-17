package com.yj.player.camera

abstract class CameraLoader {

    protected var onPreviewFrame: ((data: ByteArray, format: Int, width: Int, height: Int) -> Unit)? = null

    abstract fun onResume(width: Int, height: Int)

    abstract fun onPause()

    abstract fun switchCamera()

    abstract fun getCameraOrientation(): Int

    abstract fun hasMultipleCamera(): Boolean

    fun setOnPreviewFrameListener(onPreviewFrame: (data: ByteArray, format: Int, width: Int, height: Int) -> Unit) {
        this.onPreviewFrame = onPreviewFrame
    }
}