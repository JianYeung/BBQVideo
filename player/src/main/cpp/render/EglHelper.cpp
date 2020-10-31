//
// Created by jian.yeung on 2020/9/23.
//

#include <stdexcept>
#include <DLog.h>
#include <ThreadHelper.h>
#include <GLErrorLog.h>
#include <android/native_window.h>
#include "EglHelper.h"

EglHelper::EglHelper() {}

EglHelper::~EglHelper() {

}

bool EglHelper::start() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "start() tid = %d", GetCurrentThreadID());
    }
    return inner_init();
}

bool EglHelper::start(EGLSurfaceType surfaceType) {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "start() surfaceType = %d, tid = %d", (int) surfaceType,
               GetCurrentThreadID());
    }
    this->eglSurfaceType = surfaceType;
    return inner_init();
}

bool EglHelper::createSurface(ANativeWindow *nativeWindow, int width, int height) {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "createSurface() tid = %d", GetCurrentThreadID());
    }
    this->nativeWindow = nativeWindow;
    bool result;
    switch (eglSurfaceType) {
        case PBUFFER_SURFACE:
            result = inner_createPbufferSurface(width, height);
            break;
        case WINDOW_SURFACE:
        default:
            result = inner_createWindowSurface(nativeWindow);
            break;
    }
    DLOGI(EGL_TAG, "createSurface() result = %d ", result);
    if (result) {
        inner_makeContext();
    } else {
        inner_doneContext();
    }
    return result;
}

void EglHelper::swap() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "swap() tid = %d", GetCurrentThreadID());
    }
    inner_swap();
}

void EglHelper::destroySurface() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "destroySurface() tid = %d", GetCurrentThreadID());
    }
    inner_destroySurface();
}

void EglHelper::release() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "release() tid = %d", GetCurrentThreadID());
    }
    inner_destroySurface();
    inner_release();
}

bool EglHelper::inner_init() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_init() tid = %d", GetCurrentThreadID());
    }
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY) {
        throw std::runtime_error("eglGetDisplay failed");
    }

    if (!eglInitialize(eglDisplay, &eglMajorVersion, &eglMinorVersion)) {
        checkEglError("eglInitialize()");
        throw std::runtime_error("eglInitialize failed");
    }

    if (eglSurfaceType == PBUFFER_SURFACE) {
        if (!eglChooseConfig(eglDisplay, pbufferConfAttr, &eglConfig, 1, &numConfigs)) {
            checkEglError("eglChooseConfig() pbufferConfAttr");
            throw std::runtime_error("eglChooseConfig() pbufferConfAttr failed");
        }
    } else {
        if (!eglChooseConfig(eglDisplay, windowConfAttr, &eglConfig, 1, &numConfigs)) {
            checkEglError("eglChooseConfig() pbufferConfAttr");
            throw std::runtime_error("eglChooseConfig() windowConfAttr failed");
        }
    }

    if (!eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxAttr)) {
        checkEglError("eglCreateContext()");
    }


    if (eglContext == nullptr || eglContext == EGL_NO_CONTEXT) {
        eglContext = nullptr;
        return false;
    }

    pfneglPresentationTimeANDROID = (PFNEGLPRESENTATIONTIMEANDROIDPROC)eglGetProcAddress("eglPresentationTimeANDROID");
    if (!pfneglPresentationTimeANDROID) {
        DLOGE(EGL_TAG, "eglPresentationTimeANDROID is not available!");
    }
    return true;
}

bool EglHelper::inner_createWindowSurface(ANativeWindow *nativeWindow) {
    if (EGL_DEBUG) {
        DFLOGI("EglHelper", "inner_createSurface() tid = %d", GetCurrentThreadID());
    }

    if (nativeWindow == nullptr) {
        throw std::runtime_error("nativeWindow not initialized");
    }

    if (eglDisplay == nullptr) {
        throw std::runtime_error("eglDisplay not initialized");
    }

    if (eglConfig == nullptr) {
        throw std::runtime_error("mEglConfig not initialized");
    }

    inner_destroySurfaceImp();

    EGLint format;
    if (!eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format)) {
        DFLOGE(EGL_TAG, "eglGetConfigAttrib() failed tid = %d", GetCurrentThreadID());
        checkEglError("eglGetConfigAttrib()");
        inner_destroySurfaceImp();
        return false;
    } else {
        DFLOGI(EGL_TAG, "eglGetConfigAttrib() Success tid = %d", GetCurrentThreadID());
    }

    ANativeWindow_setBuffersGeometry(nativeWindow, 0, 0, format);
    int surfaceAttribs[] = {
            EGL_NONE
    };

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, surfaceAttribs);
    if (eglSurface == nullptr || eglSurface == EGL_NO_SURFACE) {
        switch (eglGetError()) {
            case EGL_BAD_ALLOC:
                // Not enough resources available. Handle and recover
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG,
                           "eglCreateWindowSurface() Not enough resources available tid = %d",
                           GetCurrentThreadID());
                }
                break;
            case EGL_BAD_CONFIG:
                // Verify that provided EGLConfig is valid
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG,
                           "eglCreateWindowSurface() provided EGLConfig is invalid tid = %d",
                           GetCurrentThreadID());
                }
                break;
            case EGL_BAD_PARAMETER:
                // Verify that the EGL_WIDTH and EGL_HEIGHT are
                // non-negative values
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG,
                           "eglCreateWindowSurface() provided EGL_WIDTH and EGL_HEIGHT is invalid tid = %d",
                           GetCurrentThreadID());
                }
                break;
            case EGL_BAD_MATCH:
                // Check window and EGLConfig attributes to determine
                // compatibility and pbuffer-texture parameters
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG,
                           "eglCreateWindowSurface() Check window and EGLConfig attributes tid = %d",
                           GetCurrentThreadID());
                }
                break;
            default:
                if (eglGetError() != EGL_SUCCESS) {
                    DFLOGE(EGL_TAG, "eglCreateWindowSurface() failed tid = %d",
                           GetCurrentThreadID());
                    checkEglError("eglCreateWindowSurface()");
                }
                break;
        }
        return false;
    }

    return true;
}

