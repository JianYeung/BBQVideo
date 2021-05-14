//
// Created by jian.yeung on 2020/12/29.
//

#include "VideoPlayerStatusCallback.h"

VideoPlayerStatusCallback::VideoPlayerStatusCallback(JavaVM *vm, JNIEnv *env, jobject jobj_callback) {
    if (DebugEnable && VIDEO_PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG, "~~~VideoPlayerStatusCallback::VideoPlayerStatusCallback()~~~\n");
    }
    this->javaVm = vm;
    this->jenv = env;
    this->jobj = jobj_callback;
    this->jobj = env->NewGlobalRef(jobj);
    jclass jclz = jenv->GetObjectClass(jobj);
    if (!jclz) {
        DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "get object class wrong");
    }
    this->jmid_prepared = jenv->GetMethodID(jclz, "onPrepared", "()V");
    this->jmid_progress = jenv->GetMethodID(jclz, "onProgress", "(J)V");
    this->jmid_error = jenv->GetMethodID(jclz, "onError", "(I)V");
    playerStatusCallbackHandler = new VideoPlayerStatusCallbackHandler();
}

VideoPlayerStatusCallback::~VideoPlayerStatusCallback() {
    if (DebugEnable && VIDEO_PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG, "~~~VideoPlayerStatusCallback::~VideoPlayerStatusCallback()~~~\n");
    }
}

void VideoPlayerStatusCallback::onPrepared() {
    if (DebugEnable && VIDEO_PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG, "~~~VideoPlayerStatusCallback::onPrepared()~~~\n");
    }
    if (playerStatusCallbackHandler != nullptr) {
        Message msg = Message();
        msg.what = kMsgPrepared;
        msg.obj = this;
        playerStatusCallbackHandler->sendMessage(msg);
    }
}

void VideoPlayerStatusCallback::onProgress(long presentationTime) {
    if (DebugEnable && VIDEO_PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG, "~~~VideoPlayerStatusCallback::onProgress()~~~\n");
    }
    if (playerStatusCallbackHandler != nullptr) {
        Message msg = Message();
        msg.what = kMsgProgress;
        msg.arg3 = presentationTime;
        msg.obj = this;
        playerStatusCallbackHandler->sendMessage(msg);
    }
}

void VideoPlayerStatusCallback::onError(int errorCode) {
    if (DebugEnable && VIDEO_PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG, "~~~VideoPlayerStatusCallback::onError()~~~\n");
        DFLOGD(VIDEO_PLAY_STATUS_CALLBACK_TAG, "VideoPlayerStatusCallback:onError() error code = 0x%04x",
               errorCode);
    }

    if (playerStatusCallbackHandler != nullptr) {
        Message msg = Message();
        msg.what = kMsgError;
        msg.arg1 = errorCode;
        msg.obj = this;
        playerStatusCallbackHandler->sendMessage(msg);
    }
}

void VideoPlayerStatusCallback::release() {
    if (DebugEnable && VIDEO_PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG, "~~~VideoPlayerStatusCallback::release()~~~\n");
    }
    if (playerStatusCallbackHandler != nullptr) {
        playerStatusCallbackHandler->quit();
        delete playerStatusCallbackHandler;
        playerStatusCallbackHandler = nullptr;
    }
    if (jenv != nullptr && jobj != nullptr) {
        jenv->DeleteGlobalRef(jobj);
    }
}

void VideoPlayerStatusCallbackHandler::handleMessage(Message &msg) {
    if (DebugEnable && VIDEO_PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG, "~~~~VideoPlayStatusCallbackHandler::handleMessage()~~~\n");
    }

    int what = msg.what;
    if (DebugEnable && VIDEO_PLAY_STATUS_CALLBACK_DEBUG) {
        DFLOGD(VIDEO_PLAY_STATUS_CALLBACK_TAG, "handleMessage msg what = %d", msg.what);
    }
    switch (what) {
        case kMsgPrepared: {
            auto playStatusCallback = (VideoPlayerStatusCallback *) msg.obj;
            if (playStatusCallback == nullptr) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "jobject is null");
                return;
            }

            if (playStatusCallback->jmid_prepared == nullptr) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "prepared method id is null");
                return;
            }

            JavaVM *jvm = playStatusCallback->javaVm;
            JNIEnv *jniEnv;
            if (jvm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "get thread jni env wrong");
                return;
            }
            jniEnv->CallVoidMethod(playStatusCallback->jobj, playStatusCallback->jmid_prepared);
            jvm->DetachCurrentThread();
        }
            break;
        case kMsgProgress: {
            long presentationTime = msg.arg3;
            auto playStatusCallback = (VideoPlayerStatusCallback *) msg.obj;
            if (playStatusCallback == nullptr) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "jobject is null");
                return;
            }

            if (playStatusCallback->jmid_progress == nullptr) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "progress method id is null");
                return;
            }

            JavaVM *jvm = playStatusCallback->javaVm;
            JNIEnv *jniEnv;
            if (jvm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "get thread jni env wrong");
                return;
            }
            jniEnv->CallVoidMethod(playStatusCallback->jobj, playStatusCallback->jmid_progress,
                                   presentationTime);
            jvm->DetachCurrentThread();
        }
            break;
        case kMsgError: {
            int errorCode = msg.arg1;
            auto playStatusCallback = (VideoPlayerStatusCallback *) msg.obj;
            if (playStatusCallback == nullptr) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "jobject is null");
                return;
            }

            if (playStatusCallback->jmid_error == nullptr) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "error method id is null");
                return;
            }
            JavaVM *jvm = playStatusCallback->javaVm;
            JNIEnv *jniEnv;
            if (jvm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
                DLOGE(VIDEO_PLAY_STATUS_CALLBACK_TAG, "get thread jni env wrong");
                return;
            }
            jniEnv->CallVoidMethod(playStatusCallback->jobj, playStatusCallback->jmid_error,
                                   errorCode);
            jvm->DetachCurrentThread();
        }
        default:
            break;
    }
}