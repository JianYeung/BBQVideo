//
// Created by jian.yeung on 2020/10/27.
//

#include <DLog.h>
#include <GLShaderUtil.h>
#include <GLErrorLog.h>
#include <Constant.h>
#include "TriangleFilter.h"

const GLint VERTEX_COLOR_COORDS_LENGTH = 21;

const GLint VERTEX_INDICES_LENGTH = 3;

const GLint STRIDE_PRE_COORD = 7;

TriangleFilter::TriangleFilter() : BaseFilter() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::TriangleFilter()~~~\n");
    }
    setUp();
}

TriangleFilter::~TriangleFilter() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::~TriangleFilter()~~~\n");
    }
    tearDown();
}

void TriangleFilter::setUp() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::setUp()~~~\n");
    }
    vShaderStr =
            "#version 300 es                          \n"
            "layout(location = 0) in vec3 aPosition;  \n"
            "layout(location = 1) in vec4 aColor;     \n"
            "out vec4 vColor;                         \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vec4(aPosition, 1.0);   \n"
            "   vColor = aColor;                      \n"
            "}                                        \n";

    fShaderStr =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "in vec4 vColor;                              \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vColor;                       \n"
            "}                                            \n";
    vertex_color_coords = new GLfloat[] {
            // Positions          // Colors
             0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // Top Right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // Bottom Right
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Bottom Left
    };

    vertex_indexs = new GLshort[] {0, 1, 2};
}

void TriangleFilter::tearDown() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::tearDown()~~~\n");
    }
}

void TriangleFilter::initVAO() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::initVAO() Start~~~\n");
    }

    //generate vao vbo ebo
    glGenVertexArrays(1, &vao);
    checkGlError("glGenVertexArrays");
    glGenVertexArrays(1, &vbo);
    checkGlError("glGenVertexArrays");
    glGenVertexArrays(1, &ebo);
    checkGlError("glGenVertexArrays");

    if (DebugEnable && FILTER_DEBUG) {
        DFLOGD(TRIANGLE_FILTER_TAG, "~~~Create VAO = %d, VBO = %d, EBO = %d ~~~", vao, vbo, ebo);
    }

    // Bind VAO
    glBindVertexArray(vao);
    checkGlError("glBindVertexArray");
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGlError("glBindBuffer vbo");
    glBufferData(GL_ARRAY_BUFFER, VERTEX_COLOR_COORDS_LENGTH * BYTES_PER_FLOAT, vertex_color_coords, GL_STATIC_DRAW);
    checkGlError("glBufferData vbo");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    checkGlError("glBindBuffer ebo");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTEX_INDICES_LENGTH * BYTES_PER_SHORT, vertex_indexs, GL_STATIC_DRAW);
    checkGlError("glBufferData ebo");
    // Position attribute
    glEnableVertexAttribArray(0);
    checkGlError("glEnableVertexAttribArray position");
    glVertexAttribPointer(0, COORDS_PER_VERTEX, GL_FLOAT, false, STRIDE_PRE_COORD * BYTES_PER_FLOAT,(const void *) nullptr);
    checkGlError("glVertexAttribPointer position");
    // Color attribute
    glEnableVertexAttribArray(1);
    checkGlError("glEnableVertexAttribArray color");
    glVertexAttribPointer(1, COORDS_PER_COLOR, GL_FLOAT, false, STRIDE_PRE_COORD * BYTES_PER_FLOAT,(const void *) (COORDS_PER_VERTEX * BYTES_PER_FLOAT));
    checkGlError("glVertexAttribPointer color");
    // Unbind VAO
    glBindVertexArray(0);
    checkGlError("glBindVertexArray");

    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter initVAO() End~~~\n");
    }
}

void TriangleFilter::onSurfaceCreated(ANativeWindow *nativeWindow) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::onSurfaceCreated()~~~\n");
    }
    program = GLShaderUtil::buildProgram(vShaderStr, fShaderStr);
    if (program == GL_NONE) {
        DLOGD(TRIANGLE_FILTER_TAG, "Not build valid program");
        return;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    // 使用VAO VBO EBO
    initVAO();

    // 不实用VAO VBO EBO
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, COORDS_PER_VERTEX, GL_FLOAT, GL_FALSE, STRIDE_PRE_COORD * BYTES_PER_FLOAT, vertex_color_coords);
//
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, COORDS_PER_COLOR, GL_FLOAT, GL_FALSE, STRIDE_PRE_COORD * BYTES_PER_FLOAT, (vertex_color_coords + VERTEX_INDICES_LENGTH));
}

void
TriangleFilter::onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::onSurfaceChanged()~~~\n");
    }
    this->pixelFormat = format;
    this->surfaceWidth = width;
    this->surfaceHeight = height;
    glViewport(0, 0, width, height);
}

void TriangleFilter::updatePreviewFrame(VideoFrame *videoFrame) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::updatePreviewFrame()~~~\n");
    }
}

void TriangleFilter::clearPreviewFrame() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::clearPreviewFrame()~~~\n");
    }
}

void TriangleFilter::draw() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::draw() Start~~~\n");
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (program == GL_NONE) {
        DLOGD(TRIANGLE_FILTER_TAG, "Invalid program");
        return;
    }

    glUseProgram(program);
    checkGlError("glUseProgram");
    // 绑定VAO
    glBindVertexArray(vao);
    checkGlError("glBindVertexArray");
    // 使用EBO
    glDrawElements(GL_TRIANGLES, VERTEX_INDICES_LENGTH, GL_UNSIGNED_SHORT, (const void *) nullptr);
    // 不使用EBO
    //glDrawElements(GL_TRIANGLES, VERTEX_INDICES_LENGTH, GL_UNSIGNED_SHORT, vertex_indexs);
    checkGlError("glDrawElements");
    // 解绑VAO
    glBindVertexArray(0);
    checkGlError("glBindVertexArray");
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::draw() End~~~\n");
    }
}

void TriangleFilter::onDestroy() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::onDestroy() Start~~~\n");
    }

    if (vertex_color_coords != nullptr) {
        delete vertex_color_coords;
        vertex_color_coords = nullptr;
    }
    if (vertex_indexs != nullptr) {
        delete vertex_indexs;
        vertex_indexs = nullptr;
    }

    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(TRIANGLE_FILTER_TAG, "~~~TriangleFilter::onDestroy() End~~~\n");
    }
}