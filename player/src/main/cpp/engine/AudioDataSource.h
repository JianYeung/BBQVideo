//
// Created by jian.yeung on 2021/1/19.
//

#ifndef BBQVIDEO_AUDIODATASOURCE_H
#define BBQVIDEO_AUDIODATASOURCE_H

#include <mutex>
#include "IRenderableAudio.h"

class AudioDataSource : public IRenderableAudio {
public:
    AudioDataSource(int32_t sampleRate, int32_t channelCount) :
            mSampleRate(sampleRate), mChannelCount(channelCount) {}

    void updateData(std::unique_ptr<float[]> data, size_t size);

    void renderAudio(float *audioData, int32_t numFrames) override;

    void renderSilence(float*, int32_t);

    void release();

    void resetPlayHead() { mReadFrameIndex = 0; };
    void setPlaying(bool isPlaying) { mIsPlaying = isPlaying; resetPlayHead(); };
    void setLooping(bool isLooping) { mIsLooping = isLooping; };

private:
    int32_t mReadFrameIndex = 0;
    std::atomic<bool> mIsPlaying { false };
    std::atomic<bool> mIsLooping { false };
    std::unique_ptr<float[]> mBuffer;
    std::mutex data_mutex_;
    int64_t mBufferSize;
    int32_t mSampleRate;
    int32_t mChannelCount;
};


#endif //BBQVIDEO_AUDIODATASOURCE_H
