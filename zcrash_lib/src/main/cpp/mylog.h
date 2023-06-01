//
// Created by zhaoyu on 2023/5/31.
//

#ifndef JNIPROJECT_MYLOG_H
#define JNIPROJECT_MYLOG_H

#include <iostream>
#include <android/log.h>

#define LOGV(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, "zcrash", fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG , "zcrash", fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO  , "zcrash", fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN  , "zcrash", fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR  , "zcrash", fmt, ##__VA_ARGS__)

class mylog {
};

#endif //JNIPROJECT_MYLOG_H
