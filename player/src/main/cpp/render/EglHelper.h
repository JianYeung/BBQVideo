//
// Created by jian.yeung on 2020/9/23.
//

#ifndef BBQVIDEO_EGLHELPER_H
#define BBQVIDEO_EGLHELPER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>

// EGL config attributes
const EGLint windowConfAttr[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, //EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,// if you need the alpha channel
        EGL_DEPTH_SIZE, 16,// if you need the depth buffer
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
};

const EGLint pbufferConfAttr[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, //EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,// if you need the alpha channel
        EGL_DEPTH_SIZE, 16,// if you need the depth buffer
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
};

// EGL context attributes
const EGLint ctxAttr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
};

const bool EGL_DEBUG = true;

enum EGLSurfaceType {
    WINDOW_SURFACE,
    PBUFFER_SURFACE,
};

class EglHelper {

private:
    const char* EGL_TAG = "EglHelper";

    EGLint eglMajorVersion;
    EGLint eglMinorVersion;
    EGLint numConfigs;

    EGLDisplay eglDisplay;
    EGLConfig eglConfig;
    EGLContext eglContext;
    EGLSurface eglSurface;

    EGLSurfaceType eglSurfaceType = WINDOW_SURFACE;
    ANativeWindow *nativeWindow;

    PFNEGLPRESENTATIONTIMEANDROIDPROC pfneglPresentationTimeANDROID;

public:
    bool lostEglContext = false;
    bool eglSurfaceIsBad = false;

private:
    bool inner_init();
    bool inner_createWindowSurface(ANativeWindow *nativeWindow);
    bool inner_createPbufferSurface(int width, int height);
    void inner_makeContext();
    void inner_doneContext();
    void inner_swap();
    void inner_destroySurface();
    void inner_destroySurfaceImp();
    void inner_release();

public:
    EglHelper();
    ~EglHelper();

    bool start();
    bool start(EGLSurfaceType surfaceType);
    bool createSurface(ANativeWindow * nativeWindow, int width, int height);
    void swap();
    void destroySurface();
    void release();
};


#endif //BBQVIDEO_EGLHELPER_H
