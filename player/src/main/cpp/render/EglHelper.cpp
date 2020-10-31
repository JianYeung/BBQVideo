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
    this->mEglSurfaceType = surfaceType;
    return inner_init();
}

bool EglHelper::createEglSurface(ANativeWindow *window) {
    return createEglSurface(window, 0, 0);
}

bool EglHelper::createEglSurface(ANativeWindow *window, int width, int height) {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "createSurface() tid = %d", GetCurrentThreadID());
    }
    bool result;
    switch (mEglSurfaceType) {
        case PBUFFER_SURFACE:
            result = inner_createPbufferEglSurface(width, height);
            break;
        case WINDOW_SURFACE:
        default:
            result = inner_createWindowEglSurface(window);
            break;
    }
    DLOGI(EGL_TAG, "createSurface() result = %d ", result);
    if (result) {
        inner_makeEglContext();
    } else {
        inner_doneEglContext();
    }
    return result;
}

bool EglHelper::swapBuffer() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "swapBuffer() tid = %d", GetCurrentThreadID());
    }
    return inner_swapBuffer();
}

void EglHelper::destroyEglSurface() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "destroySurface() tid = %d", GetCurrentThreadID());
    }
    inner_destroyEglSurface();
}

void EglHelper::finish() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "finish() tid = %d", GetCurrentThreadID());
    }
    inner_destroyEglSurface();
    inner_finish();
}

bool EglHelper::inner_init() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_init() tid = %d", GetCurrentThreadID());
    }
    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mEglDisplay == EGL_NO_DISPLAY) {
        throw std::runtime_error("eglGetDisplay failed");
    }

    if (!eglInitialize(mEglDisplay, &mEglMajorVersion, &mEglMinorVersion)) {
        checkEglError("eglInitialize");
        throw std::runtime_error("eglInitialize failed");
    }

    if (mEglSurfaceType == PBUFFER_SURFACE) {
        if (!eglChooseConfig(mEglDisplay, pbufferConfAttr, &mEglConfig, 1, &numConfigs)) {
            checkEglError("eglChooseConfig() pbufferConfAttr");
            throw std::runtime_error("eglChooseConfig() pbufferConfAttr failed");
        }
    } else {
        if (!eglChooseConfig(mEglDisplay, windowConfAttr, &mEglConfig, 1, &numConfigs)) {
            checkEglError("eglChooseConfig() windowConfAttr");
            throw std::runtime_error("eglChooseConfig() windowConfAttr failed");
        }
    }

    mEglContext = eglCreateContext(mEglDisplay, mEglConfig, EGL_NO_CONTEXT, ctxAttr);
    if (mEglContext == nullptr || mEglContext == EGL_NO_CONTEXT) {
        checkEglError("eglCreateContext");
        mEglContext = nullptr;
        return false;
    }
    mEglSurface = nullptr;
    return true;
}

bool EglHelper::inner_createWindowEglSurface(ANativeWindow *nativeWindow) {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_createWindowEglSurface() tid = %d", GetCurrentThreadID());
    }

    if (nativeWindow == nullptr) {
        throw std::runtime_error("nativeWindow not initialized");
    }

    if (mEglDisplay == nullptr) {
        throw std::runtime_error("eglDisplay not initialized");
    }

    if (mEglConfig == nullptr) {
        throw std::runtime_error("mEglConfig not initialized");
    }

    inner_destroyEglSurfaceImp();

    EGLint format;
    if (!eglGetConfigAttrib(mEglDisplay, mEglConfig, EGL_NATIVE_VISUAL_ID, &format)) {
        DFLOGE(EGL_TAG, "eglGetConfigAttrib() failed tid = %d", GetCurrentThreadID());
        checkEglError("eglGetConfigAttrib");
        inner_destroyEglSurfaceImp();
        return false;
    } else {
        DFLOGI(EGL_TAG, "eglGetConfigAttrib() Success tid = %d", GetCurrentThreadID());
    }

    ANativeWindow_setBuffersGeometry(nativeWindow, 0, 0, format);
    int surfaceAttribs[] = {
            EGL_NONE
    };

    mEglSurface = eglCreateWindowSurface(mEglDisplay, mEglConfig, nativeWindow, surfaceAttribs);
    if (mEglSurface == nullptr || mEglSurface == EGL_NO_SURFACE) {
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
                    checkEglError("eglCreateWindowSurface");
                }
                break;
        }
        return false;
    }

    return true;
}

