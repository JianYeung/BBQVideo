//
// Created by jian.yeung on 2021/1/8.
//

#ifndef BBQVIDEO_VIDEOPLAYER_H
#define BBQVIDEO_VIDEOPLAYER_H

#include <string>
#include <video/VideoDecoder.h>
#include <audio/AudioDecoder.h>
#include <VideoPlayerStatusCallback.h>
#include <OnPreparedListener.h>
#include <OnErrorListener.h>

#define VIDEO_PLAYER_DEBUG true

class VideoPlayer : public OnPreparedListener, public OnErrorListener {
private:
    const char *VIDEO_PLAYER_TAG = "VideoPlayer";

    bool softwareDecode = false;
    VideoDecoder *videoDecoder;
    AudioDecoder *audioDecoder;
    VideoPlayerStatusCallback *playerStatusCallback;
    bool videoDecoderPrepared = false;
    bool audioDecoderPrepared = false;

public:
    VideoPlayer(bool softwareDecode);
    ~VideoPlayer();

    void setSurface(ANativeWindow *window, int width, int height);
    void setRender(GLRender *render);
    void setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback);
    void setDataSource(const std::string &url);
    void setCpuIds(std::vector<int> cpuIds);
    void prepare();
    void start();
    void resume();
    void pause();
    void seek(int position);
    void stop();
    void release();
    void onPrepared(MediaType mediaType) override;
    void onError(MediaType mediaType, int errorCode) override;
};


#endif //BBQVIDEO_VIDEOPLAYER_H
