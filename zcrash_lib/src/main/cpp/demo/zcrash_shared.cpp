//
// Created by zhaoyu on 2023/6/2.
//
#include <jni.h>
#include <string>
#include "../mylog.h"

#define XC_JNI_VERSION    JNI_VERSION_1_6

static jint zc_jni_init(JNIEnv *env,
                        jobject thiz,
                        jint api_level,
                        jstring os_version) {
    return 0;
}

static JNINativeMethod zc_jni_methods[] = {
        {
                "nativeInit",
                "("
                "I"
                "Ljava/lang/String;"
                "Ljava/lang/String;"
                ")"
                "I",
                (void *) zc_jni_init
        },
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env = nullptr;
    (void)reserved;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass j_cls = env->FindClass("com/android/car/zcrash_lib/demo/Shared");

    // 注册方法
    jint r = env->RegisterNatives( j_cls, zc_jni_methods, sizeof(zc_jni_methods) / sizeof(zc_jni_methods[0]));
    if (r != JNI_OK) {
        return JNI_ERR;
    }

    env->DeleteLocalRef(j_cls);

    return JNI_VERSION_1_6;
}