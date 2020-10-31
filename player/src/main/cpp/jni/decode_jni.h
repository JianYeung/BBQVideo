//
// Created by jian.yeung on 2020/9/8.
//

#ifndef BBQVIDEO_DECODE_JNI_H
#define BBQVIDEO_DECODE_JNI_H

#include <VideoHardDecoder.h>
#include <VideoH264Decoder.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>

#ifdef __cplusplus
}
#endif

jlong createVideoDecoder(JNIEnv *env, jobject thiz, jboolean hard_decode_enable) {
    VideoDecoder *videoDecoder = nullptr;
    if (hard_decode_enable) {
        videoDecoder = new VideoHardDecoder();
    } else {
        videoDecoder = new VideoH264Decoder();
    }

    return (long) videoDecoder;
}

void setSource(JNIEnv *env, jobject thiz, jlong native_decoder_handle, jstring j_url) {
    VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
    const char *c_url = env->GetStringUTFChars(j_url, nullptr);
    std::string url = std::string(c_url);
    videoDecoder->setSource(url);
    env->ReleaseStringUTFChars(j_url, c_url);
}

void setSurface(JNIEnv *env, jobject thiz, jlong native_decoder_handle, jobject surface, jint width,
                jint height) {
    VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
    videoDecoder->setSurface(nativeWindow, width, height);
}

void decodeRelease(JNIEnv *env, jobject thiz, jlong native_decoder_handle) {
    VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
    videoDecoder->release();
    delete((VideoHardDecoder*) videoDecoder);
}

static JNINativeMethod gDecodeMethods[] = {
        {"nativeCreateVideoDecoder","(Z)J",                         (void *)createVideoDecoder},
        {"nativeSetSource",         "(JLjava/lang/String;)V",       (void *) setSource},
        {"nativeSetSurface",        "(JLandroid/view/Surface;II)V", (void *) setSurface},
        {"nativeRelease",           "(J)V",                         (void *) decodeRelease},
};

static jint registerNativeDecodeMethods(JNIEnv *env) {
    jclass clazz = env->FindClass("com/yj/player/jni/DecodeHelper");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gDecodeMethods,sizeof(gDecodeMethods) / sizeof(gDecodeMethods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_OK;
}

#endif //BBQVIDEO_DECODE_JNI_H
