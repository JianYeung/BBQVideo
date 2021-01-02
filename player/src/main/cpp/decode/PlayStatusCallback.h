//
// Created by jian.yeung on 2020/12/29.
//

#ifndef BBQVIDEO_PLAYSTATUSCALLBACK_H
#define BBQVIDEO_PLAYSTATUSCALLBACK_H

#include <jni.h>
#include <DLog.h>
#include <Handler.h>

#define PLAY_STATUS_CALLBACK_DEBUG true
#define PLAY_STATUS_CALLBACK_TAG "PlayStatusCallback"

#define PLAYER_CAN_NOT_OPEN_URL              0x1001
#define PLAYER_IS_INITING                    0x1002

enum {
    kMsgPrepared,
    kMsgProgress,
    kMsgError,
};

class PlayStatusCallbackHandler : public Handler {
private:
    const char *PLAY_STATUS_CALLBACK_HANDLER_TAG = "PlayStatusCallbackHandler";
public:
    PlayStatusCallbackHandler() : Handler() {
        DLOGI(PLAY_STATUS_CALLBACK_HANDLER_TAG, "~~~PlayStatusCallbackHandler::PlayStatusCallbackHandler()~~~\n");
        my_looper_ = new Looper();
    }

    ~PlayStatusCallbackHandler() {
        DLOGI(PLAY_STATUS_CALLBACK_HANDLER_TAG, "~~~PlayStatusCallbackHandler::~PlayStatusCallbackHandler()~~~\n");
        if (my_looper_ != nullptr) {
            delete my_looper_;
            my_looper_ = nullptr;
        }
    }

    void handleMessage(Message &msg) override;
};

class PlayStatusCallback {
public:
    JavaVM *javaVm;
    JNIEnv *jenv;
    jobject jobj;
    jmethodID jmid_prepared;
    jmethodID jmid_progress;
    jmethodID jmid_error;
    PlayStatusCallbackHandler *playStatusCallbackHandler = nullptr;

public:
    PlayStatusCallback(JavaVM *vm, JNIEnv *env, jobject jobj_callback);

    ~PlayStatusCallback();

    void onPrepared(long duration);

    void onProgress(long presentationTime);

    void onError(int errorCode);

    void release();
};


#endif //BBQVIDEO_PLAYSTATUSCALLBACK_H
