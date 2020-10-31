//
// Created by jian.yeung on 2020/9/14.
//

#ifndef BBQVIDEO_VIDEODECODER_H
#define BBQVIDEO_VIDEODECODER_H

#include <string>
#include <media/NdkMediaCodec.h>

class VideoDecoder {
protected:
    std::string url;
    ANativeWindow *window;
    int width;
    int height;

public:
    virtual void setSource(std::string string) = 0;

    virtual void setSurface(ANativeWindow *window, int width, int height) = 0;

    virtual void release() = 0;
};


#endif //BBQVIDEO_VIDEODECODER_H
