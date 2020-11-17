//
// Created by jian.yeung on 2020/11/14.
//
#include <DLog.h>
#include <GLShaderUtil.h>
#include <GLErrorLog.h>
#include <Constant.h>
#include "CameraFilter.h"
#include "ImageUtil.h"

const GLint VERTEX_COLOR_COORDS_LENGTH = 20;

const GLint VERTEX_INDICES_LENGTH = 6;

const GLint STRIDE_PRE_COORD = 5;

CameraFilter::CameraFilter() : BaseFilter() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~~CameraFilter CameraFilter()~~~\n");
    }
    setUp();
}

CameraFilter::~CameraFilter() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~~CameraFilter ~CameraFilter()~~~\n");
    }
    tearDown();
}

void CameraFilter::setUp() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~~CameraFilter setUp()~~~\n");
    }
    vShaderStr =
            "#version 300 es                   "
            "         \n"
            "layout(location = 0) in vec4 a_position;   \n"
            "layout(location = 1) in vec2 a_texcoord;   \n"
            "out vec2 v_texcoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = a_position;               \n"
            "   v_texcoord = a_texcoord;                \n"
            "}                                          \n";

    fShaderStr =
            "#version 300 es                                                        \n"
            "precision mediump float;                                               \n"
            "layout(location = 0) uniform sampler2D samplerY;                       \n"
            "layout(location = 1) uniform sampler2D samplerUV;                      \n"
            "in vec2 v_texcoord;                                                    \n"
            "out vec4 outColor;                                                     \n"
            "void main() {                                                          \n"
            "   vec4 c = vec4((texture(samplerY, v_texcoord).r - 16./255.) * 1.164);\n"
            "   vec4 U; vec4 V;                                                     \n"
            "    U = vec4(texture(samplerUV, v_texcoord).r - 128./255.);            \n"
            "    V = vec4(texture(samplerUV, v_texcoord).a - 128./255.);            \n"
            "   c += V * vec4(1.596, -0.813, 0, 0);                                 \n"
            "   c += U * vec4(0, -0.392, 2.017, 0);                                 \n"
            "   c.a = 1.0;                                                          \n"
            "   outColor = c;                                                       \n"
            "}                                                                      \n";

    vertex_color_coords = new GLfloat[]{
            // Positions        // Texture Coords
             1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 0.0f
    };

    vertex_indexs = new GLshort[]{
            0, 1, 2,
            0, 2, 3
    };
}

void CameraFilter::tearDown() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~~CameraFilter tearDown()~~~\n");
    }
}

void CameraFilter::initVAO() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~~CameraFilter initVAO() Start~~~\n");
    }

    //generate vao vbo ebo
    glGenVertexArrays(1, &vao);
    checkGlError("glGenVertexArrays");
    glGenVertexArrays(1, &vbo);
    checkGlError("glGenVertexArrays");
    glGenVertexArrays(1, &ebo);
    checkGlError("glGenVertexArrays");

    if (DebugEnable && FILTER_DEBUG) {
        DFLOGD(CAMERA_FILTER_TAG, "~~~~Create VAO = %d, VBO = %d, EBO = %d ~~~\n", vao, vbo, ebo);
    }

    // Bind VAO
    glBindVertexArray(vao);
    checkGlError("glBindVertexArray");
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGlError("glBindBuffer vbo");
    glBufferData(GL_ARRAY_BUFFER, VERTEX_COLOR_COORDS_LENGTH * BYTES_PER_FLOAT, vertex_color_coords,
                 GL_STATIC_DRAW);
    checkGlError("glBufferData vbo");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    checkGlError("glBindBuffer ebo");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTEX_INDICES_LENGTH * BYTES_PER_SHORT, vertex_indexs,
                 GL_STATIC_DRAW);
    checkGlError("glBufferData ebo");
    // Position attribute
    glEnableVertexAttribArray(0);
    checkGlError("glEnableVertexAttribArray position");
    glVertexAttribPointer(0, COORDS_PER_VERTEX, GL_FLOAT, false, STRIDE_PRE_COORD * BYTES_PER_FLOAT,(const void *) nullptr);
    checkGlError("glVertexAttribPointer position");
    // Texture attribute
    glEnableVertexAttribArray(1);
    checkGlError("glEnableVertexAttribArray texture");
    glVertexAttribPointer(1, COODS_PRE_TEXTURE, GL_FLOAT, false, STRIDE_PRE_COORD * BYTES_PER_FLOAT,(const void *) (COORDS_PER_VERTEX * BYTES_PER_FLOAT));
    checkGlError("glVertexAttribPointer texture");
    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    checkGlError("glBindBuffer vbo");
    // Unbind VAO
    glBindVertexArray(0);
    checkGlError("glBindVertexArray");
    // Unbind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    checkGlError("glBindBuffer ebo");

    if (DebugEnable && FILTER_DEBUG) {
        DLOGD(CAMERA_FILTER_TAG, "~~~~CameraFilter initVAO() End~~~\n");
    }
}

