//
// Created by jian.yeung on 2021/1/9.
//

#ifndef BBQVIDEO_AUDIOENGINE_H
#define BBQVIDEO_AUDIOENGINE_H

#include <oboe/Oboe.h>
#include <oboe/AudioStreamCallback.h>
#include "DefaultDataCallback.h"
#include "DefaultErrorCallback.h"
#include "AudioDataSource.h"

using namespace oboe;

class AudioEngine : public IRestartable{
public:
    AudioEngine();
    ~AudioEngine();

    virtual void restart() override;
    void createCallback(std::vector<int> cpuIds);
    bool start();
    bool pause();
    bool stop();
    void release();

    AudioDataSource* getAudioSource();

private:
    std::shared_ptr<AudioStream> mStream;
    std::shared_ptr<AudioDataSource> mAudioSource;
    std::unique_ptr<DefaultDataCallback> mDataCallback;
    std::unique_ptr<DefaultErrorCallback> mErrorCallback;

    oboe::Result createPlaybackStream();
};


#endif //BBQVIDEO_AUDIOENGINE_H
