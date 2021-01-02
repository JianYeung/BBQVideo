package com.yj.player.listener

interface PlayStatusListener {
    fun onPrepared(duration: Long)

    fun onProgress(presentationTime: Long)

    fun onError(error: PlayError)
}