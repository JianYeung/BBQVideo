//
// Created by jian.yeung on 2021/1/10.
//

#ifndef BBQVIDEO_ONPREPAREDLISTENER_H
#define BBQVIDEO_ONPREPAREDLISTENER_H

#include <VideoPlayerDefinition.h>

class OnPreparedListener {
public:
    virtual void onPrepared(MediaType mediaType) = 0;
};


#endif //BBQVIDEO_ONPREPAREDLISTENER_H