void CameraFilter::initTexture() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter initTexture() Start~~~\n");
    }
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &yTextId);
    checkGlError("glGenTextures");
    glBindTexture(GL_TEXTURE_2D, yTextId);
    checkGlError("glBindTexture");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    checkGlError("glTexParameteri");
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGlError("glBindTexture");


    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &uvTextId);
    checkGlError("glGenTextures");
    glBindTexture(GL_TEXTURE_2D, uvTextId);
    checkGlError("glBindTexture");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    checkGlError("glTexParameteri");
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGlError("glBindTexture");


    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter initTexture() End~~~\n");
    }
}

void CameraFilter::onSurfaceCreated(ANativeWindow *nativeWindow) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter onSurfaceCreated()~~~\n");
    }
    program = GLShaderUtil::buildProgram(vShaderStr, fShaderStr);
    if (program == GL_NONE) {
        DLOGD(CAMERA_FILTER_TAG, "Not build valid program\n");
        return;
    }

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    initVAO();
    initTexture();
}

void
CameraFilter::onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter onSurfaceChanged()~~~\n");
    }
    this->pixelFormat = format;
    this->surfaceWidth = width;
    this->surfaceHeight = height;
    glViewport(0, 0, surfaceWidth, surfaceHeight);
}

void CameraFilter::updatePreviewFrame(unsigned char *data, int format, int width, int height) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter updatePreviewFrame()~~~\n");
    }
    if (data == nullptr || width == 0 || height == 0) {
        DLOGE(CAMERA_FILTER_TAG, "~~~CameraFilter updatePreviewFrame() yuv data or width or height is invalid~~~\n");
        return;
    }
    this->yuvFormat = format;
    this->yuvWidth = width;
    this->yuvHeight = height;
    this->yuvSrcData = data;
}

void CameraFilter::updateYUVData() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter updateYUVData()~~~\n");
    }

    if (yuvSrcData == nullptr || yuvWidth == 0 || yuvHeight == 0) {
        DLOGE(CAMERA_FILTER_TAG, "~~~CameraFilter yuvData or yuvWidth or yuvHeight is illegal~~~\n");
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    checkGlError("glActiveTexture");
    glBindTexture(GL_TEXTURE_2D, yTextId);
    checkGlError("glBindTexture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, yuvWidth, yuvHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, yuvSrcData);
    checkGlError("glTexImage2D");
    glUniform1i(0,  0); // y_texture的location=0, 把纹理0赋值给y_texture
    checkGlError("glUniform1i");

    glActiveTexture(GL_TEXTURE1);
    checkGlError("glActiveTexture");
    glBindTexture(GL_TEXTURE_2D, uvTextId);
    checkGlError("glBindTexture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, yuvWidth / 2, yuvHeight / 2, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, yuvSrcData + yuvWidth * yuvHeight);
    checkGlError("glTexImage2D");
    glUniform1i(1, 1); // u_texture的location=1, 把纹理1赋值给u_texture
    checkGlError("glUniform1i");
}

void CameraFilter::updateMVPMatrix() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter updateMVPMatrix()~~~\n");
    }
}

void CameraFilter::draw() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter draw() Start~~~\n");
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (program == GL_NONE) {
        DLOGD(CAMERA_FILTER_TAG, "Invalid program\n");
        return;
    }
    glUseProgram(program);
    checkGlError("glUseProgram");
    // 绑定MVP矩阵
    updateMVPMatrix();
    // 绑定YUV data
    updateYUVData();
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
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter draw() End~~~\n");
    }
}

void CameraFilter::onDestroy() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(CAMERA_FILTER_TAG, "~~~CameraFilter onDestroy()~~~\n");
    }
    if (vertex_color_coords != nullptr) {
        delete vertex_color_coords;
        vertex_color_coords = nullptr;
    }
    if (vertex_indexs != nullptr) {
        delete vertex_indexs;
        vertex_indexs = nullptr;
    }
}