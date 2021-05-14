//
// Created by jian.yeung on 2020/12/29.
//

#ifndef BBQVIDEO_VIDEOPLAYERSTATUSCALLBACK_H
#define BBQVIDEO_VIDEOPLAYERSTATUSCALLBACK_H

#include <jni.h>
#include <DLog.h>
#include <Handler.h>

#define VIDEO_PLAY_STATUS_CALLBACK_DEBUG true
#define VIDEO_PLAY_STATUS_CALLBACK_TAG "VideoPlayerStatusCallback"

enum {
    kMsgPrepared,
    kMsgProgress,
    kMsgError,
};

class VideoPlayerStatusCallbackHandler : public Handler {
public:
    VideoPlayerStatusCallbackHandler() : Handler() {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG,
              "~~~VideoPlayerStatusCallbackHandler::VideoPlayerStatusCallbackHandler()~~~\n");
        my_looper_ = new Looper();
    }

    ~VideoPlayerStatusCallbackHandler() {
        DLOGI(VIDEO_PLAY_STATUS_CALLBACK_TAG,
              "~~~VideoPlayerStatusCallbackHandler::~VideoPlayerStatusCallbackHandler()~~~\n");
        if (my_looper_ != nullptr) {
            delete my_looper_;
            my_looper_ = nullptr;
        }
    }

    void handleMessage(Message &msg) override;
};

class VideoPlayerStatusCallback {
private:


public:
    JavaVM *javaVm;
    JNIEnv *jenv;
    jobject jobj;
    jmethodID jmid_prepared;
    jmethodID jmid_progress;
    jmethodID jmid_error;
    VideoPlayerStatusCallbackHandler *playerStatusCallbackHandler = nullptr;

public:
    VideoPlayerStatusCallback(JavaVM *vm, JNIEnv *env, jobject jobj_callback);

    ~VideoPlayerStatusCallback();

    void onPrepared();

    void onProgress(long presentationTime);

    void onError(int errorCode);

    void release();
};


#endif //BBQVIDEO_VIDEOPLAYERSTATUSCALLBACK_H
