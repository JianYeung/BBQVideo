//
// Created by jian.yeung on 2021/1/19.
//

#include "logging_macros.h"
#include "AudioDataSource.h"

void AudioDataSource::updateData(std::unique_ptr<float[]> data, size_t size) {
    LOGI("AudioDataSource::updateData()");
    std::unique_lock<std::mutex> lock(data_mutex_);
    mBuffer = std::move(data);
    mBufferSize = size;
    mIsPlaying = true;
    resetPlayHead();
}

void AudioDataSource::renderAudio(float *audioData, int32_t numFrames) {
    LOGI("AudioDataSource::renderAudio()");
    LOGD("AudioDataSource::renderAudio mReadFrameIndex = %d, numFrames = %d", mReadFrameIndex, numFrames);
    LOGD("AudioDataSource::renderAudio mRequestRender = %d", mIsPlaying.load());
    if (mIsPlaying) {

        int64_t framesToRenderFromData = numFrames;
        int64_t totalSourceFrames = mBufferSize / mChannelCount;

        LOGD("AudioDataSource::renderAudio mReadFrameIndex = %d, numFrames = %d, totalSourceFrames = %d", mReadFrameIndex, numFrames, totalSourceFrames);

        // Check whether we're about to reach the end of the recording
        if (mReadFrameIndex + numFrames >= totalSourceFrames) {
            framesToRenderFromData = totalSourceFrames - mReadFrameIndex;
            mIsPlaying = false;
            LOGI("AudioDataSource::renderAudio() 4");
        }

        {
            std::unique_lock<std::mutex> lock(data_mutex_);
            memset(audioData, 0, sizeof(float) * numFrames * mChannelCount);
            for (int i = 0; i < framesToRenderFromData; ++i) {
                for (int j = 0; j < mChannelCount; ++j) {
                    audioData[(i * mChannelCount) + j] = mBuffer[(mReadFrameIndex * mChannelCount) + j];
                }

                // Increment and handle wraparound
                if (++mReadFrameIndex >= totalSourceFrames) {
                    mReadFrameIndex = 0;
                    LOGI("AudioDataSource::renderAudio() 5");
                    LOGD("AudioDataSource::renderAudio 5 mReadFrameIndex = %d", mReadFrameIndex);
                }
            }
        }
        numFrames -= framesToRenderFromData;
//        if (framesToRenderFromData < numFrames) {
//            // fill the rest of the buffer with silence
//            renderSilence(&audioData[framesToRenderFromData], numFrames * mChannelCount);
//        }
    } else {
        renderSilence(audioData, numFrames * mChannelCount);
        LOGI("AudioDataSource::renderAudio() 3");
    }
}

void AudioDataSource::renderSilence(float *start, int32_t numSamples){
    LOGI("AudioDataSource::renderSilence()");
    for (int i = 0; i < numSamples; ++i) {
        start[i] = 0;
    }
}

void AudioDataSource::release() {
    LOGI("AudioDataSource::release()");
    if (mBuffer != nullptr) {
        mBuffer.reset();
        mBufferSize = 0;
        mChannelCount = 0;
        mIsPlaying = false;
    }
}