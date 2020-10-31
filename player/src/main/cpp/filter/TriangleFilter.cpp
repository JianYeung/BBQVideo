//
// Created by jian.yeung on 2020/10/27.
//

#include <DLog.h>
#include <GLES3/gl3.h>
#include <ShaderUtil.h>
#include "TriangleFilter.h"

TriangleFilter::TriangleFilter() : BaseFilter() {

}

TriangleFilter::~TriangleFilter() {

}

void TriangleFilter::init() {

}

void TriangleFilter::unInit() {

}

void TriangleFilter::onSurfaceCreated(ANativeWindow *nativeWindow) {
    DLOGD(TRIANGLE_FILTER_TAG, "~~~Triangle Filter Render onSurfaceCreated~~~\n");
}

void TriangleFilter::onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) {
    DLOGD(TRIANGLE_FILTER_TAG, "~~~Triangle Filter Render onSurfaceChanged~~~\n");
    this->format = format;
    this->width = width;
    this->height = height;
}

void TriangleFilter::draw() {
    DLOGD(TRIANGLE_FILTER_TAG, "~~~Triangle Filter Render draw~~~\n");
}

void TriangleFilter::onDestroy() {

}