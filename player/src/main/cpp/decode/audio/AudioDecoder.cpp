//
// Created by jian.yeung on 2021/1/9.
//

#include <DLog.h>
#include "AudioDecoder.h"

AudioDecoder::AudioDecoder() : stateMachine(nullptr), playerStatusCallback(nullptr),
                                onPreparedListener(nullptr), onErrorListener(nullptr),
                                audioEngine(nullptr) {
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

void AudioDecoder::setOnPreparedListener(OnPreparedListener *onPreparedListener) {

}

void AudioDecoder::setOnErrorListener(OnErrorListener *onErrorListener) {

}

void AudioDecoder::setDataSource(std::string url) {

}

void AudioDecoder::setCpuIds(std::vector<int> cpuIds) {

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

