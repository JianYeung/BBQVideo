//
// Created by jian.yeung on 2020/9/13.
//

#ifndef BBQVIDEO_VIDEOHARDDECODER_H
#define BBQVIDEO_VIDEOHARDDECODER_H

#define HARD_DECODER_TAG "HardDecoder"

#include <media/NdkMediaExtractor.h>
#include <Handler.h>
#include <BaseFilter.h>
#include <DLog.h>
#include "VideoDecoder.h"

enum {
    kMsgCodecBuffer,
    kMsgPause,
    kMsgResume,
    kMsgPauseAck,
    kMsgSeek,
    kMsgCodecDone,
};

class DecodeHandler : public Handler {
private:
    const char *DECODER_HANDLER_TAG = "DecodeHandler";
public:
    DecodeHandler() : Handler() {
        DLOGI(DECODER_HANDLER_TAG, "~~~DecodeHandler::DecodeHandler()~~~\n");
        my_looper_ = new Looper();
    }

    ~DecodeHandler() {
        DLOGI(DECODER_HANDLER_TAG, "~~~DecodeHandler::~DecodeHandler()~~~\n");
        if (my_looper_ != nullptr) {
            delete my_looper_;
            my_looper_ = nullptr;
        }
    }

    void handleMessage(Message &msg) override;
};

class VideoHardDecoder : public VideoDecoder {
private:
    DecodeHandler *decodeHandler = nullptr;
    AMediaExtractor *extractor = nullptr;
    AMediaCodec *codec = nullptr;
    AMediaFormat *format = nullptr;
    int outFormat;
    int outWidth;
    int outHeight;
    long outDuration;

    bool isCodecIniting = false;
    bool isCodecReady = false;
    bool isCodecRelease = false;
    bool isPlaying = false;
    bool sawInputEOS = false;
    bool sawOutputEOS = false;
    int64_t renderStart;

    GLRender *glRender = nullptr;
    BaseFilter *filter = nullptr;

public:
    VideoHardDecoder();
    ~VideoHardDecoder();

    void setSurface(ANativeWindow *nativeWindow, int width, int height) override;
    void setRender(GLRender *glRender) override;
    void setPlayStatusCallback(PlayStatusCallback *playStatusListener) override;
    void setDataSource(std::string url) override;
    void prepare() override;
    void start() override;
    void resume() override;
    void pause() override;
    void seek(int position) override;
    void stop() override;
    void release() override;

    bool initCodec();
    void doDecodeWork();
    void doSeekWork(int position);
    void doResumeWork();
    void doPauseWork();
    void releaseCodec();
    void restartCodec();

    void sendMessage(int what, bool flush = false);
    void sendMessage(Message &msg, bool flush = false);
};


#endif //BBQVIDEO_VIDEOHARDDECODER_H
