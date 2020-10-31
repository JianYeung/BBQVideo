//
// Created by jian.yeung on 2020/10/27.
//

#ifndef BBQVIDEO_TRIANGLEFILTER_H
#define BBQVIDEO_TRIANGLEFILTER_H


#include "BaseFilter.h"

class TriangleFilter : public BaseFilter {
private:
    const char* TRIANGLE_FILTER_TAG = "TriangleFilter";

public:
    TriangleFilter();
    ~TriangleFilter();

    void init() override;
    void unInit() override;
    void onSurfaceCreated(ANativeWindow *nativeWindow) override;
    void onSurfaceChanged(ANativeWindow *nativeWindow, int format, int width, int height) override;
    void draw() override;
    void onDestroy() override;
};


#endif //BBQVIDEO_TRIANGLEFILTER_H
