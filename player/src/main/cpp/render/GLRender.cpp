//
// Created by jian.yeung on 2020/10/15.
//
#include <pthread.h>
#include <stdexcept>
#include <DLog.h>
#include "GLRender.h"

GLRender::GLRender() {
    pthread_mutex_init(&render_mutex, nullptr);
    pthread_cond_init(&surface_cond, nullptr);
    pthread_cond_init(&surface_changed_cond, nullptr);
}

GLRender::~GLRender() {

}

void GLRender::init() {

}

void GLRender::unInit() {

}

void GLRender::onPause() {

}

void GLRender::onResume() {

}

void GLRender::setRenderMode(RenderMode mode) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "setRenderMode()");
    }
    this->mRenderMode = mode;
}

void GLRender::requestRender() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "requestRender()");
    }
    this->mRequestRender = true;
}

bool GLRender::readyToDraw() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DFLOGI(GL_RENDER_TAG,
               "readyToDraw mPaused = %d, mHasSurface = %ld, mSurfaceIsBad = %d, mLostEglContext = %d, mWidth = %d, mHeight = %d, mRequestRender = %d",
               mPaused, mHasSurface, mSurfaceIsBad, mLostEglContext, mSurfaceWidth, mSurfaceHeight,
               mRequestRender);
    }
    return (!mPaused) && mHasSurface && (!mSurfaceIsBad) && (!mLostEglContext)
           && (mSurfaceWidth > 0) && (mSurfaceHeight > 0) &&
           (mRequestRender || mRenderMode == RenderMode::RENDERMODE_CONTINUOUSLY);
}

void *guardedRun(void *data) {
    auto glRender = (GLRender *) data;
    glRender->prepareRenderThread();
    pthread_exit(&glRender->render_thread);
}

void GLRender::setFilter(BaseFilter *filter) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "setFilter()");
    }
    if (mFilter != nullptr) {
        throw std::runtime_error("setFilter has already been called for this instance.");
    }
    this->mFilter = filter;
    this->isRunning = true;
    pthread_create(&render_thread, nullptr, guardedRun, this);
}

