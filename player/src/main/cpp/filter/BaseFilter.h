//
// Created by jian.yeung on 2020/9/17.
//

#ifndef BBQVIDEO_BASEFILTER_H
#define BBQVIDEO_BASEFILTER_H


#include <android/native_window.h>
#include <GLES3/gl3.h>

#define FILTER_DEBUG true

enum class FilterType {
    NORMAL = 0,
    TRIANGLE = 1,
    CAMERA = 2,
    UNKNOWN = 0xFF,
};

enum class Rotation {
    NORMAL = 0,
    ROTATION_90 = 1,
    ROTATION_180 = 2,
    ROTATION_270 = 3,
    UNKNOWN = 0xFF,
};

class BaseFilter {
protected:
    //surface parameter
    int pixelFormat;
    int surfaceWidth;
    int surfaceHeight;

    //GL parameter
    const char *vShaderStr;
    const char *fShaderStr;
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    //GL Handle
    GLuint mPositionHandle;
    GLuint mColorHandle;

public:
    BaseFilter();
    ~BaseFilter();

    virtual void setUp();
    virtual void tearDown();
    virtual void updatePreviewFrame(unsigned char *data, int format, int width, int height);
    virtual void onSurfaceCreated(ANativeWindow *nativeWindow);
    virtual void onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height);
    virtual void draw();
    virtual void onDestroy();
};


#endif //BBQVIDEO_BASEFILTER_H
