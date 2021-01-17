//
// Created by jian.yeung on 2020/9/14.
//

#include <DLog.h>
#include "VideoSoftDecoder.h"

VideoSoftDecoder::VideoSoftDecoder(): VideoDecoder() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::VideoSoftDecoder()~~~\n");
    }
}

VideoSoftDecoder::~VideoSoftDecoder() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::~VideoSoftDecoder()~~~\n");
    }
}

void VideoSoftDecoder::setSurface(ANativeWindow *nativeWindow, int width, int height) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::setSurface()~~~\n");
    }
}

void VideoSoftDecoder::setRender(GLRender *render)  {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::setRender()~~~\n");
    }
}

void VideoSoftDecoder::setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::setPlayerStatusCallback()~~~\n");
    }
}

void VideoSoftDecoder::setOnPreparedListener(OnPreparedListener *preparedStatusListener) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::setOnPreparedListener()~~~\n");
    }
}

void VideoSoftDecoder::setOnErrorListener(OnErrorListener *onErrorListener) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::setOnErrorListener()~~~\n");
    }
}

void VideoSoftDecoder::setDataSource(std::string url) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::setDataSource()~~~\n");
    }
}

void VideoSoftDecoder::prepare() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::prepare()~~~\n");
    }
}

void VideoSoftDecoder::start() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::start()~~~\n");
    }
}

void VideoSoftDecoder::pause() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::pause()~~~\n");
    }
}

void VideoSoftDecoder::resume() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::resume()~~~\n");
    }
}

void VideoSoftDecoder::seek(int position) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::seek()~~~\n");
        DFLOGD(VIDEO_SOFT_DECODER_TAG, "VideoSoftDecoder::seek() position = %d", position);
    }
}

void VideoSoftDecoder::stop() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::stop()~~~\n");
    }
}

void VideoSoftDecoder::release() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(VIDEO_SOFT_DECODER_TAG, "~~~VideoSoftDecoder::release()~~~\n");
    }
}