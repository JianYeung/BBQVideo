//
// Created by jian.yeung on 2020/12/6.
//

#ifndef BBQVIDEO_VIDEOFRAME_H
#define BBQVIDEO_VIDEOFRAME_H

#include <cstdint>

class VideoFrame {
private:
    uint8_t *data;
    int format;
    int width;
    int height;

public:
    VideoFrame();

    ~VideoFrame();

    void updateFrameInfo(uint8_t *data, int format, int width, int height);

    uint8_t *getData();

    int getFormat();

    int getWidth();

    int getHeight();
};


#endif //BBQVIDEO_VIDEOFRAME_H
