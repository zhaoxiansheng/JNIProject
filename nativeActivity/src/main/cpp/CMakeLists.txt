# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)

# Declares and names the project.

project("Native-Activity")

#导入必备的native_app_glue目录
include_directories(/home/zhaoyu/AndroidStudio/AndroidSdk/ndk/25.2.9519653/sources/android/native_app_glue)

add_library(native-activity SHARED main.cpp)

find_library(log-lib log)
find_library(OPENGLES3_LIBRARY GLESv3 "OpenGL ES v3.0 library")
find_library(EGL_LIBRARY EGL "EGL 1.4 library")
find_library(android-lib android)

#编译为静态库
add_library(app_glue STATIC
        /home/zhaoyu/AndroidStudio/AndroidSdk/ndk/25.2.9519653/sources/android/native_app_glue/android_native_app_glue.c)

target_link_libraries( # Specifies the target library.
        native-activity
        app_glue #链接静态库native_app_glue
        ${log-lib}
        ${android-lib} #链接android库
        ${OPENGLES3_LIBRARY} #链接OpenGLES库
        ${EGL_LIBRARY} #链接EGL库
        )