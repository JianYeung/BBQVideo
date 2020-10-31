//
// Created by jian.yeung on 2020/10/25.
//

#ifndef BBQVIDEO_GL_RENDER_JNI_H
#define BBQVIDEO_GL_RENDER_JNI_H

#include <GLRender.h>
#include <BaseFilter.h>
#include <TriangleFilter.h>
#include <android/native_window.h>
#include <DLog.h>

const bool GL_RENDER_JNI_DEBUG = true;
const char *GL_RENDER_JNI_TAG = "GLRenderJni";

jlong CreateGLThreadHandle(JNIEnv *env, jobject thiz) {
    GLRender *glRender = new GLRender();
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "CreateGLThreadHandle() handle = %ld", (long) glRender);
    }
    return (long) glRender;
}

void DestroyGLThreadHandle(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    if (gl_render_handle != 0) {
        GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
        if (glRender != nullptr) {
            glRender->onDestroy();
        }
        delete glRender;
    }
}

void Initialize(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->init();
}

void UnInitialize(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->unInit();
}

void OnPause(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->onPause();
}

void OnResume(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    glRender->onResume();
}

void SetRenderMode(JNIEnv *env, jobject thiz, jlong gl_render_handle, jint render_mode) {
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    RenderMode renderMode = static_cast<RenderMode>(render_mode);
    glRender->setRenderMode(renderMode);
}

void SetFilter(JNIEnv *env, jobject thiz, jlong gl_render_handle, jint filter_type) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "SetFilter() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    FilterType filterType = static_cast<FilterType>(filter_type);
    BaseFilter *baseFilter;
    switch (filterType) {
        case FilterType::TRIANGLE:
            baseFilter = new TriangleFilter();
            break;
        case FilterType::NORMAL:
        default:
            baseFilter = new BaseFilter();
            break;
    }
    glRender->setFilter(baseFilter);
}

void RequestRender(JNIEnv *env, jobject thiz, jlong gl_render_handle) {
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

void
OnSurfaceChanged(JNIEnv *env, jobject thiz, jlong gl_render_handle, jobject surface, jint format,
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
        {"nativeCreateGLRenderHandle",  "()J",                           (void *) CreateGLThreadHandle},
        {"nativeDestroyGLRenderHandle", "(J)V",                          (void *) DestroyGLThreadHandle},
        {"nativeInit",                  "(J)V",                          (void *) Initialize},
        {"nativeUnInit",                "(J)V",                          (void *) UnInitialize},
        {"nativePause",                 "(J)V",                          (void *) OnPause},
        {"nativeResume",                "(J)V",                          (void *) OnResume},
        {"nativeSetRenderMode",         "(JI)V",                         (void *) SetRenderMode},
        {"nativeSetFilter",             "(JI)V",                         (void *) SetFilter},
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
