//
// Created by jian.yeung on 2020/9/14.
//

#include <DLog.h>
#include "VideoDecoder.h"

VideoDecoder::VideoDecoder() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_DECODER_TAG, "~~~~VideoDecoder::VideoDecoder()~~~\n");
    }
}

VideoDecoder::~VideoDecoder() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_DECODER_TAG, "~~~~VideoDecoder::~VideoDecoder()~~~\n");
    }
}

void VideoDecoder::setRender(GLRender *glRender)  {

}

void VideoDecoder::setDataSource(std::string url) {

}

void VideoDecoder::setSurface(ANativeWindow *window, int width, int height) {

}

void VideoDecoder::start() {

}

void VideoDecoder::resume() {

}

void VideoDecoder::pause() {

}

void VideoDecoder::seek(int position) {

}

void VideoDecoder::stop() {

}

void VideoDecoder::release() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_DECODER_TAG, "~~~~VideoDecoder::release()~~~\n");
    }
}