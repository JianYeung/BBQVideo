//
// Created by jian.yeung on 2020/9/17.
//

#include "BaseFilter.h"

BaseFilter::BaseFilter() {

}

BaseFilter::~BaseFilter() {

}

void BaseFilter::init() {

}

void BaseFilter::unInit() {

}

void BaseFilter::onSurfaceCreated(ANativeWindow *nativeWindow) {

}

void BaseFilter::onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) {
    this->format = format;
    this->width = width;
    this->height = height;
}

void BaseFilter::updateMVPMatrix() {

}

void BaseFilter::draw() {

}

void BaseFilter::onDestroy() {

}