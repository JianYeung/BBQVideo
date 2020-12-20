//
// Created by jian.yeung on 2020/9/14.
//

#ifndef BBQVIDEO_VIDEODECODER_H
#define BBQVIDEO_VIDEODECODER_H

#include <string>
#include <android/native_window.h>
#include <GLRender.h>

#define VIDEO_DECODER_DEBUG true
#define VIDEO_DECODER_TAG "VideoDecoder"

class VideoDecoder {
protected:
    std::string videoUrl;
    ANativeWindow *surfaceWindow;
    int surfaceWidth;
    int surfaceHeight;

public:
    VideoDecoder();
    virtual ~VideoDecoder();

    virtual void setRender(GLRender *glRender) = 0;
    virtual void setDataSource(std::string url) = 0;
    virtual void setSurface(ANativeWindow *window, int width, int height) = 0;
    virtual void start() = 0;
    virtual void resume() = 0;
    virtual void pause() = 0;
    virtual void seek(int position) = 0;
    virtual void stop() = 0;
    virtual void release() = 0;
};


#endif //BBQVIDEO_VIDEODECODER_H
