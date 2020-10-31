//
// Created by jian.yeung on 2020/9/13.
//

#ifndef BBQVIDEO_VIDEOHARDDECODER_H
#define BBQVIDEO_VIDEOHARDDECODER_H


#include "VideoDecoder.h"

class VideoHardDecoder : public VideoDecoder {

public:
    void setSource(std::string url) override;
    void setSurface(ANativeWindow *nativeWindow, int width, int height) override;
    void release() override;
private:

};


#endif //BBQVIDEO_VIDEOHARDDECODER_H
