package com.zhaoyu.anr

import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.os.SystemClock
import com.zhaoyu.anr.Elapse.START_FLAG
import java.util.concurrent.ConcurrentHashMap

internal class ElapseMonitor : HandlerThread("elapse-handler-thread") {

    companion object {
        private var handler: Handler? = null

        private const val TASK_POOL_COUNT_MAX = 5

        private val taskPool = mutableListOf<Task>()

        private val timeLine = ElapseTimeLine()

        private const val keyMessageRegex = "Handler (android.app.ActivityThread\$H)"

        private const val inputMessageRegexOn = false
        private const val inputMessageRegex = "Handler (android.view.Choreographer\$FrameHandler)"

        fun obtainTask(c: Long, m: String): Task {
            val task = taskPool.removeFirstOrNull()?: Task()
            task.c = c
            task.m = m
            return task
        }

        fun recycleTask(task: Task) {
            task.m = null
            task.c = null
            if (taskPool.size < TASK_POOL_COUNT_MAX) {
                taskPool.add(task)
            }
        }

        private fun cpuTime(): Long {
            return ElapseCpuTimeTracker.getCpuTimeForTid(Elapse.myPid, Elapse.myPid) // 主线程tid=pid
        }
    }

    private lateinit var slowTask: Task

    private var curMessageCpuTime = -1L   // 记录当前消息的cpuTime

    private var lastMessageEndTime = -1L

    private var curMessageStartTime = -1L  // 记录当前record的startTime

    private var curRecord: ElapseRecord? = null  // 当前record, 是一定时间范围历史消息的聚合

    private var curRecordCPUTime = -1L    // 记录当前record的cpuTime

    private val logger: ElapseLogger = Elapse.logger


    fun startMonitor(startTime: Long, m: String) {

        curMessageStartTime = startTime

        curMessageCpuTime = cpuTime()


        if (lastMessageEndTime == -1L) {
            lastMessageEndTime = startTime
        }

        if (startTime - lastMessageEndTime > Elapse.idleThreshold) { // 记录超时的idle
            curRecord?.run {
                end = lastMessageEndTime
                cpuTime = curMessageCpuTime - curRecordCPUTime  // 误差有限
                timeLine.addRecord(this)
            }
            curRecordCPUTime = -1
            curRecord = null
            val idleRecord = ElapseRecord.obtain(
                "IDLE",
                start = lastMessageEndTime,
                end = startTime
            )
            timeLine.addRecord(idleRecord)
        }

        if (m.indexOf(keyMessageRegex, startIndex = START_FLAG.length) >= 0) { // ActivityThread.mH消息
            curRecord?.run {
                end = lastMessageEndTime
                cpuTime = curMessageCpuTime - curRecordCPUTime // 误差有限
                timeLine.addRecord(this)
                curRecordCPUTime = -1
            }
            val splits = m.split(" ")
            val key = splits[splits.size - 1].toInt()
            curRecord = ElapseRecord.obtain("KeyMsg ${ElapseKeyMessages[key]}($key)", start = startTime)
        }

        if (inputMessageRegexOn) { // always false, will delete
            if (m.indexOf(
                    inputMessageRegex,
                    startIndex = START_FLAG.length
                ) > 0
            ) { // Choreographer.FrameHandler
                val splits = m.split(" ")
                val key = splits[splits.size - 1].toInt()
                if (key == 0) { // 0 = Choreographer.MSG_DO_FRAME
                    curRecord?.run {
                        if (name != "DO_FRAME") {
                            end = lastMessageEndTime
                            cpuTime = curMessageCpuTime - curRecordCPUTime // 误差有限
                            timeLine.addRecord(this)
                            curRecordCPUTime = -1
                            curRecord = null
                        }
                    }
                    curRecord = curRecord ?: ElapseRecord.obtain(
                        "DO_FRAME",
                        start = startTime
                    ) // DO_FRAME包含Input事件
                }
            } else {
                curRecord?.run {
                    if (name == "DO_FRAME") {
                        end = lastMessageEndTime
                        cpuTime = curMessageCpuTime - curRecordCPUTime
                        timeLine.addRecord(this)
                        curRecordCPUTime = -1
                        curRecord = null
                    }
                }
            }
        }

        if (curRecordCPUTime < 0) {
            curRecordCPUTime = curMessageCpuTime
        }
        val r = curRecord?: ElapseRecord.obtain("Msgs", start = startTime)

        curRecord = r

        slowTask = obtainTask(curMessageStartTime, m)
        handler?.postDelayed(slowTask, Elapse.slowThreshold)
    }

