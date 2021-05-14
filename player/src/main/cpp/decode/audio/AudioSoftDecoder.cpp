//
// Created by jian.yeung on 2021/1/9.
//

#include <DLog.h>
#include "AudioSoftDecoder.h"

AudioSoftDecoder::AudioSoftDecoder() : AudioDecoder() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::AudioSoftDecoder()~~~\n");
    }
}

AudioSoftDecoder::~AudioSoftDecoder() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::~AudioSoftDecoder()~~~\n");
    }
}

void AudioSoftDecoder::setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::setPlayerStatusCallback()~~~\n");
    }
}

void AudioSoftDecoder::setOnPreparedListener(OnPreparedListener *onPreparedListener) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::setOnPreparedListener()~~~\n");
    }
}

void AudioSoftDecoder::setOnErrorListener(OnErrorListener *onErrorListener) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::setOnErrorListener()~~~\n");
    }
}

void AudioSoftDecoder::setDataSource(std::string url) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::setDataSource()~~~\n");
    }
}

void AudioSoftDecoder::setCpuIds(std::vector<int> cpuIds) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::setCpuIds()~~~\n");
    }
}

void AudioSoftDecoder::prepare() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::prepare()~~~\n");
    }
}

void AudioSoftDecoder::start() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::start()~~~\n");
    }
}

void AudioSoftDecoder::pause() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::pause()~~~\n");
    }
}

void AudioSoftDecoder::resume() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::resume()~~~\n");
    }
}

void AudioSoftDecoder::seek(int position) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::seek()~~~\n");
        DFLOGD(AUDIO_SOFT_DECODER_TAG, "AudioSoftDecoder::seek() position = %d", position);
    }
}

void AudioSoftDecoder::stop() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::stop()~~~\n");
    }
}

void AudioSoftDecoder::release() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::release()~~~\n");
    }
}