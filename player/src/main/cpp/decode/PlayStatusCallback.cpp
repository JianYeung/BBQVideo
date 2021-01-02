//
// Created by jian.yeung on 2020/12/29.
//

#include "PlayStatusCallback.h"

PlayStatusCallback::PlayStatusCallback(JavaVM *vm, JNIEnv *env, jobject jobj_callback) {
    if (DebugEnable && PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(PLAY_STATUS_CALLBACK_TAG, "~~~PlayStatusCallback::PlayStatusCallback()~~~\n");
    }
    this->javaVm = vm;
    this->jenv = env;
    this->jobj = jobj_callback;
    this->jobj = env->NewGlobalRef(jobj);
    jclass jclz = jenv->GetObjectClass(jobj);
    if (!jclz) {
        DLOGE(PLAY_STATUS_CALLBACK_TAG, "get object class wrong");
    }
    this->jmid_prepared = jenv->GetMethodID(jclz, "onPrepared", "(J)V");
    this->jmid_progress = jenv->GetMethodID(jclz, "onProgress", "(J)V");
    this->jmid_error = jenv->GetMethodID(jclz, "onError", "(I)V");
    playStatusCallbackHandler = new PlayStatusCallbackHandler();
}

PlayStatusCallback::~PlayStatusCallback() {
    if (DebugEnable && PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(PLAY_STATUS_CALLBACK_TAG, "~~~PlayStatusCallback::~PlayStatusCallback()~~~\n");
    }
}

void PlayStatusCallback::onPrepared(long duration) {
    if (DebugEnable && PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(PLAY_STATUS_CALLBACK_TAG, "~~~PlayStatusCallback::onPrepared()~~~\n");
        DFLOGD(PLAY_STATUS_CALLBACK_TAG, "PlayStatusCallback:onPrepared() duration = %ld",
               duration);
    }
    if (playStatusCallbackHandler != nullptr) {
        Message msg = Message();
        msg.what = kMsgPrepared;
        msg.arg3 = duration;
        msg.obj = this;
        playStatusCallbackHandler->sendMessage(msg);
    }
}

void PlayStatusCallback::onProgress(long presentationTime) {
    if (DebugEnable && PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(PLAY_STATUS_CALLBACK_TAG, "~~~PlayStatusCallback::onProgress()~~~\n");
    }
    if (playStatusCallbackHandler != nullptr) {
        Message msg = Message();
        msg.what = kMsgProgress;
        msg.arg3 = presentationTime;
        msg.obj = this;
        playStatusCallbackHandler->sendMessage(msg);
    }
}

void PlayStatusCallback::onError(int errorCode) {
    if (DebugEnable && PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(PLAY_STATUS_CALLBACK_TAG, "~~~PlayStatusCallback::onError()~~~\n");
        DFLOGD(PLAY_STATUS_CALLBACK_TAG, "PlayStatusCallback:onError() error code = 0x%04x",
               errorCode);
    }

    if (playStatusCallbackHandler != nullptr) {
        Message msg = Message();
        msg.what = kMsgError;
        msg.arg1 = errorCode;
        msg.obj = this;
        playStatusCallbackHandler->sendMessage(msg);
    }
}

void PlayStatusCallback::release() {
    if (DebugEnable && PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(PLAY_STATUS_CALLBACK_TAG, "~~~PlayStatusCallback::release()~~~\n");
    }
    if (playStatusCallbackHandler != nullptr) {
        playStatusCallbackHandler->quit();
        delete playStatusCallbackHandler;
        playStatusCallbackHandler = nullptr;
    }
    if (jenv != nullptr && jobj != nullptr) {
        jenv->DeleteGlobalRef(jobj);
    }
}

void PlayStatusCallbackHandler::handleMessage(Message &msg) {
    if (DebugEnable && PLAY_STATUS_CALLBACK_DEBUG) {
        DLOGI(PLAY_STATUS_CALLBACK_HANDLER_TAG,
              "~~~~PlayStatusCallbackHandler::handleMessage()~~~\n");
    }

    int what = msg.what;
    if (DebugEnable && PLAY_STATUS_CALLBACK_DEBUG) {
        DFLOGD(PLAY_STATUS_CALLBACK_TAG, "handleMessage msg what = %d", msg.what);
    }
    switch (what) {
        case kMsgPrepared: {
            long duration = msg.arg3;
            auto playStatusCallback = (PlayStatusCallback *) msg.obj;
            if (playStatusCallback == nullptr) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "jobject is null");
                return;
            }

            if (playStatusCallback->jmid_prepared == nullptr) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "prepared method id is null");
                return;
            }

            JavaVM *jvm = playStatusCallback->javaVm;
            JNIEnv *jniEnv;
            if (jvm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "get thread jni env wrong");
                return;
            }
            jniEnv->CallVoidMethod(playStatusCallback->jobj, playStatusCallback->jmid_prepared,
                                   duration);
            jvm->DetachCurrentThread();
        }
            break;
        case kMsgProgress: {
            long presentationTime = msg.arg3;
            auto playStatusCallback = (PlayStatusCallback *) msg.obj;
            if (playStatusCallback == nullptr) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "jobject is null");
                return;
            }

            if (playStatusCallback->jmid_progress == nullptr) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "progress method id is null");
                return;
            }

            JavaVM *jvm = playStatusCallback->javaVm;
            JNIEnv *jniEnv;
            if (jvm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "get thread jni env wrong");
                return;
            }
            jniEnv->CallVoidMethod(playStatusCallback->jobj, playStatusCallback->jmid_progress,
                                   presentationTime);
            jvm->DetachCurrentThread();
        }
            break;
        case kMsgError: {
            int errorCode = msg.arg1;
            auto playStatusCallback = (PlayStatusCallback *) msg.obj;
            if (playStatusCallback == nullptr) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "jobject is null");
                return;
            }

            if (playStatusCallback->jmid_error == nullptr) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "error method id is null");
                return;
            }
            JavaVM *jvm = playStatusCallback->javaVm;
            JNIEnv *jniEnv;
            if (jvm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
                DLOGE(PLAY_STATUS_CALLBACK_TAG, "get thread jni env wrong");
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