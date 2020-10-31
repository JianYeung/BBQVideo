//
// Created by jian.yeung on 2020/9/8.
//

#ifndef BBQVIDEO_ENCODE_JNI_H
#define BBQVIDEO_ENCODE_JNI_H

#include <VideoEncoder.h>
#include <VideoH264Encoder.h>
#include <VideoHardEncoder.h>

jlong createVideoEncoder(JNIEnv *env, jobject thiz, jboolean hard_encode_enable) {
    VideoEncoder *videoEncoder = nullptr;
    if (hard_encode_enable) {
        videoEncoder = new VideoHardEncoder();
    } else {
        videoEncoder = new VideoH264Encoder();
    }

    //TODO 一定要delete，不然会内存泄露
    return (long) videoEncoder;
}

static JNINativeMethod gEncodeMethods[] = {
        {"nativeCreateVideoEncoder","(Z)J", (void *) createVideoEncoder},
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
