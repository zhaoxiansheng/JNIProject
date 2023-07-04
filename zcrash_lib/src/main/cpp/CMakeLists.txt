# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)

# Declares and names the project.

project("zcrash")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

#######################################
# libzcrash.so
#######################################

file(GLOB_RECURSE SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library( # Sets the name of the library.
        zcrash

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${SRC_FILES})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        zcrash

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})

if(USEASAN)

    target_compile_options(zcrash PUBLIC
            #启用了 ASAN 内存错误检测工具
            -fsanitize=address
            #禁止编译器省略帧指针，以帮助生成更准确的堆栈跟踪信息
            -fno-omit-frame-pointer)

    set_target_properties(zcrash PROPERTIES
            LINK_FLAGS " \
        -fsanitize=address")

else()

    target_compile_options(zcrash PUBLIC
            -Oz
            -flto
            -ffunction-sections
            -fdata-sections)

    set_target_properties(zcrash PROPERTIES
            LINK_FLAGS " \
        -O3 \
        -flto \
        -Wl,--exclude-libs,ALL \
        -Wl,--gc-sections"
        )

endif()