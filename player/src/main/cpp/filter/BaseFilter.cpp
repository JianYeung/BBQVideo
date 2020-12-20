//
// Created by jian.yeung on 2020/9/17.
//

#include <DLog.h>
#include "BaseFilter.h"

BaseFilter::BaseFilter() : program(0), mPositionHandle(0), mColorHandle(0),
                           vao(0), vbo(0), ebo(0) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(BASE_FILTER_TAG, "~~~~BaseFilter::BaseFilter()~~~\n");
    }
}

BaseFilter::~BaseFilter() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(BASE_FILTER_TAG, "~~~~BaseFilter::~BaseFilter()~~~\n");
    }
}

void BaseFilter::setUp() {

}

void BaseFilter::tearDown() {

}

void BaseFilter::onSurfaceCreated(ANativeWindow *nativeWindow) {

}

void BaseFilter::onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) {

}

void BaseFilter::updatePreviewFrame(VideoFrame *videoFrame) {

}

void BaseFilter::draw() {

}

void BaseFilter::onDestroy() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(BASE_FILTER_TAG, "~~~~BaseFilter::onDestroy()~~~\n");
    }
}