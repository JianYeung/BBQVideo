//
// Created by jian.yeung on 2020/10/27.
//

#include <DLog.h>
#include <GLShaderUtil.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TriangleFilter.h"

const char *vShaderStr =
        "#version 300 es                          \n"
        "uniform mat4 model;                      \n"
        "uniform mat4 view;                       \n"
        "uniform mat4 projection;                 \n"
        "layout(location = 0) in vec3 aPosition;  \n"
        "layout(location = 1) in vec4 aColor;     \n"
        "out vec4 vColor;                         \n"
        "void main()                              \n"
        "{                                        \n"
        "   gl_Position = vPosition;              \n"
        "   vColor = aColor;                      \n"
        "}                                        \n";

const char *fShaderStr =
        "#version 300 es                              \n"
        "precision mediump float;                     \n"
        "in vec4 vColor;                              \n"
        "out vec4 fragColor;                          \n"
        "void main()                                  \n"
        "{                                            \n"
        "   fragColor = vColor;                       \n"
        "}                                            \n";

const GLfloat positions[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
};

const GLfloat colors[] = {
        0.0f,1.0f,0.0f,1.0f,
        1.0f,0.0f,0.0f,1.0f,
        1.0f,0.0f,0.0f,1.0f,
};

const GLint indexs[] = {0, 2, 1};

TriangleFilter::TriangleFilter() : program(0), mPositionHandle(0), mColorHandle(0),
                                   mModelHandle(0), mViewHandle(0), mProjectionHandle(0),
                                   Model(1.0), View(1.0), Projection(1.0) {

}

TriangleFilter::~TriangleFilter() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~~TriangleFilter ~TriangleFilter~~~\n");
    }
}

void TriangleFilter::init() {

}

void TriangleFilter::unInit() {

}

void TriangleFilter::onSurfaceCreated(ANativeWindow *nativeWindow) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~Triangle Filter Render onSurfaceCreated~~~\n");
    }
    if (program != 0) {
        return;
    }
    program = GLShaderUtil::buildProgram(vShaderStr, fShaderStr);
    mPositionHandle = (GLuint) glGetAttribLocation(program, "aPosition");
    glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX, GL_FLOAT, GL_FALSE,COORDS_PER_VERTEX * sizeof(GLfloat), positions);
    mColorHandle = (GLuint) glGetAttribLocation(program, "aColor");
    glVertexAttribPointer(mColorHandle, COORDS_PER_COLORS, GL_FLOAT, GL_FALSE,COORDS_PER_COLORS * sizeof(GLfloat), colors);
    mModelHandle = glGetUniformLocation(program, "model");
    mViewHandle = glGetUniformLocation(program, "view");
    mProjectionHandle = glGetUniformLocation(program, "projection");
}

void TriangleFilter::onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~Triangle Filter Render onSurfaceChanged~~~\n");
    }
    this->format = format;
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
    float ratio = (float) width / height;
    Projection = glm::perspective(glm::radians(30.0f), ratio, 1.0f, 500.0f);
    View = glm::lookAt(glm::vec3(5.0f, 5.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
}

void TriangleFilter::draw() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~Triangle Filter Render draw~~~\n");
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glUniformMatrix4fv(mModelHandle, 1, GL_FALSE, glm::value_ptr(Model));
    glUniformMatrix4fv(mViewHandle, 1, GL_FALSE, glm::value_ptr(View));
    glUniformMatrix4fv(mProjectionHandle, 1, GL_FALSE, glm::value_ptr(Projection));
    glEnableVertexAttribArray(mPositionHandle);
    glEnableVertexAttribArray(mColorHandle);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indexs);
    glDisableVertexAttribArray(mPositionHandle);
    glDisableVertexAttribArray(mColorHandle);
}

void TriangleFilter::onDestroy() {

}