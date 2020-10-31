package com.yj.bbqvideo

import org.junit.Assert.assertEquals
import org.junit.Test
import java.text.DateFormat
import java.text.SimpleDateFormat
import java.util.*

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class ExampleUnitTest {
    @Test
    fun addition_isCorrect() {
        assertEquals(4, 2 + 2)
    }


    @Test
    fun testTimeStampToString() {
        val timestamp = System.currentTimeMillis();
        var dateStr: String? = ""
        val date = Date()
        //format的格式可以任意
        //format的格式可以任意
        val sdf: DateFormat = SimpleDateFormat("yyyy/MM/dd HH:mm:ss:SSS:")
        val sdf2: DateFormat = SimpleDateFormat("yyyy-MM-dd HH/mm/ss")
        try {
            dateStr = sdf.format(date)
            println(dateStr)
            dateStr = sdf2.format(date)
            println(dateStr)
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
}