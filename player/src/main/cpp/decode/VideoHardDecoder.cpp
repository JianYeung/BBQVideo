//
// Created by jian.yeung on 2020/9/13.
//

#include "VideoHardDecoder.h"

void VideoHardDecoder::setSource(std::string url) {
    this->url = url;

}

void VideoHardDecoder::setSurface(ANativeWindow *nativeWindow, const int width, const int height) {
    this->window = nativeWindow;
    this->width = width;
    this->height = height;
}

void VideoHardDecoder::release() {

}