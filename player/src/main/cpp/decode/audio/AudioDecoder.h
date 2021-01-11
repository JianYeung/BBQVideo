//
// Created by jian.yeung on 2021/1/9.
//

#ifndef BBQVIDEO_AUDIODECODER_H
#define BBQVIDEO_AUDIODECODER_H

#include <string>
#include <VideoPlayerStatusCallback.h>
#include <PreparedStatusListener.h>
#include <ErrorStatusListener.h>

#define AUDIO_DECODER_DEBUG true
#define AUDIO_DECODER_TAG "AudioDecoder"

enum AudioDecodeState {
    Idle,         // 0
    Initialized,  // 1
    Preparing,    // 2
    Prepared,     // 3
    Starting,     // 4
    Started,      // 5
    Pausing,      // 6
    Paused,       // 7
    Stopping,     // 8
    Stopped,      // 9
    Completed,    // 10
    End,          // 11
    Error,        // 12
};

class AudioDecoder {
protected:
    AudioDecodeState decodeState = AudioDecodeState::Idle;
    std::string audioUrl;
    VideoPlayerStatusCallback *playerStatusCallback;
    PreparedStatusListener *preparedStatusListener;
    ErrorStatusListener *errorStatusListener;

public:
    AudioDecoder();
    virtual ~AudioDecoder();

    virtual void setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) = 0;
    virtual void setPreparedStatusListener(PreparedStatusListener *preparedStatusListener) = 0;
    virtual void setErrorStatusListener(ErrorStatusListener *errorStatusListener) = 0;
    virtual void setDataSource(std::string url) = 0;
    virtual void prepare() = 0;
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void seek(int position) = 0;
    virtual void stop() = 0;
    virtual void release() = 0;
};


#endif //BBQVIDEO_AUDIODECODER_H
