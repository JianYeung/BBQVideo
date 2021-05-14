//
// Created by jian.yeung on 2020/9/8.
//

#include <jni.h>
#include <cassert>
#include "onLoad.h"
#include "ByteUtils.h"
#include "filter_jni.h"
#include "gl_render_jni.h"
#include "video_player_jni.h"
#include "encode_jni.h"
#include "rtmp_jni.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/jni.h>

#ifdef __cplusplus
}
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    jint result = JNI_ERR;
    assert(vm != nullptr);
    javaVm = vm;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }

    if (av_jni_set_java_vm(vm, nullptr) != JNI_OK) {
        return JNI_ERR;
    }

    assert(env != nullptr);
    jniEnv = env;
    //动态注册，自定义函数
    if (registerNativeFilterMethods(env) != JNI_OK) {
        return result;
    }

    if (registerNativeGLRenderMethods(env) != JNI_OK) {
        return result;
    }

    if (registerNativeVideoPlayerMethods(env) != JNI_OK) {
        return result;
    }

#ifdef ENCODER_SUPPORTED
    if (registerNativeEncodeMethods(env) != JNI_OK) {
        return result;
    }
#endif

#ifdef RTMP_SUPPORTED
    if (registerNativeRTMPMethods(env) != JNI_OK) {
        return result;
    }
#endif

    return JNI_VERSION_1_6;
}