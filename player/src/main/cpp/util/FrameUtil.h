//
// Created by jian.yeung on 2020/12/6.
//

#ifndef BBQVIDEO_FRAMEUTIL_H
#define BBQVIDEO_FRAMEUTIL_H


#include <VideoFrame.h>

static bool isValidVideoFrame(VideoFrame *frame) {
    return frame != nullptr && frame->getData() != nullptr
           && frame->getWidth() > 0 && frame->getHeight() > 0;
}

#endif //BBQVIDEO_FRAMEUTIL_H
