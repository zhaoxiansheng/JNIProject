//
// Created by zhaoyu on 2023/6/5.
//

#include "zc_jni.h"

#include <stdint.h>
#include <sys/types.h>
#include <jni.h>
#include "zc_test.h"

//忽略了-Wgnu-statement-expression警告
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-statement-expression"

static int zc_jni_inited = 0;

//process statue
sig_atomic_t  zc_common_native_crashed    = 0;
sig_atomic_t  zc_common_java_crashed      = 0;

static jint zc_jni_init(JNIEnv       *env,
                        jclass thiz,
                        jint          api_level,
                        jstring       os_version,
                        jstring       abi_list,
                        jstring       manufacturer,
                        jstring       brand,
                        jstring       model,
                        jstring       build_fingerprint,
                        jstring       app_id,
                        jstring       app_version,
                        jstring       app_lib_dir,
                        jstring       log_dir,
                        jboolean      crash_enable,
                        jboolean      crash_rethrow,
                        jint          crash_logcat_system_lines,
                        jint          crash_logcat_events_lines,
                        jint          crash_logcat_main_lines,
                        jboolean      crash_dump_elf_hash,
                        jboolean      crash_dump_map,
                        jboolean      crash_dump_fds,
                        jboolean      crash_dump_network_info,
                        jboolean      crash_dump_all_threads,
                        jint          crash_dump_all_threads_count_max,
                        jobjectArray  crash_dump_all_threads_whitelist,
                        jboolean      trace_enable,
                        jboolean      trace_rethrow,
                        jint          trace_logcat_system_lines,
                        jint          trace_logcat_events_lines,
                        jint          trace_logcat_main_lines,
                        jboolean      trace_dump_fds,
                        jboolean      trace_dump_network_info) {
    return 0;
}

static void zc_jni_notify_java_crashed(JNIEnv *env, jclass thiz) {
    (void) env;
    (void) thiz;

    zc_common_java_crashed = 1;
}

static void zc_jni_test_crash(JNIEnv *env, jclass thiz, jint run_in_new_thread) {
    (void) env;
    (void) thiz;

    zc_test_crash(run_in_new_thread);
}

static JNINativeMethod zc_jni_methods[] = {
        {
                "nativeInit",
                "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZIIIZZZZZI[Ljava/lang/String;ZZIIIZZ)I",
                (void *) zc_jni_init
        },
        {
                "nativeNotifyJavaCrashed",
                "("
                ")"
                "V",
                (void *) zc_jni_notify_java_crashed
        },
        {
                "nativeTestCrash",
                "(I)V",
                (void *) zc_jni_test_crash
        }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    (void) reserved;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass j_cls = env->FindClass("com/android/car/zcrash/NativeHandler");

    // 注册方法
    jint r = env->RegisterNatives(j_cls, zc_jni_methods,
                                  sizeof(zc_jni_methods) / sizeof(zc_jni_methods[0]));
    if (r != JNI_OK) {
        return JNI_ERR;
    }

    env->DeleteLocalRef(j_cls);

    return ZC_JNI_VERSION;
}