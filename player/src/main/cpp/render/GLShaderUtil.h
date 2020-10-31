//
// Created by jian.yeung on 2020/10/31.
//

#ifndef BBQVIDEO_GLSHADERUTIL_H
#define BBQVIDEO_GLSHADERUTIL_H


#include <GLES3/gl3.h>

class GLShaderUtil {
public:
    static GLuint compileShader(GLenum type, const char *shaderSource);
    static GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
    static GLboolean validateProgram(int program);
    static GLuint buildProgram(const char *vertexShaderSource, const char *fragmentShaderSource);
};


#endif //BBQVIDEO_GLSHADERUTIL_H
