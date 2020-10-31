//
// Created by jian.yeung on 2020/9/14.
//

#ifndef BBQVIDEO_VIDEOH264DECODER_H
#define BBQVIDEO_VIDEOH264DECODER_H


#include "VideoDecoder.h"

class VideoH264Decoder : public VideoDecoder {

public:
    void setSource(std::string url) override;
    void setSurface(ANativeWindow *nativeWindow, int width, int height) override;
    void release() override;
};


#endif //BBQVIDEO_VIDEOH264DECODER_H
