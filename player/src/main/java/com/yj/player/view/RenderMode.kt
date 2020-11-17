package com.yj.player.view

enum class RenderMode(data: Int) {
    /**
     * The renderer only renders
     * when the surface is created, or when {@link #requestRender} is called.
     *
     * @see NativeGLSurfaceView#setRenderMode(renderMode)
     * @see NativeGLSurfaceView#requestRender()
     */
    RENDERMODE_WHEN_DIRTY(0),

    /**
     * The renderer is called
     * continuously to re-render the scene.
     *
     * @see NativeGLSurfaceView#setRenderMode(renderMode)
     */
    RENDERMODE_CONTINUOUSLY(1),

    /**
     *
     */
    UNKNOWN(0xFF);

    val value = data

    private fun getValues(): Array<RenderMode> {
        return values()
    }

    fun find(data: Int): RenderMode {
        val values = getValues()
        for (v in values) {
            if (v.value == data) {
                return v
            }
        }
        return UNKNOWN
    }
}