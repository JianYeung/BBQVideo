//
// Created by jian.yeung on 2020/9/14.
//

#ifndef BBQVIDEO_VIDEODECODER_H
#define BBQVIDEO_VIDEODECODER_H

#include <string>
#include <android/native_window.h>
#include <GLRender.h>
#include <VideoPlayerStatusCallback.h>
#include <PreparedStatusListener.h>
#include <ErrorStatusListener.h>

#define VIDEO_DECODER_DEBUG true
#define VIDEO_DECODER_TAG "VideoDecoder"

class VideoDecoder {
protected:
    std::string videoUrl;
    ANativeWindow *surfaceWindow;
    int surfaceWidth;
    int surfaceHeight;
    VideoPlayerStatusCallback *playerStatusCallback;
    PreparedStatusListener *preparedStatusListener;
    ErrorStatusListener *errorStatusListener;

    GLRender *glRender;
    BaseFilter *filter;

public:
    VideoDecoder();
    virtual ~VideoDecoder();

    virtual void setSurface(ANativeWindow *window, int width, int height) = 0;
    virtual void setRender(GLRender *render) = 0;
    virtual void setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) = 0;
    virtual void setPreparedStatusListener(PreparedStatusListener *preparedStatusListener) = 0;
    virtual void setErrorStatusListener(ErrorStatusListener *errorStatusListener) = 0;
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
