//
// Created by jian.yeung on 2021/1/9.
//

#include <oboe/AudioStreamBuilder.h>
#include <memory>
#include <utility>
#include <trace.h>
#include "AudioEngine.h"

AudioEngine::AudioEngine() {
    LOGI("AudioEngine::AudioEngine()");
}

AudioEngine::~AudioEngine() {
    LOGI("AudioEngine::~AudioEngine()");
    if (mStream) {
        LOGE("MegaDroneEngine destructor was called without calling stop()."
             "Please call stop() to ensure stream resources are not leaked.");
        stop();
    }
}

AudioDataSource* AudioEngine::getAudioSource() {
    LOGI("AudioEngine::getAudioSource()");
    return mAudioSource.get();
}

void AudioEngine::restart() {
    LOGI("AudioEngine::restart()");
    stop();
    start();
}

Result AudioEngine::createPlaybackStream() {
    LOGI("AudioEngine::createPlaybackStream()");
    AudioStreamBuilder builder;
    return builder.setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setFormat(AudioFormat::Float)
            ->setDataCallback(mDataCallback.get())
            ->setErrorCallback(mErrorCallback.get())
            ->openStream(mStream);
}

void AudioEngine::createCallback(std::vector<int> cpuIds) {
    LOGI("AudioEngine::createCallback()");
    Trace::initialize();
    mDataCallback = std::make_unique<DefaultDataCallback>();

    // Create the error callback, we supply ourselves as the parent so that we can restart the stream
    // when it's disconnected
    mErrorCallback = std::make_unique<DefaultErrorCallback>(*this);

    // Bind the audio callback to specific CPU cores as this can help avoid underruns caused by
    // core migrations
    mDataCallback->setCpuIds(std::move(cpuIds));
    mDataCallback->setThreadAffinityEnabled(true);
}

bool AudioEngine::start() {
    LOGI("AudioEngine::start()");
    auto result = createPlaybackStream();
    if (result == Result::OK){
        // Create our synthesizer audio source using the properties of the stream
        LOGD("sampleRate = %d, channelCount = %d", mStream->getSampleRate(), mStream->getChannelCount());
        mAudioSource = std::make_shared<AudioDataSource>(mStream->getSampleRate(), mStream->getChannelCount());
        mDataCallback->reset();
        mDataCallback->setSource(std::dynamic_pointer_cast<IRenderableAudio>(mAudioSource));
        mStream->start();
        return true;
    } else {
        LOGE("Failed to create the playback stream. Error: %s", convertToText(result));
        return false;
    }
}

bool AudioEngine::pause() {
    LOGI("AudioEngine::pause()");
    if(mStream && mStream->getState() == oboe::StreamState::Started) {
        mStream->pause();
    }
    return true;
}

bool AudioEngine::stop() {
    LOGI("AudioEngine::stop()");
    if(mStream && mStream->getState() != oboe::StreamState::Closed) {
        mStream->stop();
        mStream->close();
    }
    mStream.reset();
    return true;
}

void AudioEngine::release() {
    LOGI("AudioEngine::release()");
    stop();
}
