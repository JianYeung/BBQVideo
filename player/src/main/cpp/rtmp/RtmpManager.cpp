//
// Created by jian.yeung on 2020/9/8.
//

#include "RtmpManager.h"

RtmpManager& RtmpManager::Instance() {
    static RtmpManager gInstnce;
    return gInstnce;
}

void RtmpManager::setLiveUrl(string url) {
    this->url = std::move(url);
}

string RtmpManager::getLiveUrl() {
    return std::string();
}

void RtmpManager::initVideo(int width, int height) {
    this->width = width;
    this->height = height;
}

bool RtmpManager::startStream() {
    return false;
}

bool RtmpManager::stopStream() {
    if (!isLiveing && !isStreaming) {
        return true;
    } else if (isLiveing) {

    } else if (isStreaming) {

    } else {

    }
    return false;
}

bool RtmpManager::getLiveStatus() {
    return isLiveing;
}

bool RtmpManager::getStreamStatus() {
    return isStreaming;
}



