//
// Created by jian.yeung on 2020/9/14.
//

#ifndef BBQVIDEO_VIDEOH264DECODER_H
#define BBQVIDEO_VIDEOH264DECODER_H


#include "VideoDecoder.h"

class VideoH264Decoder : public VideoDecoder {
private:
    const char *H264_DECODER_TAG = "H264Decoder";

public:
    VideoH264Decoder();
    virtual ~VideoH264Decoder();

    void setSurface(ANativeWindow *nativeWindow, int width, int height) override;
    void setRender(GLRender *glRender) override;
    void setPlayStatusCallback(PlayStatusCallback *playStatusCallback) override;
    void setDataSource(std::string url) override;
    void prepare() override;
    void start() override;
    void resume() override;
    void pause() override;
    void seek(int position) override;
    void stop() override;
    void release() override;
};


#endif //BBQVIDEO_VIDEOH264DECODER_H
