//
// Created by jian.yeung on 2021/1/8.
//

#include <video/VideoSoftDecoder.h>
#include <video/VideoHardDecoder.h>
#include <audio/AudioSoftDecoder.h>
#include <audio/AudioHardDecoder.h>

#include <utility>
#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(bool softwareDecode) : playerStatusCallback(nullptr) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::VideoPlayer()~~~\n");
        DFLOGD(VIDEO_PLAYER_TAG, "VideoPlayer::VideoPlayer() softwareDecode  = %d", softwareDecode);
    }
    this->softwareDecode = softwareDecode;
    if (softwareDecode) {
        videoDecoder = new VideoSoftDecoder();
        audioDecoder = new AudioSoftDecoder();
    } else {
        videoDecoder = new VideoHardDecoder();
        audioDecoder = new AudioHardDecoder();
    }
    if (videoDecoder != nullptr) {
        videoDecoder->setOnPreparedListener(this);
    }

    if (audioDecoder != nullptr) {
        audioDecoder->setOnPreparedListener(this);
    }
}

VideoPlayer::~VideoPlayer() {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::~VideoPlayer()~~~\n");
    }
}

void VideoPlayer::setSurface(ANativeWindow *window, int width, int height) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::setSurface()~~~\n");
    }
    if (videoDecoder != nullptr) {
        videoDecoder->setSurface(window, width, height);
    }
}

void VideoPlayer::setRender(GLRender *render) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::setRender()~~~\n");
    }
    if (render == nullptr) {
        DLOGE(VIDEO_PLAYER_TAG, "Invalid Render");
        return;
    }
    if (videoDecoder != nullptr) {
        videoDecoder->setRender(render);
    }
}

void VideoPlayer::setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::setPlayerStatusCallback()~~~\n");
    }
    this->playerStatusCallback = playerStatusCallback;
    if (videoDecoder != nullptr) {
        videoDecoder->setPlayerStatusCallback(playerStatusCallback);
    }
    if (audioDecoder != nullptr) {
        audioDecoder->setPlayerStatusCallback(playerStatusCallback);
    }
}

void VideoPlayer::setDataSource(const std::string &url) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::setDataSource()~~~\n");
        DFLOGD(VIDEO_PLAYER_TAG, "VideoPlayer::setDataSource() video url = %s", url.data());
    }
    if (videoDecoder != nullptr) {
        videoDecoder->setDataSource(url);
    }
    if (audioDecoder != nullptr) {
        audioDecoder->setDataSource(url);
    }
}

void VideoPlayer::setCpuIds(std::vector<int> cpuIds) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::setDataSource()~~~\n");
    }
    if (audioDecoder != nullptr) {
        audioDecoder->setCpuIds(std::move(cpuIds));
    }
}

void VideoPlayer::prepare() {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::prepare()~~~\n");
    }
    videoDecoderPrepared = false;
    audioDecoderPrepared = false;
    if (videoDecoder != nullptr) {
        videoDecoder->prepare();
    }
    if (audioDecoder != nullptr) {
        audioDecoder->prepare();
    }
}

void VideoPlayer::start() {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::start()~~~\n");
    }
    if (videoDecoder != nullptr) {
        videoDecoder->start();
    }
    if (audioDecoder != nullptr) {
        audioDecoder->start();
    }
}

void VideoPlayer::pause() {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::pause()~~~\n");
    }
    if (videoDecoder != nullptr) {
        videoDecoder->pause();
    }
    if (audioDecoder != nullptr) {
        audioDecoder->pause();
    }
}

void VideoPlayer::resume() {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::resume()~~~\n");
    }
    if (videoDecoder != nullptr) {
        videoDecoder->resume();
    }
    if (audioDecoder != nullptr) {
        audioDecoder->resume();
    }
}

void VideoPlayer::seek(int position) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::seek()~~~\n");
        DFLOGD(VIDEO_PLAYER_TAG, "VideoPlayer::seek() position = %d", position);
    }
    if (videoDecoder != nullptr) {
        videoDecoder->seek(position);
    }
    if (audioDecoder != nullptr) {
        audioDecoder->seek(position);
    }
}

void VideoPlayer::stop() {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::stop()~~~\n");
    }
    if (videoDecoder != nullptr) {
        videoDecoder->stop();
    }
    if (audioDecoder != nullptr) {
        audioDecoder->stop();
    }
}

void VideoPlayer::release() {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::release() Start~~~\n");
    }

    if (softwareDecode) {
        if (videoDecoder != nullptr) {
            videoDecoder->release();
            delete ((VideoSoftDecoder *) videoDecoder);
            videoDecoder = nullptr;
        }
        if (audioDecoder != nullptr) {
            audioDecoder->release();
            delete ((AudioSoftDecoder *) audioDecoder);
            audioDecoder = nullptr;
        }
    } else {
        if (videoDecoder != nullptr) {
            videoDecoder->release();
            delete ((VideoHardDecoder *) videoDecoder);
            videoDecoder = nullptr;
        }
        if (audioDecoder != nullptr) {
            audioDecoder->release();
            delete ((AudioHardDecoder *) audioDecoder);
            audioDecoder = nullptr;
        }
    }

    if (playerStatusCallback != nullptr) {
        playerStatusCallback->release();
        delete playerStatusCallback;
        playerStatusCallback = nullptr;
    }

    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::release() End~~~\n");
    }
}

void VideoPlayer::onPrepared(MediaType mediaType) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::onPrepared()~~~\n");
        DFLOGD(VIDEO_PLAYER_TAG, "VideoPlayer::onPrepared() mediaType = %d", mediaType);
    }
    switch (mediaType) {
        case Video:
            videoDecoderPrepared = true;
            break;
        case Audio:
            audioDecoderPrepared = true;
            break;
        default:
            break;
    }
    DFLOGD(VIDEO_PLAYER_TAG, "VideoPlayer::onPrepared() videoDecoderPrepared = %d, audioDecoderPrepared = %d", videoDecoderPrepared, audioDecoderPrepared);
    if (videoDecoderPrepared && audioDecoderPrepared) {
        if (playerStatusCallback != nullptr) {
            playerStatusCallback->onPrepared();
        }
    }
}

void VideoPlayer::onError(MediaType mediaType, int errorCode) {
    if (DebugEnable && VIDEO_PLAYER_DEBUG) {
        DLOGI(VIDEO_PLAYER_TAG, "~~~VideoPlayer::onError()~~~\n");
        DFLOGD(VIDEO_PLAYER_TAG, "VideoPlayer::onError() mediaType = %d", mediaType);
    }
    if (playerStatusCallback != nullptr) {
        playerStatusCallback->onError(errorCode);
    }
}

