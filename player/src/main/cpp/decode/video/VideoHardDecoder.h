//
// Created by jian.yeung on 2020/9/13.
//

#ifndef BBQVIDEO_VIDEOHARDDECODER_H
#define BBQVIDEO_VIDEOHARDDECODER_H

#include <media/NdkMediaExtractor.h>
#include <Handler.h>
#include <BaseFilter.h>
#include <DLog.h>
#include "VideoDecoder.h"
#include "BaseHardDecoder.h"

#define VIDEO_HARD_DECODER_TAG "VideoHardDecoder"

class VideoDecodeHandler : public Handler {
public:
    VideoDecodeHandler() : Handler() {
        DLOGI(VIDEO_HARD_DECODER_TAG, "~~~VideoDecodeHandler::VideoDecodeHandler()~~~\n");
        my_looper_ = new Looper();
    }

    ~VideoDecodeHandler() {
        DLOGI(VIDEO_HARD_DECODER_TAG, "~~~VideoDecodeHandler::~VideoDecodeHandler()~~~\n");
        if (my_looper_ != nullptr) {
            delete my_looper_;
            my_looper_ = nullptr;
        }
    }

    void handleMessage(Message &msg) override;
};

class VideoHardDecoder : public VideoDecoder, public BaseHardDecoder {
private:
    AMediaExtractor *extractor;
    AMediaCodec *codec;
    AMediaFormat *format;
    int outFormat;
    int outWidth;
    int outHeight;
    long outDuration;

    VideoDecodeHandler *decodeHandler;
    bool isCodecIniting = false;
    bool isCodecReady = false;
    bool isCodecRelease = false;
    bool isPlaying = false;
    bool sawInputEOS = false;
    bool sawOutputEOS = false;
    int64_t renderStart;

public:
    VideoHardDecoder();
    ~VideoHardDecoder();

    void setSurface(ANativeWindow *nativeWindow, int width, int height) override;
    void setRender(GLRender *render) override;
    void setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) override;
    void setPreparedStatusListener(PreparedStatusListener *preparedStatusListener) override;
    void setErrorStatusListener(ErrorStatusListener *errorStatusListener) override;
    void setDataSource(std::string url) override;
    void prepare() override;
    void start() override;
    void pause() override;
    void resume() override;
    void seek(int position) override;
    void stop() override;
    void release() override;

    bool initCodec();
    void releaseCodec();
    void restartCodec();
    void doDecodeWork();
    void doSeekWork(int position);
    void doResumeWork();
    void doPauseWork();

    void sendMessage(int what, bool flush = false);
    void sendMessage(Message &msg, bool flush = false);
};


#endif //BBQVIDEO_VIDEOHARDDECODER_H
