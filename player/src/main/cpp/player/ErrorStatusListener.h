//
// Created by jian.yeung on 2021/1/10.
//

#ifndef BBQVIDEO_ERRORSTATUSLISTENER_H
#define BBQVIDEO_ERRORSTATUSLISTENER_H

#include <VideoPlayerDefinition.h>

class ErrorStatusListener {
public:
    virtual void onError(MediaType mediaType, int errorCode) = 0;
};

#endif //BBQVIDEO_ERRORSTATUSLISTENER_H
