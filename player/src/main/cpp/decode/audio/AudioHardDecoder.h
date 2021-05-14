//
// Created by jian.yeung on 2021/1/9.
//

#ifndef BBQVIDEO_AUDIOHARDDECODER_H
#define BBQVIDEO_AUDIOHARDDECODER_H

#include <media/NdkMediaExtractor.h>
#include <Handler.h>
#include <DLog.h>
#include "AudioDecoder.h"
#include "BaseHardDecoder.h"

#define AUDIO_HARD_DECODER_TAG "AudioHardDecoder"

class AudioDecodeHandler : public Handler {
public:
    AudioDecodeHandler() : Handler() {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioDecodeHandler::AudioDecodeHandler()~~~\n");
        my_looper_ = new Looper();
    }

    ~AudioDecodeHandler() {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioDecodeHandler::~AudioDecodeHandler()~~~\n");
        if (my_looper_ != nullptr) {
            delete my_looper_;
            my_looper_ = nullptr;
        }
    }

    void handleMessage(Message &msg) override;
};

class AudioHardDecoder : public AudioDecoder, public BaseHardDecoder {
private:
    AudioDecodeHandler *decodeHandler;
    AMediaExtractor *extractor;
    AMediaCodec *codec;
    AMediaFormat *format;
    std::mutex codec_mutex_;
    int audioTrackIndex = -1;
    bool sawInputEOS = false;
    bool sawOutputEOS = false;
    long outDuration = -1;
    int64_t renderStart = -1;
    bool playCompleted = false;

public:
    AudioHardDecoder();
    ~AudioHardDecoder();

    void setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) override;
    void setOnPreparedListener(OnPreparedListener *onPreparedListener) override;
    void setOnErrorListener(OnErrorListener *onErrorListener) override;
    void setDataSource(std::string url) override;
    void setCpuIds(std::vector<int> cpuIds) override;
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
    void resetCodec();
    void doDecodeWork();
    void doPauseWork();
    void doResumeWork();
    void doSeekWork(int position);
    void doResetWork();

    void sendMessage(int what, bool flush = false);
    void sendMessage(Message &msg, bool flush = false);
};


#endif //BBQVIDEO_AUDIOHARDDECODER_H
