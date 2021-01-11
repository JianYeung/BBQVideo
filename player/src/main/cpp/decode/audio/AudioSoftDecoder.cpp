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

void AudioSoftDecoder::setPreparedStatusListener(PreparedStatusListener *preparedStatusListener) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::setPreparedStatusListener()~~~\n");
    }
}

void AudioSoftDecoder::setErrorStatusListener(ErrorStatusListener *errorStatusListener) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::setErrorStatusListener()~~~\n");
    }
}

void AudioSoftDecoder::setDataSource(std::string url) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_SOFT_DECODER_TAG, "~~~AudioSoftDecoder::setDataSource()~~~\n");
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