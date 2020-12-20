//
// Created by jian.yeung on 2020/9/8.
//

#ifndef BBQVIDEO_DECODE_JNI_H
#define BBQVIDEO_DECODE_JNI_H

#include <VideoHardDecoder.h>
#include <VideoH264Decoder.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <DLog.h>
#include <GLRender.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>

#ifdef __cplusplus
}
#endif

const bool DECODER_JNI_DEBUG = true;
const char *DECODER_JNI_TAG = "DecoderJni";
bool softwareDecodeEnable = false;

jlong CreateVideoDecoderHandle(JNIEnv *env, jobject thiz, jboolean software_decode_enable) {
    if (DECODER_JNI_DEBUG) {
        DFLOGD(DECODER_JNI_TAG, "CreateVideoDecoderHandle() software decode enable = %d",
               software_decode_enable);
    }
    VideoDecoder *videoDecoder;
    if (software_decode_enable) {
        softwareDecodeEnable = software_decode_enable;
        videoDecoder = new VideoH264Decoder();
    } else {
        videoDecoder = new VideoHardDecoder();
    }

    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "CreateVideoDecoderHandle() handle = %ld", (long) videoDecoder);
    }
    return (long) videoDecoder;
}

void ReleaseVideoDecoderHandle(JNIEnv *env, jobject thiz, jlong native_decoder_handle) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "ReleaseVideoDecoderHandle() handle = %ld",
               (long) native_decoder_handle);
    }
    if (native_decoder_handle != 0) {
        if (DECODER_JNI_DEBUG) {
            DFLOGD(DECODER_JNI_TAG, "software decode enable = %d",softwareDecodeEnable);
        }
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        if (videoDecoder != nullptr) {
            videoDecoder->release();
            if (softwareDecodeEnable) {
                delete ((VideoH264Decoder *)videoDecoder);
            } else {
                delete ((VideoHardDecoder *)videoDecoder);
            }
            videoDecoder = nullptr;
        }
    }
}

void SetRender(JNIEnv *env, jobject thiz, jlong native_decoder_handle, jlong native_render_handle) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "SetRender() handle = %ld", (long) native_decoder_handle);
    }
    if (native_decoder_handle != 0 && native_render_handle != 0) {
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        GLRender *glRender = reinterpret_cast<GLRender *>(native_render_handle);
        videoDecoder->setRender(glRender);
    }
}

void SetDataSource(JNIEnv *env, jobject thiz, jlong native_decoder_handle, jstring j_url) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "SetDataSource() handle = %ld", (long) native_decoder_handle);
    }
    if (native_decoder_handle != 0) {
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        const char *c_url = env->GetStringUTFChars(j_url, nullptr);
        std::string url = std::string(c_url);
        videoDecoder->setDataSource(url);
        if (DECODER_JNI_DEBUG) {
            DFLOGD(DECODER_JNI_TAG, "SetDataSource() url = %s", url.c_str());
        }
        env->ReleaseStringUTFChars(j_url, c_url);
    }
}

void SetSurface(JNIEnv *env, jobject thiz, jlong native_decoder_handle, jobject surface, jint width,
                jint height) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "SetSurface() handle = %ld", (long) native_decoder_handle);
        DFLOGD(DECODER_JNI_TAG, "SetSurface() width = %d, height = %d", width, height);
    }
    if (native_decoder_handle != 0 && surface != nullptr) {
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
        videoDecoder->setSurface(nativeWindow, width, height);
    }
}

void StartDecode(JNIEnv *env, jobject thiz, jlong native_decoder_handle) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "StartDecode() handle = %ld", (long) native_decoder_handle);
    }
    if (native_decoder_handle != 0) {
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        videoDecoder->start();
    }
}

void StopDecode(JNIEnv *env, jobject thiz, jlong native_decoder_handle) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "StopDecode() handle = %ld", (long) native_decoder_handle);
    }
    if (native_decoder_handle != 0) {
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        videoDecoder->stop();
    }
}

void ResumeDecode(JNIEnv *env, jobject thiz, jlong native_decoder_handle) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "ResumeDecode() handle = %ld", (long) native_decoder_handle);
    }
    if (native_decoder_handle != 0) {
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        videoDecoder->resume();
    }
}

void PauseDecode(JNIEnv *env, jobject thiz, jlong native_decoder_handle) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "PauseDecode() handle = %ld", (long) native_decoder_handle);
    }
    if (native_decoder_handle != 0) {
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        videoDecoder->pause();
    }
}

void SeekDecode(JNIEnv *env, jobject thiz, jlong native_decoder_handle, jint position) {
    if (DECODER_JNI_DEBUG) {
        DFLOGI(DECODER_JNI_TAG, "SeekDecode() handle = %ld", (long) native_decoder_handle);
        DFLOGD(DECODER_JNI_TAG, "SeekDecode() position = %d", position);
    }
    if (native_decoder_handle != 0) {
        VideoDecoder *videoDecoder = reinterpret_cast<VideoDecoder *>(native_decoder_handle);
        videoDecoder->seek(position);
    }
}

static JNINativeMethod gDecodeMethods[] = {
        {"nativeCreateVideoDecoderHandle",  "(Z)J",                         (void *) CreateVideoDecoderHandle},
        {"nativeReleaseVideoDecoderHandle", "(J)V",                         (void *) ReleaseVideoDecoderHandle},
        {"nativeSetRender",                 "(JJ)V",                        (void *) SetRender},
        {"nativeSetDataSource",             "(JLjava/lang/String;)V",       (void *) SetDataSource},
        {"nativeSetSurface",                "(JLandroid/view/Surface;II)V", (void *) SetSurface},
        {"nativeStartDecode",               "(J)V",                         (void *) StartDecode},
        {"nativeStopDecode",                "(J)V",                         (void *) StopDecode},
        {"nativeResumeDecode",              "(J)V",                         (void *) ResumeDecode},
        {"nativePauseDecode",               "(J)V",                         (void *) PauseDecode},
        {"nativeSeekDecode",                "(JI)V",                        (void *) SeekDecode},
};

static jint registerNativeDecodeMethods(JNIEnv *env) {
    jclass clazz = env->FindClass("com/yj/player/jni/DecodeHelper");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gDecodeMethods,
                             sizeof(gDecodeMethods) / sizeof(gDecodeMethods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_OK;
}

#endif //BBQVIDEO_DECODE_JNI_H
