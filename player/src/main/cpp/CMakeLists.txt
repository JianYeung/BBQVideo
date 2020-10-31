# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CORE_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(JNI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/jni)
set(UTIL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/util)
set(RENDER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/render)
set(FILTER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/filter)
set(DECODER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/decode)
set(ENCODER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/encode)
set(RTMP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rtmp)
set(GLM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm-master)

set(NATIVE_LIBS_DIR ${PROJECT_SOURCE_DIR}/src/main/nativeLibs/${ANDROID_ABI})

add_definitions(-DRENDER_SUPPORTED)
#add_definitions(-DENCODER_SUPPORTED)
#add_definitions(-DRTMP_SUPPORTED)

#设置OpenGLES, 如果当前系统版本过低设置拒绝编译
if (${ANDROID_PLATFORM_LEVEL} LESS 11)
    message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 (currently using ${ANDROID_PLATFORM_LEVEL}).")
    return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
    add_definitions("-DDYNAMIC_ES3")
    set(OPENGL-lib GLESv2)
else ()
    set(OPENGL-lib GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 11)

#收集文件
file(GLOB ROOT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB ROOT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB ROOT_MD ${CMAKE_CURRENT_SOURCE_DIR}/*.md)

file(GLOB_RECURSE CORE_HEADER ${CORE_HEADER_DIR}/*.h ${CORE_HEADER_DIR}/*.hpp)
file(GLOB_RECURSE JNI_HEADER ${JNI_SOURCE_DIR}/*.h ${JNI_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE JNI_SOURCE ${JNI_SOURCE_DIR}/*.c ${JNI_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE UTIL_HEADER ${UTIL_SOURCE_DIR}/*.h ${UTIL_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE UTIL_SOURCE ${UTIL_SOURCE_DIR}/*.c ${UTIL_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE RENDER_HEADER ${RENDER_SOURCE_DIR}/*.h ${RENDER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE RENDER_SOURCE ${RENDER_SOURCE_DIR}/*.c ${RENDER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE FILTER_HEADER ${FILTER_SOURCE_DIR}/*.h ${FILTER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE FILTER_SOURCE ${FILTER_SOURCE_DIR}/*.c ${FILTER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE DECODER_HEADER ${DECODER_SOURCE_DIR}/*.h ${DECODER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE DECODER_SOURCE ${DECODER_SOURCE_DIR}/*.c ${DECODER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE ENCODER_HEADER ${ENCODER_SOURCE_DIR}/*.h ${ENCODER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE ENCODER_SOURCE ${ENCODER_SOURCE_DIR}/*.c ${ENCODER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE RTMP_HEADER ${RTMP_SOURCE_DIR}/*.h ${RTMP_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE RTMP_SOURCE ${RTMP_SOURCE_DIR}/*.c ${RTMP_SOURCE_DIR}/*.cpp)

set(ALL_SOURCE  ${ROOT_HEADER} ${ROOT_SOURCE}
                ${ROOT_MD} ${CORE_HEADER}
                ${JNI_HEADER} ${JNI_SOURCE}
                ${UTIL_HEADER} ${UTIL_SOURCE}
                ${RENDER_HEADER} ${RENDER_SOURCE}
                ${FILTER_HEADER} ${FILTER_SOURCE}
                ${DECODER_HEADER} ${DECODER_SOURCE}
                ${ENCODER_HEADER} ${ENCODER_SOURCE}
                ${RTMP_HEADER} ${RTMP_SOURCE})
source_group_by_dir(ALL_SOURCE)

add_subdirectory(${GLM_SOURCE_DIR})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
add_library( # Sets the name of the library.
        player-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${ROOT_SOURCE}
        ${JNI_SOURCE}
        ${UTIL_SOURCE}
        ${RENDER_SOURCE}
        ${FILTER_SOURCE}
        ${DECODER_SOURCE}
        ${ENCODER_SOURCE}
        ${RTMP_SOURCE} )

target_include_directories(player-lib

        PUBLIC

        ${CORE_HEADER_DIR}
        ${JNI_SOURCE_DIR}
        ${UTIL_SOURCE_DIR}
        ${RENDER_SOURCE_DIR}
        ${FILTER_SOURCE_DIR}
        ${DECODER_SOURCE_DIR}
        ${ENCODER_SOURCE_DIR}
        ${RTMP_SOURCE_DIR} )

add_library(avcodec
        SHARED
        IMPORTED)

set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${NATIVE_LIBS_DIR}/libavcodec.so)

add_library(avdevice
        SHARED
        IMPORTED)

set_target_properties(avdevice
        PROPERTIES IMPORTED_LOCATION
        ${NATIVE_LIBS_DIR}/libavdevice.so)

add_library(avfilter
        SHARED
        IMPORTED)

set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${NATIVE_LIBS_DIR}/libavfilter.so)

add_library(avformat
        SHARED
        IMPORTED)

set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${NATIVE_LIBS_DIR}/libavformat.so)

add_library(avutil
        SHARED
        IMPORTED)

set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${NATIVE_LIBS_DIR}/libavutil.so)

add_library(swresample
        SHARED
        IMPORTED)

set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${NATIVE_LIBS_DIR}/libswresample.so)

add_library(swscale
        SHARED
        IMPORTED)

set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${NATIVE_LIBS_DIR}/libswscale.so)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, yo u only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library( # Sets the name of the path variable.
        MEDIACODEC-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        mediandk)

find_library( # Sets the name of the path variable.
        EGL-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        EGL)

find_library( # Sets the name of the path variable.
        android-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        android )

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                        player-lib


                       # Links the target library to the log library
                       # included in the NDK.
                        avcodec
                        avdevice
                        avfilter
                        avformat
                        avutil
                        swresample
                        swscale
                        glm
                        ${MEDIACODEC-lib}
                        ${OPENGL-lib}
                        ${EGL-lib}
                        ${android-lib}
                        ${log-lib} )