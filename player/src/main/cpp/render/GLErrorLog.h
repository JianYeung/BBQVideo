//
// Created by jian.yeung on 2020/9/26.
//

#ifndef BBQVIDEO_GLERRORLOG_H
#define BBQVIDEO_GLERRORLOG_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#define EGL_ERROR_TAG "egl_error"
#define GL_ERROR_TAG "gl_error"

static void checkEglError(const char* op)
{
    EGLint error;
    for (error = eglGetError(); error; error = eglGetError())
    {
        DFLOGE(EGL_ERROR_TAG, "error::after %s() eglError (0x%x)\n", op, error);
    }
}

static void checkGlError(const char* op)
{
    GLint error;
    for (error = glGetError(); error; error = glGetError())
    {
        DFLOGE(GL_ERROR_TAG, "error::after %s() glError (0x%x)\n", op, error);
    }
}


#endif //BBQVIDEO_GLERRORLOG_H
