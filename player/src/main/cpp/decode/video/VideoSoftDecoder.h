//
// Created by jian.yeung on 2020/9/14.
//

#ifndef BBQVIDEO_VIDEOSOFTDECODER_H
#define BBQVIDEO_VIDEOSOFTDECODER_H


#include "VideoDecoder.h"

#define VIDEO_SOFT_DECODER_TAG "VideoSoftDecoder"

class VideoSoftDecoder : public VideoDecoder {
public:
    VideoSoftDecoder();
    virtual ~VideoSoftDecoder();

    void setSurface(ANativeWindow *nativeWindow, int width, int height) override;
    void setRender(GLRender *render) override;
    void setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) override;
    void setPreparedStatusListener(PreparedStatusListener *preparedStatusListener) override;
    void setErrorStatusListener(ErrorStatusListener *errorStatusListener) override;
    void setDataSource(std::string url) override;
    void prepare() override;
    void start() override;
    void pause() override;
    void resume() override;
    void seek(int position) override;
    void stop() override;
    void release() override;
};


#endif //BBQVIDEO_VIDEOSOFTDECODER_H
