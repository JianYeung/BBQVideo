//
// Created by jian.yeung on 2021/1/9.
//

#ifndef BBQVIDEO_AUDIOSOFTDECODER_H
#define BBQVIDEO_AUDIOSOFTDECODER_H


#include "AudioDecoder.h"

#define AUDIO_SOFT_DECODER_TAG "AudioSoftDecoder"

class AudioSoftDecoder : public AudioDecoder {

public:
    AudioSoftDecoder();
    ~AudioSoftDecoder();

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
};


#endif //BBQVIDEO_AUDIOSOFTDECODER_H
