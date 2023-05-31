#include <jni.h>
#include <string>
#include "mylog.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jniproject_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jniproject_MainActivity_visitField(JNIEnv *env, jobject thiz) {
    jclass j_cls = env->GetObjectClass(thiz);
    jfieldID j_fieldId = env->GetFieldID(j_cls, "name", "Ljava/lang/String;");
    jstring j_str = static_cast<jstring>(env->GetObjectField(thiz, j_fieldId));
    char *name = const_cast<char *>(env->GetStringUTFChars(j_str, nullptr));
    LOG_DEBUG("aaa");
}