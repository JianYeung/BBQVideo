//
// Created by jian.yeung on 2020/9/14.
//

#include <DLog.h>
#include "VideoDecoder.h"
void VideoDecoder::setSource(std::string s_url) {
    DLOGI("0", "s_url = %s", s_url.c_str());
    this->url = std::move(s_url);
}