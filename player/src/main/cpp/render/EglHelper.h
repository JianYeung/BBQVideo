//
// Created by jian.yeung on 2020/9/23.
//

#ifndef BBQVIDEO_EGLHELPER_H
#define BBQVIDEO_EGLHELPER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "Constant.h"

// EGL config attributes
const EGLint windowConfAttr[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, //EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
        EGL_RED_SIZE, CFG_RED_SIZE,
        EGL_GREEN_SIZE, CFG_GREEN_SIZE,
        EGL_BLUE_SIZE, CFG_BLUE_SIZE,
        EGL_ALPHA_SIZE, CFG_ALPHA_SIZE, // if you need the alpha channel
        EGL_DEPTH_SIZE, CFG_DEPTH_SIZE, // if you need the depth buffer
        EGL_STENCIL_SIZE, CFG_STENCIL_SIZE,
        EGL_NONE
};

const EGLint pbufferConfAttr[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, //EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
        EGL_RED_SIZE, CFG_RED_SIZE,
        EGL_GREEN_SIZE, CFG_GREEN_SIZE,
        EGL_BLUE_SIZE, CFG_BLUE_SIZE,
        EGL_ALPHA_SIZE, CFG_ALPHA_SIZE, // if you need the alpha channel
        EGL_DEPTH_SIZE, CFG_DEPTH_SIZE, // if you need the depth buffer
        EGL_STENCIL_SIZE, CFG_STENCIL_SIZE,
        EGL_NONE
};

// EGL context attributes
const EGLint ctxAttr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
};

const bool EGL_DEBUG = false;

enum EGLSurfaceType {
    WINDOW_SURFACE,
    PBUFFER_SURFACE,
};

class EglHelper {

private:
    const char *EGL_TAG = "EglHelper";
    EGLSurfaceType mEglSurfaceType = WINDOW_SURFACE;

    EGLint mEglMajorVersion;
    EGLint mEglMinorVersion;
    EGLint numConfigs;

    EGLDisplay mEglDisplay;
    EGLConfig mEglConfig;
    EGLContext mEglContext = nullptr;
    EGLSurface mEglSurface = nullptr;

    int swapEglError = EGL_SUCCESS;

private:
    bool inner_init();
    bool inner_createWindowEglSurface(ANativeWindow *nativeWindow);
    bool inner_createPbufferEglSurface(int width, int height);
    void inner_makeEglContext();
    void inner_doneEglContext();
    bool inner_swapBuffer();
    void inner_destroyEglSurface();
    void inner_destroyEglSurfaceImp();
    void inner_finish();

public:
    EglHelper();
    ~EglHelper();

    bool start();
    bool start(EGLSurfaceType surfaceType);
    bool createEglSurface(ANativeWindow *nativeWindow);
    bool createEglSurface(ANativeWindow *nativeWindow, int width, int height);
    bool swapBuffer();
    bool hasEglSurface();
    bool hasEglContext();
    int getSwapEglError();
    void destroyEglSurface();
    void finish();
};


#endif //BBQVIDEO_EGLHELPER_H
