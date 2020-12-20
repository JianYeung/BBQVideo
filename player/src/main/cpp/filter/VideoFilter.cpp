//
// Created by jian.yeung on 2020/12/5.
//

#include <DLog.h>
#include <GLErrorLog.h>
#include <Constant.h>
#include <GLShaderUtil.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>
#include <FrameUtil.h>
#include "VideoFilter.h"

const GLint VERTEX_COLOR_COORDS_LENGTH = 20;

const GLint VERTEX_INDICES_LENGTH = 6;

const GLint STRIDE_PRE_COORD = 5;

VideoFilter::VideoFilter() : BaseFilter(), yuvSrcData(nullptr) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::VideoFilter()~~~\n");
    }
    setUp();
}

VideoFilter::~VideoFilter() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::~VideoFilter()~~~\n");
    }
    tearDown();
}

void VideoFilter::setUp() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::setUp()~~~\n");
    }

    vShaderStr = "#version 300 es\n"
                 "layout(location = 0) in vec3 a_position;\n"
                 "layout(location = 1) in vec2 a_texcoord;\n"
                 "uniform mat4 uMVPMatrix;\n"
                 "out vec2 v_texcoord;\n"
                 "\n"
                 "void main() {\n"
                 "    v_texcoord = a_texcoord;\n"
                 "    gl_Position = uMVPMatrix * vec4(a_position, 1.0);\n"
                 "}\n";

    fShaderStr = "#version 300 es\n"
                 "precision mediump float;\n"
                 "layout(location = 0) uniform sampler2D y_texture;\n"
                 "layout(location = 1) uniform sampler2D u_texture;\n"
                 "layout(location = 2) uniform sampler2D v_texture;\n"
                 "layout(location = 3) uniform sampler2D uv_texture;\n"
                 "uniform int yuvType; //0 代表 I420, 1 代表NV21\n"
                 "in vec2 v_texcoord;\n"
                 "out vec4 outColor;\n"
                 "\n"
                 "void main() {\n"
                 "    vec3 yuv;\n"
                 "    if (yuvType == 0) {\n"
                 "        // 因为I420是YUV的一个平面，所以采样后的r,g,b,a这四个参数的数值是一样的\n"
                 "        yuv.x = texture(y_texture, v_texcoord).r;     \n"
                 "        yuv.y = texture(uv_texture, v_texcoord).r-0.5;\n"
                 "        yuv.z = texture(uv_texture, v_texcoord).r-0.5;\n"
                 "    } else {\n"
                 "        // 因为NV21是2平面的，对于UV平面，在加载纹理时，会指定格式，让U值存在a中，V值存在r,g,b中\n"
                 "        yuv.x = texture(y_texture, v_texcoord).r;     \n"
                 "        yuv.y = texture(uv_texture, v_texcoord).a-0.5;\n"
                 "        yuv.z = texture(uv_texture, v_texcoord).r-0.5;\n"
                 "    }\n"
                 "\n"
                 "    highp vec3 rgb = mat3(1, 1, 1,\n"
                 "                        0, -0.344, 1.770,\n"
                 "                        1.403, -0.714, 0) * yuv;\n"
                 "    outColor = vec4(rgb, 1);\n"
                 "}\n";

    vertex_color_coords = new GLfloat[]{
            // Positions        // Texture Coords
            1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };

    vertex_indexs = new GLshort[]{
            0, 1, 2,
            0, 2, 3
    };
}

void VideoFilter::tearDown() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::tearDown()~~~\n");
    }
}

void VideoFilter::initVAO() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::initVAO() Start~~~\n");
    }

    //generate vao vbo ebo
    glGenVertexArrays(1, &vao);
    checkGlError("glGenVertexArrays");
    glGenVertexArrays(1, &vbo);
    checkGlError("glGenVertexArrays");
    glGenVertexArrays(1, &ebo);
    checkGlError("glGenVertexArrays");

    if (DebugEnable && FILTER_DEBUG) {
        DFLOGD(VIDEO_FILTER_TAG, "~~~Create VAO = %d, VBO = %d, EBO = %d ~~~\n", vao, vbo, ebo);
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
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::initVAO() End~~~\n");
    }
}

void VideoFilter::initTexture() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::initTexture() Start~~~\n");
    }

    textIds = new GLuint[4];

    for (int i = 0; i < 4; ++i) {
        glActiveTexture(GL_TEXTURE0+i);
        glGenTextures(1, &textIds[i]);
        checkGlError("glGenTextures");
        glBindTexture(GL_TEXTURE_2D, textIds[i]);
        checkGlError("glBindTexture");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        checkGlError("glTexParameteri");
        glBindTexture(GL_TEXTURE_2D, 0);
        checkGlError("glBindTexture");
    }

    yuvTypeHandle = glGetUniformLocation(program, "yuvType");

    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::initTexture() End~~~\n");
    }
}

void VideoFilter::initMatrixHandle() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::initMatrixHandle() Start~~~\n");
    }
    mvpMatrixHandle = glGetUniformLocation(program, "uMVPMatrix");

    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::initMatrixHandle() End~~~\n");
    }
}

