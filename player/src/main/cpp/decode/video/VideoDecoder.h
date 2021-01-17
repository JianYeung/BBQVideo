//
// Created by jian.yeung on 2020/9/14.
//

#ifndef BBQVIDEO_VIDEODECODER_H
#define BBQVIDEO_VIDEODECODER_H

#include <string>
#include <android/native_window.h>
#include <GLRender.h>
#include <VideoPlayerStatusCallback.h>
#include <OnPreparedListener.h>
#include <OnErrorListener.h>

#define VIDEO_DECODER_DEBUG true
#define VIDEO_DECODER_TAG "VideoDecoder"

class VideoDecoder {
protected:
    std::string videoUrl;
    ANativeWindow *surfaceWindow;
    int surfaceWidth;
    int surfaceHeight;
    VideoPlayerStatusCallback *playerStatusCallback;
    OnPreparedListener *onPreparedListener;
    OnErrorListener *onErrorListener;

    GLRender *render;
    BaseFilter *filter;

public:
    VideoDecoder();
    virtual ~VideoDecoder();

    virtual void setSurface(ANativeWindow *window, int width, int height) = 0;
    virtual void setRender(GLRender *render) = 0;
    virtual void setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) = 0;
    virtual void setOnPreparedListener(OnPreparedListener *onPreparedListener) = 0;
    virtual void setOnErrorListener(OnErrorListener *onErrorListener) = 0;
    virtual void setDataSource(std::string url) = 0;
    virtual void prepare() = 0;
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void seek(int position) = 0;
    virtual void stop() = 0;
    virtual void release() = 0;
};


#endif //BBQVIDEO_VIDEODECODER_H
