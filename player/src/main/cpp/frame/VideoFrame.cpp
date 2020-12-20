//
// Created by jian.yeung on 2020/12/6.
//

#include "VideoFrame.h"

VideoFrame::VideoFrame() : data(nullptr) {

}

VideoFrame::~VideoFrame() {

}

void VideoFrame::updateFrameInfo(uint8_t *data_, int format_, int width_, int height_) {
    this->data = data_;
    this->format = format_;
    this->width = width_;
    this->height = height_;
}

uint8_t *VideoFrame::getData() {
    return this->data;
}

int VideoFrame::getFormat() {
    return this->format;
}

int VideoFrame::getWidth() {
    return this->width;
}

int VideoFrame::getHeight() {
    return this->height;
}

