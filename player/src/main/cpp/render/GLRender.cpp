//
// Created by jian.yeung on 2020/10/15.
//

#include "GLRender.h"
#include <pthread.h>
#include <stdexcept>
#include <DLog.h>

GLRender::GLRender() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::GLRender()~~~~");
    }
    pthread_mutex_init(&render_mutex, nullptr);
    pthread_cond_init(&surface_cond, nullptr);
    pthread_cond_init(&surface_changed_cond, nullptr);
}

GLRender::~GLRender() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::~GLRender()~~~~");
    }
}

void GLRender::onPause() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::onPause()~~~~");
    }
    this->mRequestPaused = true;
}

void GLRender::onResume() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::onResume()~~~~");
    }
    this->mRequestPaused = false;
    this->mRequestRender = true;
}

void GLRender::onDestroy() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::onDestroy() Start~~~~");
        DFLOGD(GL_RENDER_TAG, "onDestroy isRunning = %d", isRunning);
    }
    if (isRunning) {
        requestExitAndWait();
    }

    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::onDestroy() End~~~~");
    }
}

void *guardedRun(void *data) {
    auto glRender = (GLRender *) data;
    glRender->prepareRenderThread();
    pthread_exit(&glRender->render_thread);
}

void GLRender::setRenderMode(RenderMode mode) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::setRenderMode()~~~~");
    }
    this->mRenderMode = mode;
}

RenderMode GLRender::getRenderMode() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::getRenderMode()~~~~");
    }
    return this->mRenderMode;
}

void GLRender::requestRender() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::requestRender()~~~~");
    }
    this->mRequestRender = true;
}

bool GLRender::readyToDraw() {
    if (DebugEnable && GL_RENDER_DEBUG) {
//        DFLOGI(GL_RENDER_TAG,
//               "GLRender::readyToDraw() mPaused = %d, mHasSurface = %d, mSurfaceIsBad = %d, mLostEglContext = %d, mWidth = %d, mHeight = %d, mRequestRender = %d",
//               mPaused, mHasSurface, mSurfaceIsBad, mLostEglContext, mSurfaceWidth, mSurfaceHeight,
//               mRequestRender);
    }
    return (!mPaused) && mHasSurface && (!mSurfaceIsBad) && (!mLostEglContext)
           && (mSurfaceWidth > 0) && (mSurfaceHeight > 0) &&
           (mRequestRender || mRenderMode == RenderMode::RENDERMODE_CONTINUOUSLY);
}

void GLRender::setFilter(BaseFilter *filter) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::setFilter()~~~~");
    }
    this->mFilter = filter;
    pthread_create(&render_thread, nullptr, guardedRun, this);
}

BaseFilter *GLRender::getFilter() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::getFilter()~~~~");
    }
    return this->mFilter;
}

