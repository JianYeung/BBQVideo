//
// Created by jian.yeung on 2020/10/28.
//

#ifndef BBQVIDEO_SHADERUTIL_H
#define BBQVIDEO_SHADERUTIL_H

#include <GLES3/gl3.h>
#include <cstdlib>
#include <GLErrorLog.h>

const char *ShaderUtil_TAG = "ShaderUtil";

GLuint compileShader(GLenum type, const char *shaderSource) {
    DLOGD(ShaderUtil_TAG, "~~~compileShader~~~\n");
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        DLOGE(ShaderUtil_TAG, "Could not create new shader.\n");
    }
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char *buf = (char *) malloc(sizeof(char) * infoLen);
            if (buf) {
                glGetShaderInfoLog(shader, infoLen, NULL, buf);
                DFLOGE(ShaderUtil_TAG, "Could not compile shader %d:\n%s\n", shader, buf);
            }
            free(buf);
        }
        glDeleteShader(shader);
        shader = 0;
    } else {
        DFLOGD(ShaderUtil_TAG, "Create a new shader %d\n", shader);
    }
    return shader;
}

GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    DLOGD(ShaderUtil_TAG, "~~~linkProgram~~~\n");
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, fragmentShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        checkGlError("glLinkProgram");
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        GLint linkStatus = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char *buf = (char *) malloc(sizeof(char) * infoLen);
                if (buf) {
                    glGetProgramInfoLog(program, infoLen, NULL, buf);
                    DFLOGE(ShaderUtil_TAG, "Could not link program %d:\n%s\n", program, buf);
                }
                free(buf);
            }
            glDeleteProgram(program);
            program = 0;
        }
    } else {
        DLOGE(ShaderUtil_TAG, "Could not create program.\n");
    }
    return program;
}

GLboolean validateProgram(int program) {
    DLOGD(ShaderUtil_TAG, "~~~validateProgram~~~\n");
    if (!program) {
        DLOGE(ShaderUtil_TAG, "programId invalid\n");
        return GL_FALSE;
    }

    glValidateProgram(program);
    checkGlError("glValidateProgram");

    GLint validStatus = 0;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validStatus);
    if (!validStatus) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char *buf = (char *) malloc(sizeof(char) * infoLen);
            if (buf) {
                glGetProgramInfoLog(program, infoLen, NULL, buf);
                DFLOGE(ShaderUtil_TAG, "Could not validate program %d:\n%s\n", program, buf);
            }
            free(buf);
        }
        return GL_FALSE;
    }
    return GL_TRUE;
}

GLuint buildProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
    DLOGD(ShaderUtil_TAG, "~~~buildProgram~~~\n");
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = linkProgram(vertexShader, fragmentShader);
    validateProgram(program);
    return program;
}

#endif //BBQVIDEO_SHADERUTIL_H
