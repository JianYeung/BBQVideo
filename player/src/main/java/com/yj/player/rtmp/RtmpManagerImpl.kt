package com.yj.player.rtmp

import com.yj.player.jni.RtmpHelper


class RtmpManagerImpl {
    fun setLiveUrl(url: String) {
        RtmpHelper.setLiveUrl(url)
    }

    fun startStream(): Boolean {
        return RtmpHelper.startStream()
    }
}