bool EglHelper::inner_createPbufferEglSurface(int width, int height) {
    if (EGL_DEBUG) {
        DFLOGI("EglHelper", "inner_createPbufferEglSurface() tid = %d", GetCurrentThreadID());
    }
    if (mEglDisplay == nullptr) {
        throw std::runtime_error("eglDisplay not initialized");
    }
    if (mEglConfig == nullptr) {
        throw std::runtime_error("mEglConfig not initialized");
    }

    inner_destroyEglSurfaceImp();

    EGLint surfaceAttr[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE
    };

    mEglSurface = eglCreatePbufferSurface(mEglDisplay, mEglConfig, surfaceAttr);
    if (mEglSurface == nullptr || mEglSurface == EGL_NO_SURFACE) {
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

void EglHelper::inner_makeEglContext() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_makeEglContext() tid = %d", GetCurrentThreadID());
    }

    if (mEglDisplay == nullptr || mEglDisplay == EGL_NO_DISPLAY) {
        throw std::runtime_error("eglDisplay not initialized");
    }

    if (mEglSurface == nullptr || mEglSurface == EGL_NO_SURFACE) {
        throw std::runtime_error("eglSurface not initialized");
    }

    if (mEglContext == nullptr || mEglContext == EGL_NO_CONTEXT) {
        throw std::runtime_error("eglContext not initialized");
    }

    if (!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
        if (mEglContext == nullptr || mEglContext == EGL_NO_SURFACE) {
            checkEglError("eglMakeCurrent");
        }
    }
}

void EglHelper::inner_doneEglContext() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_doneEglContext() tid = %d", GetCurrentThreadID());
    }

    if (mEglDisplay == nullptr || mEglDisplay == EGL_NO_DISPLAY) {
        throw std::runtime_error("eglDisplay not initialized");
    }

    if (!eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
        checkEglError("eglMakeCurrent");
    }
}

bool EglHelper::inner_swapBuffer() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_swapBuffer() tid = %d", GetCurrentThreadID());
    }
    if (mEglDisplay != EGL_NO_DISPLAY && mEglSurface != EGL_NO_SURFACE) {
        bool status = eglSwapBuffers(mEglDisplay, mEglSurface);
        if (EGL_DEBUG) {
            DFLOGD(EGL_TAG, "inner_swapBuffer() status = %d", status);
        }
        if(!status) {
            checkEglError("eglSwapBuffers");
        }
        return status;
    }
    return false;
}

bool EglHelper::hasEglSurface() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "hasEglSurface() tid = %d", GetCurrentThreadID());
    }
    bool result = mEglSurface != nullptr && mEglSurface != EGL_NO_SURFACE;
    if (EGL_DEBUG) {
        //DFLOGD(EGL_TAG, "hasEglSurface() result = %d, tid = %d", result, GetCurrentThreadID());
    }
    return result;
}

void EglHelper::inner_destroyEglSurface() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_destroyEglSurface() tid = %d", GetCurrentThreadID());
    }

    if (mEglDisplay != nullptr && mEglDisplay != EGL_NO_DISPLAY) {
        inner_destroyEglSurfaceImp();
    }
}

void EglHelper::inner_destroyEglSurfaceImp() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_destroyEglSurfaceImp() tid = %d", GetCurrentThreadID());
    }
    if (mEglSurface != nullptr && mEglSurface != EGL_NO_SURFACE) {
        if (!eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
            checkEglError("eglMakeCurrent");
        }

        if (!eglDestroySurface(mEglDisplay, mEglSurface)) {
            checkEglError("eglDestroySurface");
        }
        mEglSurface = nullptr;
    }
}

bool EglHelper::hasEglContext() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "hasEglContext() tid = %d", GetCurrentThreadID());
    }
    bool result = mEglContext != nullptr && mEglContext != EGL_NO_CONTEXT;
    if (EGL_DEBUG) {
        //DFLOGD(EGL_TAG, "hasEglContext() result = %d, tid = %d", result, GetCurrentThreadID());
    }
    return result;
}

void EglHelper::inner_finish() {
    if (EGL_DEBUG) {
        DFLOGI(EGL_TAG, "inner_finish() tid = %d", GetCurrentThreadID());
    }

    if (mEglDisplay != nullptr && mEglDisplay != EGL_NO_DISPLAY) {
        if (mEglContext != nullptr && mEglContext != EGL_NO_CONTEXT) {
            if (!eglDestroyContext(mEglDisplay, mEglContext)) {
                checkEglError("eglDestroyContext");
            }
            mEglContext = nullptr;
        }

        if (!eglTerminate(mEglDisplay)) {
            checkEglError("eglTerminate");
        }
        mEglDisplay = nullptr;
    }
}
