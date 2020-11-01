//
// Created by jian.yeung on 2020/10/31.
//

#include <DLog.h>
#include <cstdlib>
#include "GLShaderUtil.h"
#include "GLErrorLog.h"

const char *GLShaderUtil_TAG = "GLShaderUtil";

GLuint GLShaderUtil::compileShader(GLenum type, const char *shaderSource) {
    DLOGD(GLShaderUtil_TAG, "~~~compileShader~~~\n");
    GLuint shader = glCreateShader(type);
    checkGlError("glCreateShader");
    if (shader == 0) {
        DLOGE(GLShaderUtil_TAG, "Could not create new shader.\n");
    }
    glShaderSource(shader, 1, &shaderSource, NULL);
    checkGlError("glShaderSource");
    glCompileShader(shader);
    checkGlError("glCompileShader");
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char *buf = (char *) malloc(sizeof(char) * infoLen);
            if (buf) {
                glGetShaderInfoLog(shader, infoLen, NULL, buf);
                DFLOGE(GLShaderUtil_TAG, "Could not compile shader %d:\n%s\n", shader, buf);
            }
            free(buf);
        }
        glDeleteShader(shader);
        shader = 0;
    } else {
        DFLOGD(GLShaderUtil_TAG, "Create type = 0x%x, shader = %d Success! \n", type, shader);
    }
    return shader;
}

GLuint GLShaderUtil::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    DLOGD(GLShaderUtil_TAG, "~~~linkProgram~~~\n");
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
                    DFLOGE(GLShaderUtil_TAG, "Could not link program %d:\n%s\n", program, buf);
                }
                free(buf);
            }
            glDeleteProgram(program);
            program = 0;
        }
    } else {
        DLOGE(GLShaderUtil_TAG, "Could not create program.\n");
    }
    return program;
}

GLboolean GLShaderUtil::validateProgram(int program) {
    DLOGD(GLShaderUtil_TAG, "~~~validateProgram~~~\n");
    if (!program) {
        DLOGE(GLShaderUtil_TAG, "programId invalid\n");
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
                DFLOGE(GLShaderUtil_TAG, "Could not validate program %d:\n%s\n", program, buf);
            }
            free(buf);
        }
        return GL_FALSE;
    }
    return GL_TRUE;
}

GLuint GLShaderUtil::buildProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
    DLOGD(GLShaderUtil_TAG, "~~~buildProgram Start~~~\n");
    DFLOGD(GLShaderUtil_TAG, "vertexShaderSource =%s\n", vertexShaderSource);
    DFLOGD(GLShaderUtil_TAG, "fragmentShaderSource =%s\n", fragmentShaderSource);
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = linkProgram(vertexShader, fragmentShader);
    validateProgram(program);
    DLOGD(GLShaderUtil_TAG, "~~~buildProgram End~~~\n");
    return program;
}
