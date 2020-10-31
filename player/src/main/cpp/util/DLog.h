//
// Created by jian.yeung on 2020/9/8.
//

#ifndef BBQVIDEO_DLOG_H
#define BBQVIDEO_DLOG_H

#include <android/log.h>

#define FF_LOG_TAG     "FFmpeg_Video"


#define FF_LOG_UNKNOWN        ANDROID_LOG_UNKNOWN
#define FF_LOG_DEFAULT        ANDROID_LOG_DEFAULT

#define FF_LOG_VERBOSE        ANDROID_LOG_VERBOSE
#define FF_LOG_DEBUG          ANDROID_LOG_DEBUG
#define FF_LOG_INFO           ANDROID_LOG_INFO
#define FF_LOG_WARN           ANDROID_LOG_WARN
#define FF_LOG_ERROR          ANDROID_LOG_ERROR
#define FF_LOG_FATAL          ANDROID_LOG_FATAL
#define FF_LOG_SILENT         ANDROID_LOG_SILENT

// 打印可变参数
#define VLOG(level, TAG, format, ...)    ((void)__android_log_vprint(level, TAG, format, __VA_ARGS__))
#define VLOGV(format, ...)  VLOG(FF_LOG_VERBOSE,   FF_LOG_TAG, format, __VA_ARGS__)
#define VLOGD(format, ...)  VLOG(FF_LOG_DEBUG,     FF_LOG_TAG, format, ##__VA_ARGS__)
#define VLOGI(format, ...)  VLOG(FF_LOG_INFO,      FF_LOG_TAG, format, __VA_ARGS__)
#define VLOGW(format, ...)  VLOG(FF_LOG_WARN,      FF_LOG_TAG, format, __VA_ARGS__)
#define VLOGE(format, ...)  VLOG(FF_LOG_ERROR,     FF_LOG_TAG, format, __VA_ARGS__)

#define ALOG(level, TAG, ...)    ((void)__android_log_print(level, TAG, __VA_ARGS__))
#define ALOGV(...)  ALOG(FF_LOG_VERBOSE,   FF_LOG_TAG, __VA_ARGS__)
#define ALOGD(...)  ALOG(FF_LOG_DEBUG,     FF_LOG_TAG, __VA_ARGS__)
#define ALOGI(...)  ALOG(FF_LOG_INFO,      FF_LOG_TAG, __VA_ARGS__)
#define ALOGW(...)  ALOG(FF_LOG_WARN,      FF_LOG_TAG, __VA_ARGS__)
#define ALOGE(...)  ALOG(FF_LOG_ERROR,     FF_LOG_TAG, __VA_ARGS__)

#define DLOGI(tag, ...)  __android_log_print(ANDROID_LOG_INFO, tag, ##__VA_ARGS__)
#define DLOGD(tag, ...)  __android_log_print(ANDROID_LOG_DEBUG, tag, ##__VA_ARGS__)
#define DLOGE(tag, ...)  __android_log_print(ANDROID_LOG_ERROR, tag, ##__VA_ARGS__)

#define DFLOGI(tag, format, ...)  __android_log_print(ANDROID_LOG_INFO, tag, format, ##__VA_ARGS__)
#define DFLOGD(tag, format, ...)  __android_log_print(ANDROID_LOG_DEBUG, tag, format, ##__VA_ARGS__)
#define DFLOGE(tag, format, ...)  __android_log_print(ANDROID_LOG_ERROR, tag, format, ##__VA_ARGS__)

const bool DebugEnable = true;

#endif //BBQVIDEO_DLOG_H
