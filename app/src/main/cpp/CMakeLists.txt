# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

#cmake版本 cmake中不区分大小写
cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("jniproject")

file(GLOB SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

#add_executable(${PROJECT_NAME} ${SRC_FILES})
#创建一个库 可以多次调用创建多个库
add_library( # 设置库的名称.
        jniproject
        # 设置库的类型 SHARED or STATIC
        SHARED
        # 设置源文件
        #             native-lib.cpp
        ${SRC_FILES}
        )

#搜索指定的预构建库（CMake默认包含系统库）
find_library( # 设置路径变量的名称.
        log-lib
        # 指定要CMake查找的NDK库的名称.
        log)

#指定目标链接库
target_link_libraries( # 指定目标库.
        jniproject
        # 将目标库链接到日志库包含在NDK中
        ${log-lib})