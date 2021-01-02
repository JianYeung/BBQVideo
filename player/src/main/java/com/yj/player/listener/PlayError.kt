package com.yj.player.listener

class PlayError internal constructor(val errorCode: Int, val description: String) {

    companion object {
        /**
         * Network not available.
         */
        val NETWORK_NOT_AVAILABLE = PlayError(0x0001, "Network not available.")
        /**
         * Player can not open the url.
         */
        val PLAYER_CAN_NOT_OPEN_URL = PlayError(0x1001, "Player can not open the url.")

        /**
         * Player is initing, please wait for a minutes.
         */
        val PLAYER_IS_INITING = PlayError(0x1002, "Player is initing, please wait for a minutes.")

        /**
         * Unknown error.
         */
        val UNKNOWN = PlayError(0xFFFF, "Unknown error")

        /**
         * Gets an object of `PlayerError` by given the player internal error code.
         *
         * @param errorCode The player internal error code
         * @return An object of `PlayerError`
         */
        fun getError(errorCode: Int): PlayError {
            when (errorCode) {
                0x0001 -> return NETWORK_NOT_AVAILABLE
                0x1001 -> return PLAYER_CAN_NOT_OPEN_URL
                0x1002 -> return PLAYER_IS_INITING
                else -> {
                }
            }
            return UNKNOWN
        }
    }
}