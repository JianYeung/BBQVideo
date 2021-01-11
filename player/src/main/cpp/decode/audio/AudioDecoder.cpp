//
// Created by jian.yeung on 2021/1/9.
//

#include <DLog.h>
#include "AudioDecoder.h"

AudioDecoder::AudioDecoder() : playerStatusCallback(nullptr), preparedStatusListener(nullptr),
                               errorStatusListener(nullptr) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_DECODER_TAG, "~~~AudioDecoder::AudioDecoder()~~~\n");
    }
}

AudioDecoder::~AudioDecoder() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_DECODER_TAG, "~~~AudioDecoder::~AudioDecoder()~~~\n");
    }
}

void AudioDecoder::setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) {

}

void AudioDecoder::setPreparedStatusListener(PreparedStatusListener *preparedStatusListener) {

}

void AudioDecoder::setErrorStatusListener(ErrorStatusListener *errorStatusListener) {

}

void AudioDecoder::setDataSource(std::string url) {

}

void AudioDecoder::prepare() {

}

void AudioDecoder::start() {

}

void AudioDecoder::pause() {

}

void AudioDecoder::resume() {

}

void AudioDecoder::seek(int position) {

}

void AudioDecoder::stop() {

}

void AudioDecoder::release() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_DECODER_TAG, "~~~AudioDecoder::release()~~~\n");
    }
}

