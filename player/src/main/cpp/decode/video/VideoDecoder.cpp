//
// Created by jian.yeung on 2020/9/14.
//

#include <DLog.h>
#include "VideoDecoder.h"

VideoDecoder::VideoDecoder() : surfaceWindow(nullptr), playerStatusCallback(nullptr),
                               onPreparedListener(nullptr), onErrorListener(nullptr),
                               render(nullptr), filter(nullptr) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_DECODER_TAG, "~~~VideoDecoder::VideoDecoder()~~~\n");
    }
}

VideoDecoder::~VideoDecoder() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_DECODER_TAG, "~~~VideoDecoder::~VideoDecoder()~~~\n");
    }
}

void VideoDecoder::setSurface(ANativeWindow *window, int width, int height) {

}

void VideoDecoder::setRender(GLRender *render) {

}

void VideoDecoder::setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) {

}

void VideoDecoder::setOnPreparedListener(OnPreparedListener *onPreparedListener) {

}

void VideoDecoder::setOnErrorListener(OnErrorListener *onErrorListener) {

}

void VideoDecoder::setDataSource(std::string url) {

}

void VideoDecoder::prepare() {

}

void VideoDecoder::start() {

}

void VideoDecoder::pause() {

}

void VideoDecoder::resume() {

}

void VideoDecoder::seek(int position) {

}

void VideoDecoder::stop() {

}

void VideoDecoder::release() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_DECODER_TAG, "~~~VideoDecoder::release()~~~\n");
    }
}