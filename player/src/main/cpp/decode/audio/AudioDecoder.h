//
// Created by jian.yeung on 2021/1/9.
//

#ifndef BBQVIDEO_AUDIODECODER_H
#define BBQVIDEO_AUDIODECODER_H

#include <DecodeStateMachine.h>
#include <string>
#include <VideoPlayerStatusCallback.h>
#include <OnPreparedListener.h>
#include <OnErrorListener.h>
#include <AudioEngine.h>

#define AUDIO_DECODER_DEBUG true
#define AUDIO_DECODER_TAG "AudioDecoder"

class AudioDecoder {
protected:
    DecodeStateMachine *stateMachine;
    std::string audioUrl;
    VideoPlayerStatusCallback *playerStatusCallback;
    OnPreparedListener *onPreparedListener;
    OnErrorListener *onErrorListener;
    AudioEngine *audioEngine;
    AudioDataSource* audioDataSource;

public:
    AudioDecoder();
    virtual ~AudioDecoder();

    virtual void setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) = 0;
    virtual void setOnPreparedListener(OnPreparedListener *onPreparedListener) = 0;
    virtual void setOnErrorListener(OnErrorListener *onErrorListener) = 0;
    virtual void setDataSource(std::string url) = 0;
    virtual void setCpuIds(std::vector<int> cpuIds) = 0;
    virtual void prepare() = 0;
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void seek(int position) = 0;
    virtual void stop() = 0;
    virtual void release() = 0;
};


#endif //BBQVIDEO_AUDIODECODER_H
