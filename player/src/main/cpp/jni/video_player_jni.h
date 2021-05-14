//
// Created by jian.yeung on 2020/9/8.
//

#ifndef BBQVIDEO_VIDEO_PLAYER_JNI_H
#define BBQVIDEO_VIDEO_PLAYER_JNI_H

#include <VideoPlayer.h>
#include <VideoPlayerStatusCallback.h>
#include <GLRender.h>

const bool VIDEO_PLAYER_JNI_DEBUG = false;
const char *VIDEO_PLAYER_JNI_TAG = "VideoPlayerJni";

jlong CreateVideoPlayerHandle(JNIEnv *env, jobject thiz, jboolean software_decode_enable) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGD(VIDEO_PLAYER_JNI_TAG, "CreateVideoPlayerHandle() software decode enable = %d",
               software_decode_enable);
    }

    VideoPlayer *videoPlayer = new VideoPlayer(software_decode_enable);

    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "CreateVideoPlayerHandle() handle = %ld", (long) videoPlayer);
    }
    return (long) videoPlayer;
}

void ReleaseVideoPlayerHandle(JNIEnv *env, jobject thiz, jlong native_player_handle) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "ReleaseVideoPlayerHandle() handle = %ld",
               (long) native_player_handle);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        if (videoPlayer != nullptr) {
            videoPlayer->release();
            delete videoPlayer;
            videoPlayer = nullptr;
        }
    }
}

void SetSurface(JNIEnv *env, jobject thiz, jlong native_player_handle, jobject surface, jint width,
                jint height) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "SetSurface() handle = %ld", (long) native_player_handle);
        DFLOGD(VIDEO_PLAYER_JNI_TAG, "SetSurface() width = %d, height = %d", width, height);
    }
    if (native_player_handle != 0 && surface != nullptr) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
        videoPlayer->setSurface(nativeWindow, width, height);
    }
}

void SetRender(JNIEnv *env, jobject thiz, jlong native_player_handle, jlong native_render_handle) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "SetRender() handle = %ld", (long) native_player_handle);
    }
    if (native_player_handle != 0 && native_render_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        GLRender *glRender = reinterpret_cast<GLRender *>(native_render_handle);
        videoPlayer->setRender(glRender);
    }
}

void SetPlayerStatusCallback(JNIEnv *env, jobject thiz, jlong native_player_handle,
                             jobject jobj_play_status_callback) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "SetPlayerStatusCallback() handle = %ld",
               (long) native_player_handle);
    }
    if (native_player_handle != 0 && jobj_play_status_callback != nullptr) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        auto playStatusCallback = new VideoPlayerStatusCallback(javaVm, env,
                                                                jobj_play_status_callback);
        videoPlayer->setPlayerStatusCallback(playStatusCallback);
    }
}

void SetDataSource(JNIEnv *env, jobject thiz, jlong native_player_handle, jstring j_url) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "SetDataSource() handle = %ld", (long) native_player_handle);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        const char *c_url = env->GetStringUTFChars(j_url, nullptr);
        std::string url = std::string(c_url);
        videoPlayer->setDataSource(url);
        if (VIDEO_PLAYER_JNI_DEBUG) {
            DFLOGD(VIDEO_PLAYER_JNI_TAG, "SetDataSource() url = %s", url.c_str());
        }
        env->ReleaseStringUTFChars(j_url, c_url);
    }
}

void SetCpuIds(JNIEnv *env, jobject thiz, jlong native_player_handle, jintArray jCpuIds) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "SetDataSource() handle = %ld", (long) native_player_handle);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        std::vector<int> cpuIds = convertJavaArrayToVector(env, jCpuIds);
        videoPlayer->setCpuIds(std::move(cpuIds));
    }
}

void Prepare(JNIEnv *env, jobject thiz, jlong native_player_handle) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "Prepare() handle = %ld", (long) native_player_handle);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        videoPlayer->prepare();
    }
}

void Start(JNIEnv *env, jobject thiz, jlong native_player_handle) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "Start() handle = %ld", (long) native_player_handle);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        videoPlayer->start();
    }
}

void Pause(JNIEnv *env, jobject thiz, jlong native_player_handle) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "Pause() handle = %ld", (long) native_player_handle);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        videoPlayer->pause();
    }
}

void Resume(JNIEnv *env, jobject thiz, jlong native_player_handle) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "Resume() handle = %ld", (long) native_player_handle);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        videoPlayer->resume();
    }
}

void Seek(JNIEnv *env, jobject thiz, jlong native_player_handle, jint position) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "Seek() handle = %ld", (long) native_player_handle);
        DFLOGD(VIDEO_PLAYER_JNI_TAG, "Seek() position = %d", position);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        videoPlayer->seek(position);
    }
}

void Stop(JNIEnv *env, jobject thiz, jlong native_player_handle) {
    if (VIDEO_PLAYER_JNI_DEBUG) {
        DFLOGI(VIDEO_PLAYER_JNI_TAG, "Stop() handle = %ld", (long) native_player_handle);
    }
    if (native_player_handle != 0) {
        VideoPlayer *videoPlayer = reinterpret_cast<VideoPlayer *>(native_player_handle);
        videoPlayer->stop();
    }
}

static JNINativeMethod gVideoPlayerMethods[] = {
        {"nativeCreateVideoPlayerHandle",  "(Z)J",                                                              (void *) CreateVideoPlayerHandle},
        {"nativeReleaseVideoPlayerHandle", "(J)V",                                                              (void *) ReleaseVideoPlayerHandle},
        {"nativeSetSurface",               "(JLandroid/view/Surface;II)V",                                      (void *) SetSurface},
        {"nativeSetRender",                "(JJ)V",                                                             (void *) SetRender},
        {"nativeSetPlayerStatusCallback",  "(JLcom/yj/player/videoPlayer/NativeVideoPlayerStatusCallback;)V",   (void *) SetPlayerStatusCallback},
        {"nativeSetDataSource",            "(JLjava/lang/String;)V",                                            (void *) SetDataSource},
        {"nativeSetCpuIds",                "(J[I)V",                                                            (void *) SetCpuIds},
        {"nativePrepare",                  "(J)V",                                                              (void *) Prepare},
        {"nativeStart",                    "(J)V",                                                              (void *) Start},
        {"nativeResume",                   "(J)V",                                                              (void *) Resume},
        {"nativePause",                    "(J)V",                                                              (void *) Pause},
        {"nativeSeek",                     "(JI)V",                                                             (void *) Seek},
        {"nativeStop",                     "(J)V",                                                              (void *) Stop},
};

static jint registerNativeVideoPlayerMethods(JNIEnv *env) {
    jclass clazz = env->FindClass("com/yj/player/jni/VideoPlayerHelper");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gVideoPlayerMethods,
                             sizeof(gVideoPlayerMethods) / sizeof(gVideoPlayerMethods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_OK;
}

#endif //BBQVIDEO_VIDEO_PLAYER_JNI_H