void VideoFilter::onSurfaceCreated(ANativeWindow *nativeWindow) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::onSurfaceCreated()~~~\n");
    }
    program = GLShaderUtil::buildProgram(vShaderStr, fShaderStr);
    if (program == GL_NONE) {
        DLOGD(VIDEO_FILTER_TAG, "Not build valid program\n");
        return;
    }

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    initVAO();
    initTexture();
    initMatrixHandle();
}

void VideoFilter::onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::onSurfaceChanged()~~~\n");
    }
    this->pixelFormat = format;
    this->surfaceWidth = width;
    this->surfaceHeight = height;
    glViewport(0, 0, surfaceWidth, surfaceHeight);
}

void VideoFilter::updateMVPMatrix() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::updateMVPMatrix()~~~\n");
    }
    glm::mat4 myIdentityMatrix = glm::mat4(1.0);
    glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, glm::value_ptr(myIdentityMatrix));
}

void VideoFilter::updatePreviewFrame(VideoFrame *videoFrame) {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::updatePreviewFrame()~~~\n");
    }
    if (!isValidVideoFrame(videoFrame)) {
        DLOGE(VIDEO_FILTER_TAG, "yuv data or width or height is invalid\n");
        return;
    }
    this->yuvSrcData = videoFrame->getData();
    this->yuvFormat = videoFrame->getFormat();
    this->yuvWidth = videoFrame->getWidth();
    this->yuvHeight = videoFrame->getHeight();
}

void VideoFilter::updateYUVType() {
    if (DebugEnable && FILTER_DEBUG) {
        DFLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::updateYUVType() yuvType: %d ~~~\n", yuvType);
    }
    switch (yuvFormat) {
        case YUV420P:
            this->yuvType = 0;
            break;
        case YUV420SP:
        default:
            this->yuvType = 1;
            break;
    }

    glUniform1i(yuvTypeHandle, yuvType);
}

void VideoFilter::updateTextureData() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::updateTextureData()~~~\n");
    }

    if (yuvSrcData == nullptr || yuvWidth <= 0 || yuvHeight <= 0) {
        DLOGE(VIDEO_FILTER_TAG, "yuvData or yuvWidth or yuvHeight is illegal\n");
        return;
    }

    if (yuvType == 0) {
        updateTextureY(yuvSrcData, yuvWidth, yuvHeight, 0);
        updateTextureY(yuvSrcData + yuvWidth * yuvHeight, yuvWidth/2, yuvHeight/2, 1);
        updateTextureY(yuvSrcData + yuvWidth * yuvHeight * 5/4, yuvWidth/2, yuvHeight/2, 2);
    } else {
        updateTextureY(yuvSrcData, yuvWidth, yuvHeight, 0);
        updateTextureUV(yuvSrcData + yuvWidth * yuvHeight, yuvWidth/2, yuvHeight/2, 3);
    }
}

void VideoFilter::updateTextureY(uint8_t *data, int width, int height, int index) {
    if (DebugEnable && FILTER_DEBUG) {
        DFLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::updateTextureY() index: %d ~~~\n", index);
    }

    if (data == nullptr || width == 0 || height == 0 || index > 3) {
        DLOGE(VIDEO_FILTER_TAG, "data or width or height is illegal\n");
        return;
    }

    glActiveTexture(GL_TEXTURE0+index);
    checkGlError("glActiveTexture");
    glBindTexture(GL_TEXTURE_2D, textIds[index]);
    checkGlError("glBindTexture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
    checkGlError("glTexImage2D");
    glUniform1i(index,  index);
    checkGlError("glUniform1i");
}

void VideoFilter::updateTextureUV(uint8_t *data, int width, int height, int index) {
    if (DebugEnable && FILTER_DEBUG) {
        DFLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::updateTextureUV() index: %d ~~~\n", index);
    }

    if (data == nullptr || width == 0 || height == 0 || index > 3) {
        DLOGE(VIDEO_FILTER_TAG, "data or width or height is illegal\n");
        return;
    }

    glActiveTexture(GL_TEXTURE0+index);
    checkGlError("glActiveTexture");
    glBindTexture(GL_TEXTURE_2D, textIds[index]);
    checkGlError("glBindTexture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
    checkGlError("glTexImage2D");
    glUniform1i(index, index);
    checkGlError("glUniform1i");
}

void VideoFilter::draw() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::draw() Start~~~\n");
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (program == GL_NONE) {
        DLOGD(VIDEO_FILTER_TAG, "Invalid program\n");
        return;
    }
    glUseProgram(program);
    checkGlError("glUseProgram");
    // 更新MVP矩阵
    updateMVPMatrix();
    // 更新YUV类型
    updateYUVType();
    // 更新纹理采样数据
    updateTextureData();
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
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::draw() End~~~\n");
    }
}

void VideoFilter::onDestroy() {
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::onDestroy() Start~~~\n");
    }

    if (vertex_color_coords != nullptr) {
        delete vertex_color_coords;
        vertex_color_coords = nullptr;
    }
    if (vertex_indexs != nullptr) {
        delete vertex_indexs;
        vertex_indexs = nullptr;
    }
    if (textIds != nullptr) {
        delete [] textIds;
        textIds = nullptr;
    }
    if (DebugEnable && FILTER_DEBUG) {
        DLOGI(VIDEO_FILTER_TAG, "~~~VideoFilter::onDestroy() End~~~\n");
    }
}