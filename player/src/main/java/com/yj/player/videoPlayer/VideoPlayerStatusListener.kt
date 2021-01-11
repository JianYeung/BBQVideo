package com.yj.player.videoPlayer

interface VideoPlayerStatusListener {
    fun onPrepared()

    fun onProgress(presentationTime: Long)

    fun onError(errorVideo: VideoPlayerError)
}