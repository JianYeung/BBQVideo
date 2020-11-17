//
// Created by jian.yeung on 2020/11/15.
//

#ifndef BBQVIDEO_IMAGEUTIL_H
#define BBQVIDEO_IMAGEUTIL_H

#include <cstring>

const char *IMAGE_UTIL_TAG = "ImageUtil";

static void
nv21To420pData(unsigned char *yuvSourceData, int width, int height, unsigned char *yuvTargetData) {
    if (yuvSourceData == nullptr) {
        DLOGE(IMAGE_UTIL_TAG, "YUV source data is NULL");
        return;
    }
    DFLOGD(IMAGE_UTIL_TAG, "YUV source width = %d, height = %d", width, height);

    int ySize = width * height;
    int i, j;

    //y
    for (i =0; i < ySize; i++) {
        yuvTargetData[i] = yuvSourceData[i];
    }

    //u
    i =0;
    for (j =1; j < ySize /2; j+=2) {
        yuvTargetData[ySize + i] = yuvSourceData[ySize + j];
        i++;
    }

    //v
    i =0;
    for (j =0; j < ySize /2; j+=2) {
        yuvTargetData[ySize *5 /4 + i] = yuvSourceData[ySize + j];
        i++;
    }
}

#endif //BBQVIDEO_IMAGEUTIL_H
