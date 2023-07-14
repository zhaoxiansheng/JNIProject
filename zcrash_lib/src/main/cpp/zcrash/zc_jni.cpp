//
// Created by zhaoyu on 2023/6/5.
//

#include "zc_jni.h"

#include <stdint.h>
#include <sys/types.h>
#include <jni.h>
#include "zc_test.h"
#include "../common/xcc_errno.h"
#include "zc_common.h"

//忽略了-Wgnu-statement-expression警告
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-statement-expression"

static int zc_jni_inited = 0;

//回调java层
#define XC_CRASH_CALLBACK_METHOD_NAME      "crashCallback"
#define XC_CRASH_CALLBACK_METHOD_SIGNATURE "(Ljava/lang/String;Ljava/lang/String;ZZLjava/lang/String;)V"

//初始化优先申请内存的大小，防止发生crash的时候无法申请内存
#define XC_CRASH_EMERGENCY_BUF_LEN         (30 * 1024)

//the log file
//优先预留FD 防止发生Crash的时候无法申请FD
static int xc_crash_prepared_fd = -1;

//the crash
static pid_t xc_crash_tid = 0;

//process statue
sig_atomic_t zc_common_native_crashed = 0;
sig_atomic_t zc_common_java_crashed = 0;

static jint zc_jni_init(JNIEnv *env,
                        jclass thiz,
                        jint api_level,
                        jstring os_version,
                        jstring abi_list,
                        jstring manufacturer,
                        jstring brand,
                        jstring model,
                        jstring build_fingerprint,
                        jstring app_id,
                        jstring app_version,
                        jstring app_lib_dir,
                        jstring log_dir,
                        jboolean crash_enable,
                        jboolean crash_rethrow,
                        jint crash_logcat_system_lines,
                        jint crash_logcat_events_lines,
                        jint crash_logcat_main_lines,
                        jboolean crash_dump_elf_hash,
                        jboolean crash_dump_map,
                        jboolean crash_dump_fds,
                        jboolean crash_dump_network_info,
                        jboolean crash_dump_all_threads,
                        jint crash_dump_all_threads_count_max,
                        jobjectArray crash_dump_all_threads_whitelist,
                        jboolean trace_enable,
                        jboolean trace_rethrow,
                        jint trace_logcat_system_lines,
                        jint trace_logcat_events_lines,
                        jint trace_logcat_main_lines,
                        jboolean trace_dump_fds,
                        jboolean trace_dump_network_info) {

    int r_crash = XCC_ERRNO_JNI;
    int r_trace = XCC_ERRNO_JNI;

    const char *c_os_version = nullptr;
    const char *c_abi_list = nullptr;
    const char *c_manufacturer = nullptr;
    const char *c_brand = nullptr;
    const char *c_model = nullptr;
    const char *c_build_fingerprint = nullptr;
    const char *c_app_id = nullptr;
    const char *c_app_version = nullptr;
    const char *c_app_lib_dir = nullptr;
    const char *c_log_dir = nullptr;

    const char **c_crash_dump_all_threads_whitelist = nullptr;
    size_t c_crash_dump_all_threads_whitelist_len = 0;

    size_t len, i;
    jstring tmp_str;
    const char *tmp_c_str;

    (void) thiz;

    if (zc_jni_inited) return XCC_ERRNO_JNI;
    zc_jni_inited = 1;

    if (!env || (!crash_enable && !trace_enable) || api_level < 0 ||
        !os_version || !abi_list || !manufacturer || !brand || !model || !build_fingerprint ||
        !app_id || !app_version || !app_lib_dir || !log_dir ||
        crash_logcat_system_lines < 0 || crash_logcat_events_lines < 0 ||
        crash_logcat_main_lines < 0 ||
        crash_dump_all_threads_count_max < 0 ||
        trace_logcat_system_lines < 0 || trace_logcat_events_lines < 0 ||
        trace_logcat_main_lines < 0)
        return XCC_ERRNO_INVAL;

    if(nullptr == (c_os_version        = (*env).GetStringUTFChars(os_version, nullptr))) goto clean;
    if(nullptr == (c_abi_list          = (*env).GetStringUTFChars(abi_list,          nullptr))) goto clean;
    if(nullptr == (c_manufacturer      = (*env).GetStringUTFChars(manufacturer,      nullptr))) goto clean;
    if(nullptr == (c_brand             = (*env).GetStringUTFChars(brand,             nullptr))) goto clean;
    if(nullptr == (c_model             = (*env).GetStringUTFChars(model,             nullptr))) goto clean;
    if(nullptr == (c_build_fingerprint = (*env).GetStringUTFChars(build_fingerprint, nullptr))) goto clean;
    if(nullptr == (c_app_id            = (*env).GetStringUTFChars(app_id,            nullptr))) goto clean;
    if(nullptr == (c_app_version       = (*env).GetStringUTFChars(app_version,       nullptr))) goto clean;
    if(nullptr == (c_app_lib_dir       = (*env).GetStringUTFChars(app_lib_dir,       nullptr))) goto clean;
    if(nullptr == (c_log_dir           = (*env).GetStringUTFChars(log_dir,           nullptr))) goto clean;

    if (0 != zc_common_init((int )api_level,
                            c_os_version,
                            c_abi_list,
                            c_manufacturer,
                            c_brand,
                            c_model,
                            c_build_fingerprint,
                            c_app_id,
                            c_app_version,
                            c_app_lib_dir,
                            c_log_dir))
        goto clean;


    clean:
    if(c_os_version != nullptr)        (*env).ReleaseStringUTFChars(os_version, c_os_version);
    if(c_abi_list != nullptr)          (*env).ReleaseStringUTFChars(abi_list, c_abi_list);
    if(c_manufacturer != nullptr)      (*env).ReleaseStringUTFChars(manufacturer, c_manufacturer);
    if(c_brand != nullptr)             (*env).ReleaseStringUTFChars(brand, c_brand);
    if(c_model != nullptr)             (*env).ReleaseStringUTFChars(model, c_model);
    if(c_build_fingerprint != nullptr) (*env).ReleaseStringUTFChars(build_fingerprint, c_build_fingerprint);
    if(c_app_id != nullptr)            (*env).ReleaseStringUTFChars(app_id, c_app_id);
    if(c_app_version != nullptr)       (*env).ReleaseStringUTFChars(app_version, c_app_version);
    if(c_app_lib_dir != nullptr)       (*env).ReleaseStringUTFChars(app_lib_dir, c_app_lib_dir);
    if(c_log_dir != nullptr)           (*env).ReleaseStringUTFChars(log_dir, c_log_dir);

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