bool EglHelper::inner_createPbufferSurface(int width, int height) {
    if (EGL_DEBUG) {
        DFLOGI("EglHelper", "inner_createSurface() tid = %d", GetCurrentThreadID());
    }
    if (eglDisplay == nullptr) {
        throw std::runtime_error("eglDisplay not initialized");
    }
    if (eglConfig == nullptr) {
        throw std::runtime_error("mEglConfig not initialized");
    }

    inner_destroySurfaceImp();

    EGLint surfaceAttr[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE
    };

    eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, surfaceAttr);
    if (eglSurface == nullptr || eglSurface == EGL_NO_SURFACE) {
        switch (eglGetError()) {
            case EGL_BAD_ALLOC:
                // Not enough resources available. Handle and recover
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG,
                           "eglCreateWindowSurface() Not enough resources available tid = %d",
                           GetCurrentThreadID());
                }
                break;
            case EGL_BAD_CONFIG:
                // Verify that provided EGLConfig is valid
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG,
                           "eglCreateWindowSurface() provided EGLConfig is invalid tid = %d",
                           GetCurrentThreadID());
                }
                break;
            case EGL_BAD_PARAMETER:
                // Verify that the EGL_WIDTH and EGL_HEIGHT are
                // non-negative values
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG,
                           "eglCreateWindowSurface() provided EGL_WIDTH and EGL_HEIGHT is invalid tid = %d",
                           GetCurrentThreadID());
                }
                break;
            case EGL_BAD_MATCH:
                // Check window and EGLConfig attributes to determine
                // compatibility and pbuffer-texture parameters
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG,
                           "eglCreateWindowSurface() Check window and EGLConfig attributes tid = %d",
                           GetCurrentThreadID());
                }
                break;
        }
        return false;
    }

    return true;
}

void EglHelper::inner_makeContext() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_makeContext() tid = %d", GetCurrentThreadID());
    }

    if (eglDisplay == nullptr || eglDisplay == EGL_NO_DISPLAY) {
        throw std::runtime_error("eglDisplay not initialized");
    }

    if (eglSurface == nullptr || eglSurface == EGL_NO_SURFACE) {
        throw std::runtime_error("eglSurface not initialized");
    }

    if (eglContext == nullptr || eglContext == EGL_NO_CONTEXT) {
        throw std::runtime_error("eglContext not initialized");
    }

    if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
        if (eglContext == nullptr || eglContext == EGL_NO_SURFACE) {
            checkEglError("eglMakeCurrent()");
        }
    }
}

void EglHelper::inner_doneContext() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_doneContext() tid = %d", GetCurrentThreadID());
    }

    if (eglDisplay == nullptr || eglDisplay == EGL_NO_DISPLAY) {
        throw std::runtime_error("eglDisplay not initialized");
    }

    if (!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
        checkEglError("eglMakeCurrent()");
    }
}

void EglHelper::inner_swap() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_swap() tid = %d", GetCurrentThreadID());
    }
    DFLOGE(EGL_TAG, "egl swap eglSurface = %d", (eglSurface != nullptr));
    if (eglDisplay != EGL_NO_DISPLAY && eglSurface != EGL_NO_SURFACE) {
        bool status = eglSwapBuffers(eglDisplay, eglSurface);
        DFLOGE(EGL_TAG, "egl swap status = %d", status);
        switch (eglGetError()) {
            case EGL_SUCCESS:
                break;
            case EGL_BAD_SURFACE:
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG, "egl surface is bad tid = %d", GetCurrentThreadID());
                }
                eglSurfaceIsBad = true;
                break;
            case EGL_CONTEXT_LOST:
                if (EGL_DEBUG) {
                    DFLOGE(EGL_TAG, "egl context lost tid = %d", GetCurrentThreadID());
                }
                lostEglContext = true;
                break;
            default:
                checkEglError("eglSwapBuffers()");
                break;
        }
    }
}

void EglHelper::inner_destroySurface() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_destroySurface() tid = %d", GetCurrentThreadID());
    }

    if (eglDisplay != nullptr && eglDisplay != EGL_NO_DISPLAY) {
        inner_destroySurfaceImp();
    }
}

void EglHelper::inner_destroySurfaceImp() {
    if (eglSurface != nullptr && eglSurface != EGL_NO_SURFACE) {
        if (!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
            checkEglError("eglMakeCurrent()");
        }

        if (!eglDestroySurface(eglDisplay, eglSurface)) {
            checkEglError("eglDestroySurface()");
        }
        eglSurface = nullptr;
    }
}

void EglHelper::inner_release() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_release() tid = %d", GetCurrentThreadID());
    }

    if (eglDisplay != nullptr && eglDisplay != EGL_NO_DISPLAY) {
        if (eglContext != nullptr && eglContext != EGL_NO_CONTEXT) {
            if (!eglDestroyContext(eglDisplay, eglContext)) {
                checkEglError("eglDestroyContext()");
            }
            eglContext = nullptr;
        }

        if (!eglTerminate(eglDisplay)) {
            checkEglError("eglTerminate");
        }
        eglDisplay = nullptr;
    }
}
