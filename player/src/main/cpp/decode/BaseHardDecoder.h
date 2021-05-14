//
// Created by jian.yeung on 2021/1/10.
//

#ifndef BBQVIDEO_BASEHARDDECODER_H
#define BBQVIDEO_BASEHARDDECODER_H

#include <cstdint>
#include <bits/timespec.h>
#include <linux/time.h>
#include <time.h>

enum {
    kMsgCodecBuffer, // 0
    kMsgPause,       // 1
    kMsgResume,      // 2
    kMsgPauseAck,    // 3
    kMsgSeek,        // 4
    kMsgCodecDone,   // 5
    kMsgReset,       // 6
};

class BaseHardDecoder {

protected:
    int64_t systemNanoTime() {
        timespec now{};
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec * 1000000000LL + now.tv_nsec;
    }
};


#endif //BBQVIDEO_BASEHARDDECODER_H
