package com.yj.player.preview

import android.view.Surface

interface IVideoPreviewer {
    fun setSource(url: String)
    fun setSurface(surface: Surface, width: Int, height: Int)
    fun release()
}