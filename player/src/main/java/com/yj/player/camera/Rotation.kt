package com.yj.player.camera

enum class Rotation(data: Int) {
    /**
     * Normal.
     */
    NORMAL(0),

    /**
     * Rotation 90 degree.
     */
    ROTATION_90(1),

    /**
     * Rotation 180 degree.
     */
    ROTATION_180(2),

    /**
     * Rotation 270 degree.
     */
    ROTATION_270(3),

    /**
     * Unknown
     */
    UNKNOWN(0xFF);

    var value = data


    private fun getValues(): Array<Rotation> {
        return values()
    }

    fun find(data: Int): Rotation {
        val values = getValues()
        for (v in values) {
            if (v.value == data) {
                return v
            }
        }
        return UNKNOWN
    }

    /**
     * Retrieves the int representation of the Rotation.
     *
     * @return 0, 90, 180 or 270
     */
    fun asInt(): Int {
        return when (this) {
            NORMAL -> 0
            ROTATION_90 -> 90
            ROTATION_180 -> 180
            ROTATION_270 -> 270
            else -> throw IllegalStateException("Unknown Rotation!")
        }
    }

    companion object {
        /**
         * Create a Rotation from an integer. Needs to be either 0, 90, 180 or 270.
         *
         * @param rotation 0, 90, 180 or 270
         * @return Rotation object
         */
        fun fromInt(rotation: Int): Rotation {
            return when (rotation) {
                0, 360 -> NORMAL
                90 -> ROTATION_90
                180 -> ROTATION_180
                270 -> ROTATION_270
                else -> throw IllegalStateException("$rotation is an unknown rotation. Needs to be either 0, 90, 180 or 270!")
            }
        }
    }
}