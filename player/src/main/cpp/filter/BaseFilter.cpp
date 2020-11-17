//
// Created by jian.yeung on 2020/9/17.
//

#include "BaseFilter.h"

BaseFilter::BaseFilter() : program(0), mPositionHandle(0), mColorHandle(0),
                           vao(0), vbo(0), ebo(0) {

}

BaseFilter::~BaseFilter() {

}

void BaseFilter::setUp() {

}

void BaseFilter::tearDown() {

}

void BaseFilter::onSurfaceCreated(ANativeWindow *nativeWindow) {

}

void BaseFilter::onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) {

}

void BaseFilter::updatePreviewFrame(unsigned char *data, int format, int width, int height) {

}

void BaseFilter::draw() {

}

void BaseFilter::onDestroy() {

}