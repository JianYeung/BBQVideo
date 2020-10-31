package com.yj.player.preview

import android.view.Surface
import com.yj.player.PlayerManager

class VideoPreviewerImpl : IVideoPreviewer {
    private var videoDecoderImpl = PlayerManager.createVideoDecoder()

    override fun setSource(url: String) {
        videoDecoderImpl.setSource(url)
    }

    override fun setSurface(surface: Surface, width: Int, height: Int) {
        videoDecoderImpl.setSurface(surface, width, height)
    }

    override fun release() {
        videoDecoderImpl.release();
    }

}