void GLRender::prepareRenderThread() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::prepareRenderThread() start~~~~");
    }
    mEglHelper = EglHelper();
    this->isRunning = true;
    while (true) {
        pthread_mutex_lock(&render_mutex);
        if (mShouldExit) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DLOGD(GL_RENDER_TAG, "mShouldExit = true");
            }
            break;
        }
        if (!mHasSurface && !mPaused) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DLOGD(GL_RENDER_TAG, "mHasSurface = false");
            }
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
                DLOGE(GL_RENDER_TAG, "mSurfaceIsBad = true, mHasEglSurface = true");
            }
            stopEglSurfaceLocked();
        }

        if (mLostEglContext && mHasEglContext) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DLOGE(GL_RENDER_TAG, "mLostEglContext = true, mHasEglContext = true");
            }
            stopEglContextLocked();
        }

        bool pausing = false;
        if (mPaused != mRequestPaused) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DFLOGD(GL_RENDER_TAG, "mRequestPaused = %d", mPaused);
            }
            pausing = mRequestPaused;
            mPaused = mRequestPaused;
        }

        if (pausing && mHasEglSurface) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DLOGD(GL_RENDER_TAG, "pausing stopEglSurfaceLocked");
            }
            stopEglSurfaceLocked();
        }

        if (pausing && mHasEglContext) {
            if (DebugEnable && GL_RENDER_DEBUG) {
                DLOGD(GL_RENDER_TAG, "pausing stopEglContextLocked");
            }
            stopEglContextLocked();
        }

        if (readyToDraw()) {
            if (!mHasEglContext && !mHasEglSurface && !mSizeChanged) {
                pthread_cond_wait(&surface_changed_cond, &render_mutex);
            }

            if (mSizeChanged) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DLOGD(GL_RENDER_TAG, "mGLRender size changed");
                }
                if (mHasEglSurface) {
                    stopEglSurfaceLocked();
                }
            }

            if (!mHasEglContext && mHasEglSurface) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DLOGD(GL_RENDER_TAG, "mHaveEGLContext = false, mHaveEGLSurface = true");
                }
                mLostEglContext = true;
            }

            if (!mHasEglContext && !mHasEglSurface) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DLOGD(GL_RENDER_TAG, "mHaveEGLContext = false, mHaveEGLSurface = false");
                }
                if (mEglHelper.start()) {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DLOGD(GL_RENDER_TAG, "mEglHelper start success");
                    }
                    mHasEglContext = true;
                } else {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DLOGE(GL_RENDER_TAG, "mEglHelper start failed");
                    }
                    mLostEglContext = true;
                }
            }

            if (mHasEglContext && !mHasEglSurface) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DLOGD(GL_RENDER_TAG, "mHaveEGLContext = true, mHaveEGLSurface = false");
                }
                if (mEglHelper.createEglSurface(mSurfaceWindow)) {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DLOGD(GL_RENDER_TAG, "mEglHelper createEglSurface success");
                    }
                    mHasEglSurface = true;
                    if (mFilter != nullptr) {
                        if (DebugEnable && GL_RENDER_DEBUG) {
                            DLOGD(GL_RENDER_TAG, "~~~notify filter surface created~~~");
                        }
                        mFilter->onSurfaceCreated(mSurfaceWindow);
                    }
                    if (mSizeChanged) {
                        if (mFilter != nullptr) {
                            if (DebugEnable && GL_RENDER_DEBUG) {
                                DLOGD(GL_RENDER_TAG, "~~~notify filter size changed~~~");
                            }
                            mFilter->onSurfaceChanged(mSurfaceWindow, mSurfaceFormat, mSurfaceWidth, mSurfaceHeight);
                        }
                        mSizeChanged = false;
                    }
                } else {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DLOGE(GL_RENDER_TAG, "mEglHelper createEglSurface failed");
                    }
                    mSurfaceIsBad = true;
                }
            }

            if (mHasEglContext && mHasEglSurface) {
                if (DebugEnable && GL_RENDER_DEBUG) {
                    DLOGD(GL_RENDER_TAG, "mHaveEGLContext = true, mHaveEGLSurface = true");
                }
                this->mRequestRender = false;
                if (mFilter != nullptr) {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DLOGD(GL_RENDER_TAG, "~~~notify filter draw~~~");
                    }
                    mFilter->draw();
                }
                if (mEglHelper.swapBuffer()) {
                    if (DebugEnable && GL_RENDER_DEBUG) {
                        DLOGD(GL_RENDER_TAG, "mEglHelper swapBuffer success");
                    }
                } else {
                    int swapError = mEglHelper.getSwapEglError();
                    switch (swapError) {
                        case EGL_SUCCESS:
                            if (DebugEnable && GL_RENDER_DEBUG) {
                                DLOGD(GL_RENDER_TAG, "After mEglHelper swapBuffer success");
                            }
                            break;
                        case EGL_CONTEXT_LOST:
                            if (DebugEnable && GL_RENDER_DEBUG) {
                                DLOGE(GL_RENDER_TAG,
                                       "After mEglHelper swapBuffer egl context lost");
                            }
                            mLostEglContext = true;
                            break;
                        default:
                            if (DebugEnable && GL_RENDER_DEBUG) {
                                DLOGE(GL_RENDER_TAG, "mEglHelper swapBuffer egl surface is bad");
                            }
                            mSurfaceIsBad = true;
                            break;
                    }
                }
            }
        }
        pthread_mutex_unlock(&render_mutex);
    }
    this->isRunning = false;
    stopEglSurfaceLocked();
    stopEglContextLocked();
    pthread_mutex_unlock(&render_mutex);
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::prepareRenderThread() end~~~~");
    }
}

void GLRender::onSurfaceCreated(ANativeWindow *window) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::onSurfaceCreated()~~~~");
    }
    this->mHasSurface = true;
    this->mSurfaceWindow = window;
    pthread_cond_signal(&surface_cond);
}

void GLRender::onSurfaceChanged(ANativeWindow *window, int format, int width, int height) {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::onSurfaceChanged()~~~~");
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
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::onSurfaceDestroyed()~~~~");
    }
    this->mHasSurface = false;
}

void GLRender::stopEglSurfaceLocked() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::stopEglSurfaceLocked()~~~~");
    }
    if (mHasEglSurface) {
        mHasEglSurface = false;
        mEglHelper.destroyEglSurface();
    }
}

void GLRender::stopEglContextLocked() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::stopEglContextLocked()~~~~");
    }
    if (mHasEglContext) {
        mHasEglContext = false;
        mEglHelper.finish();
    }
}

void GLRender::requestExitAndWait() {
    if (DebugEnable && GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "~~~~GLRender::requestExitAndWait()~~~~");
    }
    this->mShouldExit = true;
}