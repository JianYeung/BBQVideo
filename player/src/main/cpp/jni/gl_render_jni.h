//
// Created by jian.yeung on 2020/10/25.
//

#ifndef BBQVIDEO_GL_RENDER_JNI_H
#define BBQVIDEO_GL_RENDER_JNI_H

#include <GLRender.h>
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
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "DestroyGLThreadHandle() handle = %ld", (long) gl_render_handle);
    }
    if (gl_render_handle != 0) {
        GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
        if (glRender != nullptr) {
            glRender->onDestroy();
        }
        delete glRender;
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

void SetRotation(JNIEnv *env, jobject thiz, jlong gl_render_handle, jint j_rotation) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "SetRotation() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    Rotation rotation = static_cast<Rotation>(j_rotation);
    glRender->setRotation(rotation);
}

void SetRenderMode(JNIEnv *env, jobject thiz, jlong gl_render_handle, jint render_mode) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "SetRenderMode() handle = %ld", (long) gl_render_handle);
    }
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
    glRender->setFilterType(filterType);
}

void UpdatePreviewFrame(JNIEnv *env, jobject thiz, jlong gl_render_handle, jbyteArray byteAarry, jint format, jint width, jint height) {
    if (GL_RENDER_JNI_DEBUG) {
        DFLOGI(GL_RENDER_JNI_TAG, "UpdatePreviewFrame() handle = %ld", (long) gl_render_handle);
    }
    GLRender *glRender = reinterpret_cast<GLRender *>(gl_render_handle);
    jbyte *p_byte = env->GetByteArrayElements(byteAarry, nullptr);
    unsigned char *data = reinterpret_cast<unsigned char *>(p_byte);
    glRender->updatePreviewFrame(data, format, width, height);
    //TODO 这里可能存在内存无法释放的问题，后面引入性能检测工具，并修复这个问题
    env->ReleaseByteArrayElements(byteAarry, p_byte, JNI_COMMIT);
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
        {"nativeCreateGLRenderHandle",  "()J",                           (void *) CreateGLThreadHandle},
        {"nativeDestroyGLRenderHandle", "(J)V",                          (void *) DestroyGLThreadHandle},
        {"nativeInit",                  "(J)V",                          (void *) Initialize},
        {"nativeUnInit",                "(J)V",                          (void *) UnInitialize},
        {"nativePause",                 "(J)V",                          (void *) OnPause},
        {"nativeResume",                "(J)V",                          (void *) OnResume},
        {"nativeAttachedToWindow",      "(J)V",                          (void *) OnAttachedToWindow},
        {"nativeDetachedFromWindow",    "(J)V",                          (void *) OnDetachedFromWindow},
        {"nativeSetRotation",           "(JI)V",                         (void *) SetRotation},
        {"nativeSetRenderMode",         "(JI)V",                         (void *) SetRenderMode},
        {"nativeSetFilter",             "(JI)V",                         (void *) SetFilter},
        {"nativeRequestRender",         "(J)V",                          (void *) RequestRender},
        {"nativeUpdatePreviewFrame",    "(J[BIII)V",                     (void *) UpdatePreviewFrame},
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
