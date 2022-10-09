package com.zhaoyu.anr

import android.os.SystemClock
import android.util.Printer

internal class ElapseHandler : Printer {

    private var startTime = -1L

    override fun println(m: String) {
        if (m[0] == '>') { // >>>>> Dispatching to
            messageStarted(m)
        } else if (m[0] == '<') { // <<<<< Finished to
            if (startTime > 0) {  // 纠错
                messageFinished()
            }
        }
    }

    private fun messageStarted(m: String) {
        startTime = SystemClock.uptimeMillis()
        Elapse.monitor.startMonitor(startTime, m)
    }

    private fun messageFinished() {
        Elapse.monitor.finishMonitor()
    }
}
