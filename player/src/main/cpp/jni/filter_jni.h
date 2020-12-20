//
// Created by jian.yeung on 2020/12/16.
//

#ifndef BBQVIDEO_FILTER_JNI_H
#define BBQVIDEO_FILTER_JNI_H


#include <DLog.h>
#include <TriangleFilter.h>
#include <CameraFilter.h>
#include <VideoFilter.h>

const bool FILTER_JNI_DEBUG = false;
const char *FILTER_JNI_TAG = "FilterJni";
FilterType filterType = FilterType::NORMAL;

jlong CreateFilterHandle(JNIEnv *env, jobject thiz, jint filter_type) {
    if (FILTER_JNI_DEBUG) {
        DFLOGD(FILTER_JNI_TAG, "CreateFilterHandle() filter type = %d", filter_type);
    }
    BaseFilter *filter = nullptr;
    filterType = static_cast<FilterType>(filter_type);
    switch (filterType) {
        case FilterType::TRIANGLE:
            filter = new TriangleFilter();
            break;
        case FilterType::CAMERA:
            filter = new CameraFilter();
            break;
        case FilterType::VIDEO:
            filter = new VideoFilter();
            break;
        case FilterType::NORMAL:
        default:
            filter = new BaseFilter();
            break;
    }
    if (FILTER_JNI_DEBUG) {
        DFLOGI(FILTER_JNI_TAG, "CreateFilterHandle() handle = %ld", (long) filter);
    }
    return (long) filter;
}

void DestroyFilterHandle(JNIEnv *env, jobject thiz, jlong filter_handle) {
    if (FILTER_JNI_DEBUG) {
        DFLOGI(FILTER_JNI_TAG, "DestroyFilterHandle() handle = %ld", (long) filter_handle);
    }
    if (filter_handle != 0) {
        if (FILTER_JNI_DEBUG) {
            DFLOGD(FILTER_JNI_TAG, "DestroyFilterHandle() filter type = %d", (int) filterType);
        }
        BaseFilter *filter = reinterpret_cast<BaseFilter *> (filter_handle);;
        if (filter != nullptr) {
            filter->onDestroy();
            switch (filterType) {
                case FilterType::TRIANGLE:
                    delete ((TriangleFilter*) filter);
                    break;
                case FilterType::CAMERA:
                    delete ((CameraFilter*) filter);
                    break;
                case FilterType::VIDEO:
                    delete ((VideoFilter*) filter);
                    break;
                case FilterType::NORMAL:
                default:
                    delete ((BaseFilter*) filter);
                    break;
            }
            filter = nullptr;
        }
    }
}

void UpdatePreviewFrame(JNIEnv *env, jobject thiz, jlong filter_handle, jbyteArray byteAarry,
                        jint format, jint width, jint height) {
    if (FILTER_JNI_DEBUG) {
        DFLOGI(FILTER_JNI_TAG, "UpdatePreviewFrame() handle = %ld", (long) filter_handle);
    }
    if (filter_handle != 0) {
        BaseFilter *baseFilter = reinterpret_cast<BaseFilter *>(filter_handle);
        jbyte *p_byte = env->GetByteArrayElements(byteAarry, nullptr);
        unsigned char *data = reinterpret_cast<unsigned char *>(p_byte);
        VideoFrame frame = VideoFrame();
        frame.updateFrameInfo(data, format, width, height);
        baseFilter->updatePreviewFrame(&frame);
        env->ReleaseByteArrayElements(byteAarry, p_byte, JNI_FALSE);
    }
}

static JNINativeMethod gFilterMethods[] = {
        {"nativeCreateFilterHandle",  "(I)J",      (void *) CreateFilterHandle},
        {"nativeDestroyFilterHandle", "(J)V",      (void *) DestroyFilterHandle},
        {"nativeUpdatePreviewFrame",  "(J[BIII)V", (void *) UpdatePreviewFrame},
};

static jint registerNativeFilterMethods(JNIEnv *env) {
    jclass clazz = env->FindClass("com/yj/player/jni/FilterHelper");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gFilterMethods,
                             sizeof(gFilterMethods) / sizeof(gFilterMethods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_OK;
}

#endif //BBQVIDEO_FILTER_JNI_H
