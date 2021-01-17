# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(FFMPEG_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/include)
set(JNI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/jni)
set(UTIL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/util)
set(HANDLER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/handler)
set(FSM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fsm)
set(FRAME_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/frame)
set(FILTER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/filter)
set(RENDER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/render)
set(ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine)
set(DECODER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/decode)
set(AUDIO_DECODER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/decode/audio)
set(VIDEO_DECODER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/decode/video)
set(PLAYER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/player)
set(ENCODER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/encode)
set(AUDIO_ENCODER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/encode/audio)
set(VIDEO_ENCODER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/encode/video)
set(RTMP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rtmp)
set(GLM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm-master)
set(OBOE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/oboe-master)


set(NATIVE_LIBS_DIR ${PROJECT_SOURCE_DIR}/src/main/nativeLibs/${ANDROID_ABI})

#add_definitions(-DENCODER_SUPPORTED)
#add_definitions(-DRTMP_SUPPORTED)

#设置OpenGLES 为GLESv3
set(OPENGL-lib GLESv3)

#收集文件
file(GLOB ROOT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB ROOT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB ROOT_MD ${CMAKE_CURRENT_SOURCE_DIR}/*.md)

file(GLOB_RECURSE FFMPEG_HEADER ${FFMPEG_HEADER_DIR}/*.h ${FFMPEG_HEADER_DIR}/*.hpp)
file(GLOB_RECURSE JNI_HEADER ${JNI_SOURCE_DIR}/*.h ${JNI_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE JNI_SOURCE ${JNI_SOURCE_DIR}/*.c ${JNI_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE UTIL_HEADER ${UTIL_SOURCE_DIR}/*.h ${UTIL_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE UTIL_SOURCE ${UTIL_SOURCE_DIR}/*.c ${UTIL_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HANDLER_HEADER ${HANDLER_SOURCE_DIR}/*.h ${HANDLER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE HANDLER_SOURCE ${HANDLER_SOURCE_DIR}/*.c ${HANDLER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE FSM_HEADER ${FSM_SOURCE_DIR}/*.h ${FSM_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE FSM_SOURCE ${FSM_SOURCE_DIR}/*.c ${FSM_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE FRAME_HEADER ${FRAME_SOURCE_DIR}/*.h ${FRAME_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE FRAME_SOURCE ${FRAME_SOURCE_DIR}/*.c ${FRAME_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE FILTER_HEADER ${FILTER_SOURCE_DIR}/*.h ${FILTER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE FILTER_SOURCE ${FILTER_SOURCE_DIR}/*.c ${FILTER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE RENDER_HEADER ${RENDER_SOURCE_DIR}/*.h ${RENDER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE RENDER_SOURCE ${RENDER_SOURCE_DIR}/*.c ${RENDER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE ENGINE_HEADER ${ENGINE_SOURCE_DIR}/*.h ${ENGINE_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE ENGINE_SOURCE ${ENGINE_SOURCE_DIR}/*.c ${ENGINE_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE DECODER_HEADER ${DECODER_SOURCE_DIR}/*.h ${DECODER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE DECODER_SOURCE ${DECODER_SOURCE_DIR}/*.c ${DECODER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE AUDIO_DECODER_HEADER ${AUDIO_DECODER_SOURCE_DIR}/*.h ${AUDIO_DECODER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE AUDIO_DECODER_SOURCE ${AUDIO_DECODER_SOURCE_DIR}/*.c ${AUDIO_DECODER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE VIDEO_DECODER_HEADER ${VIDEO_DECODER_SOURCE_DIR}/*.h ${VIDEO_DECODER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE VIDEO_DECODER_SOURCE ${VIDEO_DECODER_SOURCE_DIR}/*.c ${VIDEO_DECODER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE PLAYER_HEADER ${PLAYER_SOURCE_DIR}/*.h ${PLAYER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE PLAYER_SOURCE ${PLAYER_SOURCE_DIR}/*.c ${PLAYER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE ENCODER_HEADER ${ENCODER_SOURCE_DIR}/*.h ${ENCODER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE ENCODER_SOURCE ${ENCODER_SOURCE_DIR}/*.c ${ENCODER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE AUDIO_ENCODER_HEADER ${AUDIO_ENCODER_SOURCE_DIR}/*.h ${AUDIO_ENCODER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE AUDIO_ENCODER_SOURCE ${AUDIO_ENCODER_SOURCE_DIR}/*.c ${AUDIO_ENCODER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE VIDEO_ENCODER_HEADER ${VIDEO_ENCODER_SOURCE_DIR}/*.h ${VIDEO_ENCODER_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE VIDEO_ENCODER_SOURCE ${VIDEO_ENCODER_SOURCE_DIR}/*.c ${VIDEO_ENCODER_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE RTMP_HEADER ${RTMP_SOURCE_DIR}/*.h ${RTMP_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE RTMP_SOURCE ${RTMP_SOURCE_DIR}/*.c ${RTMP_SOURCE_DIR}/*.cpp)

set(ALL_SOURCE  ${ROOT_HEADER} ${ROOT_SOURCE}
                ${ROOT_MD} ${FFMPEG_HEADER}
                ${JNI_HEADER} ${JNI_SOURCE}
                ${UTIL_HEADER} ${UTIL_SOURCE}
                ${HANDLER_HEADER} ${HANDLER_SOURCE}
                ${FSM_HEADER} ${FSM_SOURCE}
                ${FRAME_HEADER} ${FRAME_SOURCE}
                ${FILTER_HEADER} ${FILTER_SOURCE}
                ${RENDER_HEADER} ${RENDER_SOURCE}
                ${ENGINE_HEADER} ${ENGINE_SOURCE}
                ${DECODER_HEADER} ${DECODER_SOURCE}
                ${AUDIO_DECODER_HEADER} ${AUDIO_DECODER_SOURCE}
                ${VIDEO_DECODER_HEADER} ${VIDEO_DECODER_SOURCE}
                ${PLAYER_HEADER} ${PLAYER_SOURCE}
                ${ENCODER_HEADER} ${ENCODER_SOURCE}
                ${AUDIO_ENCODER_HEADER} ${AUDIO_ENCODER_SOURCE}
                ${VIDEO_ENCODER_HEADER} ${VIDEO_ENCODER_SOURCE}
                ${RTMP_HEADER} ${RTMP_SOURCE})
source_group_by_dir(ALL_SOURCE)

add_subdirectory(${GLM_SOURCE_DIR})
add_subdirectory(${OBOE_SOURCE_DIR})

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
        ${HANDLER_SOURCE}
        ${FSM_SOURCE}
        ${FRAME_SOURCE}
        ${FILTER_SOURCE}
        ${RENDER_SOURCE}
        ${ENGINE_SOURCE}
        ${DECODER_SOURCE}
        ${AUDIO_DECODER_SOURCE}
        ${VIDEO_DECODER_SOURCE}
        ${PLAYER_SOURCE}
        ${ENCODER_SOURCE}
        ${AUDIO_ENCODER_SOURCE}
        ${VIDEO_ENCODER_SOURCE}
        ${RTMP_SOURCE} )

target_include_directories(player-lib

        PUBLIC

        ${FFMPEG_HEADER_DIR}
        ${JNI_SOURCE_DIR}
        ${UTIL_SOURCE_DIR}
        ${HANDLER_SOURCE_DIR}
        ${FSM_SOURCE_DIR}
        ${FRAME_SOURCE_DIR}
        ${FILTER_SOURCE_DIR}
        ${RENDER_SOURCE_DIR}
        ${ENGINE_SOURCE_DIR}
        ${DECODER_SOURCE_DIR}
        ${AUDIO_DECODER_SOURCE_DIR}
        ${VIDEO_DECODER_SOURCE_DIR}
        ${PLAYER_SOURCE_DIR}
        ${ENCODER_SOURCE_DIR}
        ${AUDIO_ENCODER_SOURCE_DIR}
        ${VIDEO_ENCODER_SOURCE_DIR}
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
                        oboe
                        ${MEDIACODEC-lib}
                        ${OPENGL-lib}
                        ${EGL-lib}
                        ${android-lib}
                        ${log-lib} )