void GLRender::prepareRenderThread() {
    mEglHelper = EglHelper();
    while (isRunning) {
        pthread_mutex_lock(&render_mutex);
        if (!mHasSurface) {
            pthread_cond_wait(&surface_cond, &render_mutex);
        }

        if (mEglHelper.hasEglContext()) {
//            if (DebugEnable && GL_RENDER_DEBUG) {
//                DFLOGD(GL_RENDER_TAG, "mHaveEGLContext = true");
//            }
            mHasEglContext = true;
        }

        if (mEglHelper.hasEglSurface()) {
//            if (DebugEnable && GL_RENDER_DEBUG) {
//                DFLOGD(GL_RENDER_TAG, "mHaveEGLSurface = true");
//            }
            mHasEglSurface = true;
        }

        if (mSurfaceIsBad && mHasEglSurface) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DFLOGE(GL_RENDER_TAG, "mSurfaceIsBad = true");
            }
            stopEglSurfaceLocked();
        }

        if (mLostEglContext && mHasEglContext) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DFLOGE(GL_RENDER_TAG, "mLostEglContext = true");
            }
            stopEglContextLocked();
        }

        bool pausing = false;
        if (mPaused != mRequestPaused) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DFLOGD(GL_RENDER_TAG, "mRequestPaused = true");
            }
            pausing = mRequestPaused;
            mPaused = mRequestPaused;
        }

        if (pausing && mHasEglSurface) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DFLOGD(GL_RENDER_TAG, "pausing stopEglSurfaceLocked");
            }
            stopEglSurfaceLocked();
        }

        if (pausing && mHasEglContext) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DFLOGD(GL_RENDER_TAG, "pausing stopEglContextLocked");
            }
            stopEglContextLocked();
        }

        if (readyToDraw()) {
            if (!mHasEglContext && !mHasEglSurface && !mSizeChanged) {
                pthread_cond_wait(&surface_changed_cond, &render_mutex);
            }

            if (mSizeChanged) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DFLOGD(GL_RENDER_TAG, "mGLRender resize");
                }
                if (mHasEglSurface) {
                    stopEglSurfaceLocked();
                }
            }

            if (!mHasEglContext && mHasEglSurface) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DFLOGD(GL_RENDER_TAG, "mHaveEGLContext = false, mHaveEGLSurface = true");
                }
                mLostEglContext = true;
            }

            if (!mHasEglContext && !mHasEglSurface) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DFLOGD(GL_RENDER_TAG, "mHaveEGLContext = false, mHaveEGLSurface = false");
                }
                if (mEglHelper.start()) {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DFLOGD(GL_RENDER_TAG, "mEglHelper start success");
                    }
                    mHasEglContext = true;
                    if (mFilter != nullptr) {
                        if (DebugEnable && GL_RENDER_DEBUG) {
                            DFLOGD(GL_RENDER_TAG, "Filter surface create...");
                        }
                        mFilter->onSurfaceCreated(mSurfaceWindow);
                    }
                } else {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DFLOGE(GL_RENDER_TAG, "mEglHelper start failed");
                    }
                    mLostEglContext = true;
                }
            }

            if (mHasEglContext && !mHasEglSurface) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DFLOGD(GL_RENDER_TAG, "mHaveEGLContext = true, mHaveEGLSurface = false");
                }
                if (mEglHelper.createEglSurface(mSurfaceWindow)) {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DFLOGD(GL_RENDER_TAG, "mEglHelper createEglSurface success");
                    }
                    mHasEglSurface = true;
                    if (mSizeChanged) {
                        if (mFilter != nullptr) {
                            if (DebugEnable && GL_RENDER_DEBUG) {
                                DFLOGD(GL_RENDER_TAG, "Filter size changed...");
                            }
                            mFilter->onSurfaceChanged(mSurfaceWindow, mSurfaceFormat, mSurfaceWidth, mSurfaceHeight);
                        }
                        mSizeChanged = false;
                    }
                } else {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DFLOGE(GL_RENDER_TAG, "mEglHelper createEglSurface failed");
                    }
                    mSurfaceIsBad = true;
                }
            }

            if (mHasEglContext && mHasEglSurface) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DFLOGD(GL_RENDER_TAG, "mHaveEGLContext = true, mHaveEGLSurface = true");
                }
                if (mFilter != nullptr) {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DFLOGD(GL_RENDER_TAG, "Filter draw...");
                    }
                    mFilter->draw();
                }
                if (mEglHelper.swapBuffer()) {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DFLOGD(GL_RENDER_TAG, "mEglHelper swapBuffer success");
                    }
                } else {
                    int swapError = mEglHelper.getSwapEglError();
                    switch (swapError) {
                        case EGL_SUCCESS:
                            if (DebugEnable && GL_RENDER_DEBUG) {
                                DFLOGD(GL_RENDER_TAG, "After mEglHelper swapBuffer success");
                            }
                            break;
                        case EGL_CONTEXT_LOST:
                            if (DebugEnable && GL_RENDER_DEBUG) {
                                DFLOGE(GL_RENDER_TAG,
                                       "After mEglHelper swapBuffer egl context lost");
                            }
                            mLostEglContext = true;
                            break;
                        default:
                            if (DebugEnable && GL_RENDER_DEBUG) {
                                DFLOGE(GL_RENDER_TAG, "mEglHelper swapBuffer egl surface is bad");
                            }
                            mSurfaceIsBad = true;
                            break;
                    }
                }
            }
        }
        pthread_mutex_unlock(&render_mutex);
    }
    stopEglSurfaceLocked();
    stopEglContextLocked();
    pthread_mutex_unlock(&render_mutex);
}

void GLRender::onSurfaceCreated(ANativeWindow *window) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "onSurfaceCreated()");
    }
    this->mHasSurface = true;
    this->mSurfaceWindow = window;
    pthread_cond_signal(&surface_cond);
}

void GLRender::onSurfaceChanged(ANativeWindow *window, int format, int width, int height) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "onSurfaceChanged()");
    }
    this->mSizeChanged = true;
    this->mSurfaceWindow = window;
    this->mSurfaceFormat = format;
    this->mSurfaceWidth = width;
    this->mSurfaceHeight = height;
    pthread_cond_signal(&surface_changed_cond);
}

void GLRender::onSurfaceDestroyed(ANativeWindow *window) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "onSurfaceDestroyed()");
    }
    this->mRequestPaused = true;
}

void GLRender::stopEglSurfaceLocked() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "stopEglSurfaceLocked()");
    }
    if (mHasEglSurface) {
        mHasEglSurface = false;
        mEglHelper.destroyEglSurface();
    }
}

void GLRender::stopEglContextLocked() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "stopEglContextLocked()");
    }
    if (mHasEglContext) {
        mHasEglContext = false;
        mEglHelper.finish();
    }
}

void GLRender::onDestroy() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "onDestroy()");
    }
    if (mFilter != nullptr) {
        mFilter->onDestroy();
    }
}