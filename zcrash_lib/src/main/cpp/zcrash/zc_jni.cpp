//
// Created by zhaoyu on 2023/6/5.
//

#include "zc_jni.h"

#include <cstdint>
#include <sys/types.h>
#include <jni.h>

//忽略了-Wgnu-statement-expression警告
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-statement-expression"

static int zc_jni_inited = 0;

static jint zc_jni_init(JNIEnv *env,
                        jclass thiz,
                        jint api_level,
                        jstring os_version, jstring abi_list) {
    return 0;
}

static JNINativeMethod zc_jni_methods[] = {
        {
                "nativeInit",
                "(ILjava/lang/String;Ljava/lang/String;)I",
                (void *) zc_jni_init
        },
};

JNIEXPORT jint

JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
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