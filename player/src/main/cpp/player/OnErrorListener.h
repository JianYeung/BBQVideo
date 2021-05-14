//
// Created by jian.yeung on 2021/1/10.
//

#ifndef BBQVIDEO_ONERRORLISTENER_H
#define BBQVIDEO_ONERRORLISTENER_H

#include <VideoPlayerDefinition.h>

class OnErrorListener {
public:
    virtual void onError(MediaType mediaType, int errorCode) = 0;
};

#endif //BBQVIDEO_ONERRORLISTENER_H
