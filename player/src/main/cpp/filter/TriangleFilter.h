//
// Created by jian.yeung on 2020/10/27.
//

#ifndef BBQVIDEO_TRIANGLEFILTER_H
#define BBQVIDEO_TRIANGLEFILTER_H

#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include "BaseFilter.h"

static const int COORDS_PER_VERTEX = 3;
static const int COORDS_PER_COLORS = 4;

class TriangleFilter : public BaseFilter {
private:
    const char* TRIANGLE_FILTER_TAG = "TriangleFilter";
    //parameter
    GLuint program;

    //Matrix
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;

    //Handle
    GLuint mPositionHandle;
    GLuint mColorHandle;
    GLint mModelHandle;
    GLint mViewHandle;
    GLint mProjectionHandle;

public:
    TriangleFilter();
    ~TriangleFilter();

    void init() override;
    void unInit() override;
    void onSurfaceCreated(ANativeWindow *nativeWindow) override;
    void onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) override;
    void draw() override;
    void onDestroy() override;
};


#endif //BBQVIDEO_TRIANGLEFILTER_H
