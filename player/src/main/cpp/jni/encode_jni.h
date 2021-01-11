//
// Created by jian.yeung on 2020/9/8.
//

#ifndef BBQVIDEO_ENCODE_JNI_H
#define BBQVIDEO_ENCODE_JNI_H

#include <video/VideoEncoder.h>
#include <video/VideoH264Encoder.h>
#include <video/VideoHardEncoder.h>

jlong CreateVideoEncoder(JNIEnv *env, jobject thiz, jboolean software_encode_enable) {
    VideoEncoder *videoEncoder = nullptr;
    if (software_encode_enable) {
        videoEncoder = new VideoH264Encoder();
    } else {
        videoEncoder = new VideoHardEncoder();
    }

    //TODO 一定要delete，不然会内存泄露
    return (long) videoEncoder;
}

static JNINativeMethod gEncodeMethods[] = {
        {"nativeCreateVideoEncoder","(Z)J", (void *) CreateVideoEncoder},
};

static jint registerNativeEncodeMethods(JNIEnv *env) {
    jclass clazz = env->FindClass("com/yj/player/jni/EncodeHelper");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gEncodeMethods,sizeof(gEncodeMethods) / sizeof(gEncodeMethods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_OK;
}

#endif //BBQVIDEO_ENCODE_JNI_H
