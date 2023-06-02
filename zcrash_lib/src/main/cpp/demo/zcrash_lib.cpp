#include <jni.h>
#include <string>
#include "../mylog.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_android_car_zcrash_1lib_demo_NativeLib_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_android_car_zcrash_1lib_demo_NativeLib_visitField(JNIEnv *env, jobject thiz) {
    jclass j_cls = env->GetObjectClass(thiz);
    jfieldID j_fieldId = env->GetFieldID(j_cls, "name", "Ljava/lang/String;");
    jstring j_str = static_cast<jstring>(env->GetObjectField(thiz, j_fieldId));
    //获取到实例变量值
    char *name = const_cast<char *>(env->GetStringUTFChars(j_str, nullptr));
    LOGD("native gets field name: %s", name);
    //改变变量的值
    jstring newName = env->NewStringUTF("Dog");
    env->SetObjectField(thiz, j_fieldId, newName);

    jfieldID j_staticFieldId = env->GetStaticFieldID(j_cls, "staticName", "Ljava/lang/String;");
    jstring j_staticStr = static_cast<jstring>(env->GetStaticObjectField(j_cls, j_staticFieldId));
    const char *staticName = env->GetStringUTFChars(j_staticStr, nullptr);

    LOGD("native gets static field name: %s", staticName);

    jstring newStaticName = env->NewStringUTF("Static Dog");
    env->SetStaticObjectField(j_cls, j_staticFieldId, newStaticName);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_android_car_zcrash_1lib_demo_NativeLib_createPerson(JNIEnv *env, jobject thiz) {
    jclass j_cls = env->FindClass("com/android/car/zcrash_lib/demo/Person");
    jmethodID j_methodId = env->GetMethodID(j_cls, "<init>", "(Ljava/lang/String;I)V");
    return env->NewObject(j_cls, j_methodId, env->NewStringUTF("孙悟空"), 20);
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_android_car_zcrash_1lib_demo_NativeLib_createPersons(JNIEnv *env, jobject thiz) {
    jclass j_cls = env->FindClass("com/android/car/zcrash_lib/demo/Person");
    jmethodID j_methodId = env->GetMethodID(j_cls, "<init>", "(Ljava/lang/String;)V");
    jobjectArray j_array = env->NewObjectArray(5, j_cls, nullptr);

    for (int i = 0; i < 5; ++i) {
        jobject obj = env->NewObject(j_cls, j_methodId, env->NewStringUTF("猪八戒"));
        env->SetObjectArrayElement(j_array, i, obj);
    }
    return j_array;
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_android_car_zcrash_1lib_demo_NativeLib_getPersons(JNIEnv *env, jobject thiz,
                                                           jobjectArray names) {
    jclass j_cls = env->FindClass("com/android/car/zcrash_lib/demo/Person");
    jmethodID j_methodId = env->GetMethodID(j_cls, "<init>", "(Ljava/lang/String;)V");

    if (names == nullptr) {
        return nullptr;
    }

    jsize j_length = env->GetArrayLength(names);

    jobjectArray j_array = env->NewObjectArray(j_length, j_cls, nullptr);

    for (int i = 0; i < j_length; ++i) {
        jobject name = (env->GetObjectArrayElement(names, i));

        if (name == nullptr) {
            return nullptr;
        }
        jobject obj = env->NewObject(j_cls, j_methodId, name);
        env->SetObjectArrayElement(j_array, i, obj);
        env->DeleteLocalRef(obj);
    }
    return j_array;
}