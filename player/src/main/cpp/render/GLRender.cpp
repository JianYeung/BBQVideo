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
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "setRenderMode()");
    }
    this->renderMode = mode;
}

void GLRender::requestRender() {
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "requestRender()");
    }
}

void *guardedRun(void *data) {
    auto glRender = (GLRender *) data;
    glRender->prepareRenderThread();
    pthread_exit(&glRender->render_thread);
}

void GLRender::setFilter(BaseFilter *filter) {
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "setFilter()");
    }
    if (filter_ != nullptr) {
        throw std::runtime_error("setFilter has already been called for this instance.");
    }
    this->filter_ = filter;
    this->isRunning = true;
    pthread_create(&render_thread, nullptr, guardedRun, this);
}

void GLRender::prepareRenderThread() {
    eglHelper = EglHelper();
    pthread_mutex_lock(&render_mutex);
    while (isRunning) {
        if (eglHelper.eglSurfaceIsBad && hasEglSurface) {
            stopEglContextLocked();
            return;
        }

        if (eglHelper.lostEglContext && hasEglContext) {
            stopEglContextLocked();
            return;
        }

        if (!hasEglContext) {
            hasEglContext = eglHelper.start();
        }

        if (hasEglContext && !hasEglSurface) {
            if (surfaceChanged) {
                hasEglSurface = eglHelper.createSurface(surfaceWindow, surfaceWidth, surfaceHeight);
            } else {
                pthread_cond_wait(&surface_changed_cond, &render_mutex);
            }
        }

        if (hasEglSurface) {
            if (hasSurface) {
                filter_->onSurfaceCreated(surfaceWindow);
            } else {
                pthread_cond_wait(&surface_cond, &render_mutex);
            }

            if (surfaceChanged) {
                filter_->onSurfaceChanged(surfaceWindow, surfaceFormat, surfaceWidth, surfaceHeight);
            } else {
                pthread_cond_wait(&surface_changed_cond, &render_mutex);
            }

            filter_->draw();
            eglHelper.swap();
        }
    }
    stopEglSurfaceLocked();
    stopEglContextLocked();
    pthread_mutex_unlock(&render_mutex);
}

void GLRender::onSurfaceCreated(ANativeWindow *window) {
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "onSurfaceCreated()");
    }
    this->hasSurface = true;
    this->surfaceWindow = window;
    pthread_cond_signal(&surface_cond);
}

void GLRender::onSurfaceChanged(ANativeWindow *window, int format, int width, int height) {
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "onSurfaceChanged()");
    }
    this->surfaceChanged = true;
    this->surfaceWindow = window;
    this->surfaceFormat = format;
    this->surfaceWidth = width;
    this->surfaceHeight = height;
    pthread_cond_signal(&surface_changed_cond);
}

void GLRender::onSurfaceDestroyed(ANativeWindow *window) {
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "onSurfaceDestroyed()");
    }
    this->isRunning = false;
    this->hasSurface = false;
    this->surfaceChanged = false;
}

void GLRender::stopEglSurfaceLocked() {
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "stopEglSurfaceLocked()");
    }
    if (hasEglSurface) {
        hasEglSurface = false;
        eglHelper.destroySurface();
    }
}

void GLRender::stopEglContextLocked() {
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "stopEglContextLocked()");
    }
    if (hasEglContext) {
        eglHelper.release();
        hasEglContext = false;
        pthread_mutex_unlock(&render_mutex);
    }
}

void GLRender::onDestroy() {
    if (GL_RENDER_DEBUG) {
        DLOGI(GL_RENDER_TAG, "onDestroy()");
    }
    if (filter_ != nullptr) {
        filter_->onDestroy();
    }
}