//
// Created by jian.yeung on 2020/10/15.
//

#ifndef BBQVIDEO_GLRENDER_H
#define BBQVIDEO_GLRENDER_H

#include "EglHelper.h"
#include "BaseFilter.h"
#include <android/native_window.h>
#include <sys/types.h>

enum class RenderMode {
    RENDERMODE_WHEN_DIRTY = 0,
    RENDERMODE_CONTINUOUSLY = 1,
};

const bool GL_RENDER_DEBUG = true;

class GLRender {

private:
    const char* GL_RENDER_TAG = "GLRender";

    EglHelper eglHelper;
    ANativeWindow *surfaceWindow;
    int surfaceFormat;
    int surfaceWidth;
    int surfaceHeight;
    BaseFilter *filter_;
    RenderMode renderMode;
    pthread_mutex_t render_mutex;
    pthread_cond_t surface_cond;
    pthread_cond_t surface_changed_cond;
    bool isRunning = false;
    bool hasSurface = false;
    bool surfaceChanged = false;
    bool hasEglContext = false;
    bool hasEglSurface = false;

public:
    pthread_t render_thread;

public:

    GLRender();
    ~GLRender();

    void init();
    void unInit();
    void onPause();
    void onResume();
    void setRenderMode(RenderMode mode);
    void setFilter(BaseFilter *render);
    void requestRender();
    void prepareRenderThread();
    void onSurfaceCreated(ANativeWindow *window);
    void onSurfaceChanged(ANativeWindow *window, int format, int width, int height);
    void onSurfaceDestroyed(ANativeWindow *window);
    void stopEglSurfaceLocked();
    void stopEglContextLocked();
    void onDestroy();
};


#endif //BBQVIDEO_GLRENDER_H
