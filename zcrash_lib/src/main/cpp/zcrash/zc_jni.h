//
// Created by zhaoyu on 2023/6/5.
//

#ifndef JNIPROJECT_ZC_JNI_H
#define JNIPROJECT_ZC_JNI_H

#include <cstdint>
#include <sys/types.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

//#define ZC_JNI_IGNORE_PENDING_EXCEPTION()                 \
//    do {                                                  \
//        if((*env)->ExceptionCheck(env))                   \
//        {                                                 \
//            (*env)->ExceptionClear(env);                  \
//        }                                                 \
//    } while(0)
//
//#define ZC_JNI_CHECK_PENDING_EXCEPTION(label)             \
//    do {                                                  \
//        if((*env)->ExceptionCheck(env))                   \
//        {                                                 \
//            (*env)->ExceptionClear(env);                  \
//            goto label;                                   \
//        }                                                 \
//    } while(0)
//
//#define ZC_JNI_CHECK_NULL_AND_PENDING_EXCEPTION(v, label) \
//    do {                                                  \
//        XC_JNI_CHECK_PENDING_EXCEPTION(label);            \
//        if(NULL == (v)) goto label;                       \
//    } while(0)

#define ZC_JNI_VERSION    JNI_VERSION_1_6
#define ZC_JNI_CLASS_NAME "zcrash/NativeHandler"

#ifdef __cplusplus
}
#endif

#endif
