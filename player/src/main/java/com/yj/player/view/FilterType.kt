package com.yj.player.view

enum class FilterType(data: Int) {
    /**
     *
     */
    NORMAL(0),

    /**
     *
     */
    TRIANGLE(1),

    /**
     *
     */
    CAMERA(2),

    /**
     *
     */
    UNKNOWN(0xFF);

    val value = data

    private fun getValues(): Array<FilterType> {
        return values()
    }

    fun find(data: Int): FilterType {
        val values = getValues()
        for (v in values) {
            if (v.value == data) {
                return v
            }
        }
        return UNKNOWN
    }
}