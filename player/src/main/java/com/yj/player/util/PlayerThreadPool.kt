package com.yj.player.util

import java.util.concurrent.BlockingQueue
import java.util.concurrent.LinkedBlockingQueue
import java.util.concurrent.ThreadPoolExecutor
import java.util.concurrent.TimeUnit

class PlayerThreadPool(
    corePoolSize: Int,
    maximumPoolSize: Int,
    keepAliveTime: Long,
    unit: TimeUnit?,
    workQueue: BlockingQueue<Runnable>?
) : ThreadPoolExecutor(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue) {

    companion object {
        private const val corePoolSize = 5
        private const val maximumPoolSize = 10
        private val blockingQueue: BlockingQueue<Runnable> = LinkedBlockingQueue()

        fun getInstance(): PlayerThreadPool {
            return Singleton.INSTANCE
        }
    }

    private class Singleton {
        companion object {
            val INSTANCE: PlayerThreadPool = PlayerThreadPool(
                corePoolSize,
                maximumPoolSize,
                60,
                TimeUnit.SECONDS,
                blockingQueue
            )
        }
    }
}