    fun finishMonitor() {
        if (Task.samplingId > 0) {
            println("stop tracing.... 2")
//            Debug.stopMethodTracing()
            Task.samplingId = -1L
        }
        val endTime = SystemClock.uptimeMillis()
        val r = curRecord!!
        val deltaTime = endTime - curMessageStartTime
        if (deltaTime < Elapse.slowThreshold) {
            handler?.removeCallbacks(slowTask)  // 非慢消息, 移除监听

            if (endTime - r.start < Elapse.recordMaxDuration) {
                // 小于阈值，聚合
                r.count ++
            } else {
                // 大于阈值, 闭合record
                r.end = endTime
                r.count ++
                r.cpuTime = cpuTime() - curRecordCPUTime
                curRecordCPUTime = -1 // 重置
                timeLine.addRecord(r)
                curRecord = null
            }

        } else {
            // 慢消息
            // 先闭合前一个record
            val cpuTime = cpuTime()
            if (r.count > 0) { // count == 0, r和slow是同一个，不能重复添加
                r.cpuTime = cpuTime - curRecordCPUTime
                r.end = lastMessageEndTime
                r.count++
                timeLine.addRecord(r)
            }
            curRecordCPUTime = -1L

            // 再生成慢消息record
            synchronized(Task::class.java) {
                var slow = Task.map[curMessageStartTime]
                if (slow != null) {
                    Task.map.remove(curMessageStartTime)
                    slow.end = endTime
                    slow.cpuTime = cpuTime - curMessageCpuTime
                    timeLine.addRecord(slow)
                } else {
                    slow = ElapseRecord.obtain(
                        "Slow",
                        start = curMessageStartTime, end = endTime,
                        count = 1, cpuTime = (cpuTime - curMessageCpuTime)
                    )
                    timeLine.addRecord(slow)
                }
            }

            curRecord = null // 重置curRecord
        }
        lastMessageEndTime = endTime
//        logger.d(msg = "finishMonitor, spent ${deltaTime}ms, ${cpuTime() - curMessageCpuTime}ms")
    }


    override fun onLooperPrepared() {
        handler = Handler(looper)
    }

    class Task(var c: Long? = null, var m: String? = null): Runnable {


        companion object {
            val map = ConcurrentHashMap<Long, ElapseRecord>()

            @Volatile
            var samplingId = -1L
        }

        override fun run() {
            if (samplingId > 0) {
                println("stop tracing.... 1")
//                Debug.stopMethodTracing()
                samplingId = -1L
            }
            samplingId = c!!
            println("start tracing....")
//            Debug.startMethodTracingSampling(elapseDir + "/elapse-pid${Elapse.myPid}-${c}", 1024 * 1024, 1000)
            val s = m!!.substring(START_FLAG.length)
            val splits = s.split(" ")
            if (splits.size < 3) {
                return
            }

            var slow: ElapseRecord?
            synchronized(Task::class.java) {
                slow = map[c]
                if (slow == null) {
                    slow = ElapseRecord.obtain("Slow", count = 1, start = c!!)
                    map[c!!] = slow!!
                } else {
                    map.remove(c!!)
                }
            }
            slow!!.apply {
                val length = splits.size
                handler = splits.joinToString(separator = "", limit = length - 2, truncated = "")
                callback = splits[length - 2].substring(0, splits[length - 2].length - 2)
                what = splits[length - 1]
                stackTrace = Looper.getMainLooper().thread.stackTrace
            }


            handler?.postDelayed({
                Elapse.dumper.enqueue(slow!!)
            }, Elapse.enqueueDelay)

            recycleTask(this)
        }

    }

    fun dumpTimeLine() {
        handler?.post {
            timeLine.anrTimestamp = SystemClock.uptimeMillis()
            curRecord?.apply {
                timeLine.runningRecord = ElapseRecord.obtain(name,
                        start, end, count, cpuTime)
                timeLine.runningRecord?.cpuTime = cpuTime() - curRecordCPUTime
                timeLine.runningRecord?.what = what
                timeLine.runningRecord?.handler = handler
                timeLine.runningRecord?.stackTrace = Looper.getMainLooper().thread.stackTrace
            }
            Elapse.dumper.enqueue(timeLine)
        }
    }


}




