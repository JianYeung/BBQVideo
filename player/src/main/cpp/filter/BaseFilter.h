//
// Created by jian.yeung on 2020/9/17.
//

#ifndef BBQVIDEO_BASEFILTER_H
#define BBQVIDEO_BASEFILTER_H


#include <android/native_window.h>

enum class FilterType {
    NORMAL = 0,
    TRIANGLE = 1,
    UNKNOWN = 0xFF,
};

class BaseFilter {
protected:
    int format;
    int width;
    int height;

public:
    BaseFilter();
    ~BaseFilter();

    virtual void init();
    virtual void unInit();
    virtual void onSurfaceCreated(ANativeWindow *nativeWindow);
    virtual void onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height);
    virtual void draw();
    virtual void onDestroy();
};


#endif //BBQVIDEO_BASEFILTER_H
