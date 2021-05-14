//
// Created by jian.yeung on 2020/10/15.
//

#ifndef BBQVIDEO_GLRENDER_H
#define BBQVIDEO_GLRENDER_H

#include "EglHelper.h"
#include "BaseFilter.h"
#include <sys/types.h>

enum class RenderMode {
    RENDERMODE_WHEN_DIRTY = 0,
    RENDERMODE_CONTINUOUSLY = 1,
};

const bool GL_RENDER_DEBUG = false;

class GLRender {

private:
    const char *GL_RENDER_TAG = "GLRender";

    pthread_mutex_t render_mutex;
    pthread_cond_t surface_cond;
    pthread_cond_t surface_changed_cond;

    EglHelper mEglHelper;
    ANativeWindow *mSurfaceWindow;
    int mSurfaceFormat;
    int mSurfaceWidth;
    int mSurfaceHeight;
    BaseFilter *mFilter;
    RenderMode mRenderMode = RenderMode::RENDERMODE_WHEN_DIRTY;

    bool isRunning = false;
    bool mPaused = false;
    bool mRequestPaused = false;
    bool mShouldExit = false;
    bool mHasSurface = false;
    bool mSizeChanged = false;
    bool mHasEglContext = false;
    bool mHasEglSurface = false;
    bool mRequestRender = true;
    bool mSurfaceIsBad = false;
    bool mLostEglContext = false;

    bool readyToDraw();

public:
    pthread_t render_thread;

    GLRender();

    ~GLRender();

    void onPause();
    void onResume();
    void onDestroy();
    void setRenderMode(RenderMode mode);
    RenderMode getRenderMode();
    void setFilter(BaseFilter *filter);
    BaseFilter* getFilter();
    void requestRender();
    void prepareRenderThread();
    void onSurfaceCreated(ANativeWindow *window);
    void onSurfaceChanged(ANativeWindow *window, int format, int width, int height);
    void onSurfaceDestroyed(ANativeWindow *window);
    void stopEglSurfaceLocked();
    void stopEglContextLocked();
    void requestExitAndWait();
};


#endif //BBQVIDEO_GLRENDER_H
