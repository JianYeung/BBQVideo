//
// Created by jian.yeung on 2020/10/25.
//

#ifndef BBQVIDEO_GL_RENDER_JNI_H
#define BBQVIDEO_GL_RENDER_JNI_H

#include <GLRender.h>
#include <android/native_window.h>
#include <DLog.h>

const bool GL_RENDER_JNI_DEBUG = false;
const char *GL_RENDER_JNI_TAG = "GLRenderJni";

jlong CreateGLRenderHandle(JNIEnv *env, jobject thiz) {
    GLRender *glRender = new GLRender();
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "CreateGLRenderHandle() handle = %ld", (long) glRender);
    }
    return (long) glRender;
}

void DestroyGLRenderHandle(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "DestroyGLRenderHandle() handle = %ld", (long) gl_render_handle);
    }
    if (gl_render_handle != 0) {
        GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
        if (glRender != nullptr) {
            glRender->onDestroy();
        }
        delete glRender;
        glRender = nullptr;
    }
}

void Initialize(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "Initialize() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->init();
}

void UnInitialize(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "UnInitialize() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->unInit();
}

void OnPause(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "OnPause() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->onPause();
}

void OnResume(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "OnResume() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->onResume();
}

void OnAttachedToWindow(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "OnAttachedToWindow() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->onAttachedToWindow();
}

void OnDetachedFromWindow(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "OnDetachedFromWindow() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->onDetachedFromWindow();
}

void SetRenderMode(JNIEnv *env, jobject thiz, jlong gl_render_handle, jint render_mode) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "SetRenderMode() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    RenderMode renderMode = static_cast<RenderMode>(render_mode);
    glRender->setRenderMode(renderMode);
}

void SetFilter(JNIEnv *env, jobject thiz, jlong gl_render_handle, jlong filter_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "SetFilter() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    BaseFilter *filter = reinterpret_cast<BaseFilter *>(filter_handle);
    glRender->setFilter(filter);
}

void RequestRender(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "RequestRender() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->requestRender();
}

void OnSurfaceCreated(JNIEnv *env, jobject thiz, jlong gl_render_handle, jobject surface) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "OnSurfaceCreated() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    glRender->onSurfaceCreated(nativeWindow);
}

void OnSurfaceChanged(JNIEnv *env, jobject thiz, jlong gl_render_handle, jobject surface, jint format,
                 jint width, jint height) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "OnSurfaceChanged() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    glRender->onSurfaceChanged(nativeWindow, format, width, height);
}

void OnSurfaceDestroyed(JNIEnv *env, jobject thiz, jlong gl_render_handle, jobject surface) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "OnSurfaceDestroyed() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    glRender->onSurfaceDestroyed(nativeWindow);
}

static JNINativeMethod gGLRenderMethods[] = {
        {"nativeCreateGLRenderHandle",  "()J",                           (void *) CreateGLRenderHandle},
        {"nativeDestroyGLRenderHandle", "(J)V",                          (void *) DestroyGLRenderHandle},
        {"nativeInit",                  "(J)V",                          (void *) Initialize},
        {"nativeUnInit",                "(J)V",                          (void *) UnInitialize},
        {"nativePause",                 "(J)V",                          (void *) OnPause},
        {"nativeResume",                "(J)V",                          (void *) OnResume},
        {"nativeAttachedToWindow",      "(J)V",                          (void *) OnAttachedToWindow},
        {"nativeDetachedFromWindow",    "(J)V",                          (void *) OnDetachedFromWindow},
        {"nativeSetRenderMode",         "(JI)V",                         (void *) SetRenderMode},
        {"nativeSetFilter",             "(JJ)V",                         (void *) SetFilter},
        {"nativeRequestRender",         "(J)V",                          (void *) RequestRender},
        {"nativeSurfaceCreated",        "(JLandroid/view/Surface;)V",    (void *) OnSurfaceCreated},
        {"nativeSurfaceChanged",        "(JLandroid/view/Surface;III)V", (void *) OnSurfaceChanged},
        {"nativeSurfaceDestroyed",      "(JLandroid/view/Surface;)V",    (void *) OnSurfaceDestroyed},
};

static jint registerNativeGLRenderMethods(JNIEnv *env) {
    jclass clazz = env->FindClass("com/yj/player/jni/GLRenderHelper");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gGLRenderMethods,
                             sizeof(gGLRenderMethods) / sizeof(gGLRenderMethods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_OK;
}


#endif //BBQVIDEO_GL_RENDER_JNI_H
