package com.yj.player.videoPlayer

class VideoPlayerError internal constructor(val errorCode: Int, val description: String) {

    companion object {
        /**
         * Network not available.
         */
        val NETWORK_NOT_AVAILABLE = VideoPlayerError(0x0001, "Network not available.")
        /**
         * Player can not open the url.
         */
        val VIDEO_PLAYER_CAN_NOT_OPEN_URL = VideoPlayerError(0x1001, "Video Player can not open the url.")

        /**
         * Player is initing, please wait for a minutes.
         */
        val VIDEO_PLAYER_IS_INITING = VideoPlayerError(0x1002, "Video Player is initing, please wait for a minutes.")

        /**
         * Unknown error.
         */
        val UNKNOWN = VideoPlayerError(0xFFFF, "Unknown error")

        /**
         * Gets an object of `PlayerError` by given the player internal error code.
         *
         * @param errorCode The player internal error code
         * @return An object of `PlayerError`
         */
        fun getError(errorCode: Int): VideoPlayerError {
            when (errorCode) {
                0x0001 -> return NETWORK_NOT_AVAILABLE
                0x1001 -> return VIDEO_PLAYER_CAN_NOT_OPEN_URL
                0x1002 -> return VIDEO_PLAYER_IS_INITING
                else -> {
                }
            }
            return UNKNOWN
        }
    }
}