package com.yj.player.render

enum class RendMode(data: Int) {
    /**
     *
     */
    RENDERMODE_WHEN_DIRTY(0),

    /**
     *
     */
    RENDERMODE_CONTINUOUSLY(1),

    /**
     *
     */
    UNKNOWN(0xFF);

    val value = data

    private fun getValues(): Array<RendMode> {
        return values()
    }

    fun find(data: Int): RendMode {
        val values = getValues()
        for (v in values) {
            if (v.value == data) {
                return v
            }
        }
        return UNKNOWN
    }
}