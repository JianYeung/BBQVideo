//
// Created by jian.yeung on 2021/1/10.
//

#ifndef BBQVIDEO_PREPAREDSTATUSLISTENER_H
#define BBQVIDEO_PREPAREDSTATUSLISTENER_H

#include <VideoPlayerDefinition.h>

class PreparedStatusListener {
public:
    virtual void onPrepared(MediaType mediaType) = 0;
};


#endif //BBQVIDEO_PREPAREDSTATUSLISTENER_H
