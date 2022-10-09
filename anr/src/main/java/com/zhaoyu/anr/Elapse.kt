package com.zhaoyu.anr

import android.annotation.SuppressLint
import android.content.Context
import android.os.Looper
import android.os.Process
import android.os.SystemClock
import java.io.File
import kotlin.math.max

@SuppressLint("StaticFieldLeak")
object Elapse {

    internal const val START_FLAG = ">>>>> Dispatching to "

    internal val myPid = Process.myPid()

    internal val elapseDir = "elapse/$myPid"

    const val END_FLAG = "<<<<< Finished to "

    internal var slowThreshold: Long = -1L

    internal var idleThreshold: Long = -1L

    internal var enqueueDelay: Long = -1L

    internal var recordMaxDuration = -1

    internal var timeLineDuration = -1

    internal var maxRecordCount = -1

    internal val zeroTime = SystemClock.uptimeMillis()

    internal lateinit var monitor: ElapseMonitor

    internal lateinit var dumper: ElapseDumper

    internal lateinit var applicationId: String

    internal var debuggable: Boolean = false

    internal lateinit var logger: ElapseLogger

    internal var logLevel = ElapseLogger.LogLevel.Slow

    internal lateinit var context: Context

    init {
        val elapseDirFile = File(elapseDir)
        if (elapseDirFile.exists()) {
            elapseDirFile.delete()
        }
    }

    @JvmStatic @JvmOverloads fun init(context: Context, options: ElapseOptions = ElapseOptions()) {
        Util.checkInMainThread()
        Elapse.context = context.applicationContext
        applicationId = context.packageName
        debuggable = Util.debuggable(context)

        slowThreshold = options.slowThreshold
        idleThreshold = options.idleThreshold
        enqueueDelay = max(slowThreshold * 2, 3000L)
        recordMaxDuration = options.recordMaxDuration
        timeLineDuration = options.timeLineDuration
        logLevel = options.logLevel
        maxRecordCount = timeLineDuration / recordMaxDuration / 3
        logger = options.logger

        dumper = ElapseDumper()

        monitor = ElapseMonitor()
        monitor.start()
        dumper.start()
        Looper.getMainLooper().setMessageLogging(ElapseHandler()) // todo 可能会覆盖已存在的MessageLogging
    }

    @JvmStatic fun dump() {
        monitor.dumpTimeLine()
    }
}