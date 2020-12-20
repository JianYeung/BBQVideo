//
// Created by jian.yeung on 2020/12/5.
//

#ifndef BBQVIDEO_VIDEOFILTER_H
#define BBQVIDEO_VIDEOFILTER_H


#include "BaseFilter.h"
#include <android/surface_texture.h>

#define YUV420P 19
#define YUV420SP 21

class VideoFilter : public BaseFilter {
private:
    const char *VIDEO_FILTER_TAG = "VideoFilter";
    //OpenGL parameter
    GLfloat *vertex_color_coords;
    GLshort *vertex_indexs;
    GLuint *textIds;
    GLint mvpMatrixHandle;
    GLint yuvType;
    GLint yuvTypeHandle;

    //frame parameter
    uint8_t *yuvData;
    int yuvFormat;
    int yuvWidth;
    int yuvHeight;

private:
    void initVAO();
    void initTexture();
    void initMatrixHandle();
    void updateYUVType();
    void updateMVPMatrix();
    void updateTextureData();
    void updateTextureY(uint8_t* data, int width, int height, int index);
    void updateTextureUV(uint8_t* data, int width, int height, int index);

public:
    VideoFilter();
    ~VideoFilter();

    void setUp() override;
    void tearDown() override;
    void updatePreviewFrame(VideoFrame *videoFrame) override;
    void onSurfaceCreated(ANativeWindow *nativeWindow) override;
    void onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) override;
    void draw() override;
    void onDestroy() override;
};


#endif //BBQVIDEO_VIDEOFILTER_H
