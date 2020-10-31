//
// Created by jian.yeung on 2020/9/8.
//

#ifndef BBQVIDEO_RTMPMANAGER_H
#define BBQVIDEO_RTMPMANAGER_H

#include <string>

using namespace std;

class RtmpManager {
private:
    bool isLiveing = false;
    bool isStreaming = false;
    string url;
    int width;
    int height;

public:
    static RtmpManager& Instance();

    void setLiveUrl(string url);

    void initVideo(int width, int height);

    bool startStream();

    bool stopStream();

    bool getLiveStatus();

    bool getStreamStatus();

    string getLiveUrl();
};


#endif //BBQVIDEO_RTMPMANAGER_H
