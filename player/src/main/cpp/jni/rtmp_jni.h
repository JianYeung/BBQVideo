//
// Created by jian.yeung on 2020/9/8.
//

#ifndef BBQVIDEO_RTMP_JNI_H
#define BBQVIDEO_RTMP_JNI_H

#include <RtmpManager.h>
#include <DLog.h>

const char *RTMP_JNI_TAG = "RTMP_JNI";

jboolean setLiveUrl(JNIEnv *env, jobject jobj, jstring jstr_url) {
    if (jstr_url == nullptr) {
        DLOGE(RTMP_JNI_TAG, "live url is null");
        return JNI_FALSE;
    }
    const char *c_url = env->GetStringUTFChars(jstr_url, nullptr);
    string url = string(c_url);
    RtmpManager::Instance().setLiveUrl(url);
    env->ReleaseStringUTFChars(jstr_url, c_url);
    return JNI_TRUE;
}

jboolean initVideo(JNIEnv *env, jobject jobj, jint width, jint height) {
    if (width <= 0 || height <= 0) {
        DLOGE(RTMP_JNI_TAG, "video width or height is 0");
        return JNI_FALSE;
    }
    RtmpManager::Instance().initVideo(width, height);
    return JNI_TRUE;
}

jboolean startStream(JNIEnv *env, jobject jobj) {
    if (RtmpManager::Instance().getStreamStatus()) {
        DLOGE(RTMP_JNI_TAG, "It is in Streaming");
        return JNI_FALSE;
    }
    return RtmpManager::Instance().startStream();
}

jboolean stopStream(JNIEnv *env, jobject jobj) {
    return RtmpManager::Instance().stopStream();
}

static JNINativeMethod gRTMPMethods[] = {
        {"nativeSetLiveUrl",  "(Ljava/lang/String;)Z", (void *) setLiveUrl},
        {"nativeInitVideo",   "(II)Z",                 (void *) initVideo},
        {"nativeStartStream", "()Z",                   (void *) startStream},
        {"nativeStopStream",  "()Z",                   (void *) stopStream},
};

static jint registerNativeRTMPMethods(JNIEnv *env) {
    jclass clazz = env->FindClass("com/yj/player/jni/RtmpHelper");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gRTMPMethods, sizeof(gRTMPMethods) / sizeof(gRTMPMethods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_OK;
}

#endif //BBQVIDEO_RTMP_JNI_H
