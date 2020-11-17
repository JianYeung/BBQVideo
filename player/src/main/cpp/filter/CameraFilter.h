//
// Created by jian.yeung on 2020/11/14.
//

#ifndef BBQVIDEO_CAMERAFILTER_H
#define BBQVIDEO_CAMERAFILTER_H


#include "BaseFilter.h"

class CameraFilter: public BaseFilter {
private:
    const char* CAMERA_FILTER_TAG = "CameraFilter";

    //OpenGL parameter
    GLuint yTextId;
    GLuint uvTextId;
    GLfloat *vertex_color_coords;
    GLshort *vertex_indexs;

    //frame data
    unsigned char *yuvSrcData;
    unsigned char *yuv420pData;
    int yuvFormat;
    int yuvWidth;
    int yuvHeight;

private:
    void initVAO();
    void initTexture();
    void updateYUVData();
    void updateMVPMatrix();

public:
    CameraFilter();
    ~CameraFilter();

    void setUp() override;
    void tearDown() override;
    void updatePreviewFrame(unsigned char *data, int format, int width, int height) override;
    void onSurfaceCreated(ANativeWindow *nativeWindow) override;
    void onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) override;
    void draw() override;
    void onDestroy() override;
};


#endif //BBQVIDEO_CAMERAFILTER_H
