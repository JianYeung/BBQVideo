//
// Created by jian.yeung on 2021/1/9.
//

#include <unistd.h>
#include "AudioHardDecoder.h"

AudioHardDecoder::AudioHardDecoder() : AudioDecoder(), extractor(nullptr),
                                       codec(nullptr), format(nullptr) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::AudioHardDecoder()~~~\n");
    }
    decodeHandler = new AudioDecodeHandler();
}

AudioHardDecoder::~AudioHardDecoder() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::~AudioHardDecoder()~~~\n");
    }
}

void AudioHardDecoder::setPlayerStatusCallback(VideoPlayerStatusCallback *playerStatusCallback) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::setPlayerStatusCallback()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::setPlayerStatusCallback() decodeState = %d", decodeState);
    }
    this->playerStatusCallback = playerStatusCallback;
}

void AudioHardDecoder::setPreparedStatusListener(PreparedStatusListener *preparedStatusListener) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::setPreparedStatusListener()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::setPreparedStatusListener() decodeState = %d", decodeState);
    }
    this->preparedStatusListener = preparedStatusListener;
}

void AudioHardDecoder::setErrorStatusListener(ErrorStatusListener *errorStatusListener) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::setErrorStatusListener()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::setErrorStatusListener() decodeState = %d", decodeState);
    }
    this->errorStatusListener = errorStatusListener;
}

void AudioHardDecoder::setDataSource(std::string url) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::setDataSource()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::setDataSource() decodeState = %d", decodeState);
    }
    if (url.empty()) {
        DLOGE(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::setDataSource() url is empty");
        return;
    }

    if (decodeState == Idle && !url.empty()) {
        forceTransformState(Initialized);
    }
    this->audioUrl = std::move(url);
}

void AudioHardDecoder::prepare() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::prepare()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::prepare() decodeState = %d", decodeState);
    }
    restartCodec();
}

void AudioHardDecoder::start() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::start()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::start() decodeState = %d", decodeState);
    }

    if (decodeState == Started) {
        DLOGE(AUDIO_HARD_DECODER_TAG, "Video is in playing state, please stop to play first!!!");
        return;
    }

    if (!audioUrl.empty()) {
        if (decodeState == Prepared) {
            forceTransformState(Starting);
            sendMessage(kMsgCodecBuffer);
        } else if (decodeState == Initialized
                   || decodeState == Stopped) {
            forceTransformState(Preparing);
            if (initCodec()) {
                if (decodeState == Prepared) {
                    forceTransformState(Starting);
                    sendMessage(kMsgCodecBuffer);
                }
            } else {
                DLOGE(AUDIO_HARD_DECODER_TAG, "Init Audio Codec Failed!!!");
            }
        } else {
            DLOGE(AUDIO_HARD_DECODER_TAG, "Audio Codec cannot be initialized in this state!!!");
        }
    } else {
        DLOGE(AUDIO_HARD_DECODER_TAG, "Has not audio url!!!");
    }
}

void AudioHardDecoder::pause() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::pause()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::pause() decodeState = %d", decodeState);
    }
    sendMessage(kMsgPause);
}

void AudioHardDecoder::resume() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::resume()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::resume() decodeState = %d", decodeState);
    }
    sendMessage(kMsgResume);
}

void AudioHardDecoder::seek(int position) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::seek()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::seek() decodeState = %d", decodeState);
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::seek() position = %d", position);
    }
    if (position < 0 || position > 100) {
        DLOGE(AUDIO_HARD_DECODER_TAG, "seek position is illegal");
        return;
    }

    Message msg = Message();
    msg.what = kMsgSeek;
    msg.arg1 = position;
    sendMessage(msg, true);
}

void AudioHardDecoder::stop() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::stop()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::stop() decodeState = %d", decodeState);
    }
    forceTransformState(Stopping);
    releaseCodec();
    sendMessage(kMsgCodecDone, true);
}

void AudioHardDecoder::release() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::release()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::release() decodeState = %d", decodeState);
    }
    if (decodeHandler != nullptr) {
        decodeHandler->quit();
        delete decodeHandler;
        decodeHandler = nullptr;
    }
    forceTransformState(Stopping);
    releaseCodec();
    forceTransformState(End);
}

bool AudioHardDecoder::initCodec() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::initCodec() Start~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::initCodec() decodeState = %d", decodeState);
    }

    if (extractor == nullptr) {
        extractor = AMediaExtractor_new();
    }

    int result = AMediaExtractor_setDataSource(extractor, audioUrl.data());
    DFLOGE(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::initCodec() AMediaExtractor_setDataSource result = %d", result);
    if (result != AMEDIA_OK) {
        DLOGE(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::initCodec() setDataSource failed!!!");
        forceTransformState(Error);
        return false;
    }

    if (extractor == nullptr) {
        //防止其他线程把extractor析构，而AMediaExtractor_setDataSource回调回来很慢
        return false;
    }

    int numTracks = AMediaExtractor_getTrackCount(extractor);
    const char *mime = nullptr;
    for (int i = 0; i < numTracks; i++) {
        format = AMediaExtractor_getTrackFormat(extractor, i);
        const char *s = AMediaFormat_toString(format);
        DFLOGD(AUDIO_HARD_DECODER_TAG, "Get track %d format: %s", i, s);
        AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mime);
        if (mime != nullptr) {
            if (!strncmp(mime, "audio/", 6)) {
                audioTrackIndex = i;
                int64_t durationUs;
                AMediaFormat_getInt64(format, AMEDIAFORMAT_KEY_DURATION, &durationUs);
                outDuration = (long) (durationUs / 1000000);
                DFLOGD(AUDIO_HARD_DECODER_TAG, "audio duration %lld us", (long long) durationUs);
                break;
            }
        } else {
            DLOGE(AUDIO_HARD_DECODER_TAG, "Has no mime type!!!");
        }
    }

    if (audioTrackIndex == -1) {
        DLOGE(AUDIO_HARD_DECODER_TAG, "Not find audio track in the video!!!");
        if (format != nullptr) {
            AMediaFormat_delete(format);
            format = nullptr;
        }
        return false;
    }

    // Omitting most error handling for clarity.
    // Production code should check for errors.
    AMediaExtractor_selectTrack(extractor, audioTrackIndex);
    codec = AMediaCodec_createDecoderByType(mime);
    AMediaCodec_configure(codec, format, nullptr, nullptr, 0);
    result = AMediaCodec_start(codec);
    if (result == AMEDIA_OK) {
        forceTransformState(Prepared);
        renderStart = -1;
        sawInputEOS = false;
        sawOutputEOS = false;
        if (preparedStatusListener != nullptr) {
            preparedStatusListener->onPrepared(MediaType::Audio);
        }
    } else {
        DLOGE(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::initCodec() start codec failed!!!");
        if (format != nullptr) {
            AMediaFormat_delete(format);
            format = nullptr;
        }
        return false;
    }

    if (format != nullptr) {
        AMediaFormat_delete(format);
        format = nullptr;
    }
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::initCodec() End~~~\n");
    }
    return true;
}

void AudioHardDecoder::releaseCodec() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::releaseCodec() Start~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::releaseCodec() decodeState = %d", decodeState);
    }
    sawInputEOS = true;
    sawOutputEOS = true;
    if (format != nullptr) {
        AMediaFormat_delete(format);
        format = nullptr;
    }
    if (codec != nullptr) {
        AMediaCodec_stop(codec);
        AMediaCodec_delete(codec);
        codec = nullptr;
    }
    if (extractor != nullptr) {
        AMediaExtractor_delete(extractor);
        extractor = nullptr;
    }
    forceTransformState(Stopped);
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~VideoHardDecoder::releaseCodec() End~~~\n");
    }
}

void AudioHardDecoder::restartCodec() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::restartCodec()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::restartCodec() decodeState = %d", decodeState);
    }
    if (!audioUrl.empty()) {
        switch (decodeState) {
            case Prepared:
            case Started:
            case Paused:
            case Stopped:
            case Completed:
            case Error:
                forceTransformState(Stopping);
                releaseCodec();
                break;
            case Preparing:
            case Starting:
            case Stopping:
                DLOGE(AUDIO_HARD_DECODER_TAG, "Audio decoder is in transition state");
                return;
            default:
                break;

        }
        forceTransformState(Preparing);
        if (!initCodec()) {
            DLOGE(AUDIO_HARD_DECODER_TAG, "Audio Codec init failed");
        }
    } else {
        DLOGE(AUDIO_HARD_DECODER_TAG, "Has not audio url!!!");
    }
}

void AudioHardDecoder::doDecodeWork() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::doDecodeWork() Start~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::doDecodeWork() Start decodeState = %d", decodeState);
    }

    if (decodeState != Starting && decodeState != Started) {
        DLOGE(AUDIO_HARD_DECODER_TAG, "doDecodeWork() cannot be executed in this state!!!");
        return;
    }

    if (!sawInputEOS) {
        ssize_t bufIdx = AMediaCodec_dequeueInputBuffer(codec, 2000);
        if (DebugEnable && AUDIO_DECODER_DEBUG) {
            DFLOGD(AUDIO_HARD_DECODER_TAG, "input buffer index: %zd", bufIdx);
        }
        if (bufIdx >= 0) {
            size_t bufSize;
            auto buf = AMediaCodec_getInputBuffer(codec, bufIdx, &bufSize);
            auto sampleSize = AMediaExtractor_readSampleData(extractor, buf, bufSize);
            if (sampleSize < 0) {
                sampleSize = 0;
                sawInputEOS = true;
                DLOGE(AUDIO_HARD_DECODER_TAG, "input EOS");
            }
            auto presentationTimeUs = AMediaExtractor_getSampleTime(extractor);
            AMediaCodec_queueInputBuffer(codec, bufIdx, 0, sampleSize, presentationTimeUs,
                                         sawInputEOS ? AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM : 0);
            AMediaExtractor_advance(extractor);
        }
    }

    if (!sawOutputEOS) {
        AMediaCodecBufferInfo info;
        auto status = AMediaCodec_dequeueOutputBuffer(codec, &info, 0);
        if (status >= 0) {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DFLOGD(AUDIO_HARD_DECODER_TAG, "output buffer index: %zd", status);
            }

            if (info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
                DLOGE(AUDIO_HARD_DECODER_TAG, "output EOS");
                sawOutputEOS = true;
            }

            uint8_t *outputBuf = AMediaCodec_getOutputBuffer(codec, status, nullptr/* out_size */);
            size_t dataSize = info.size;
            if (outputBuf != nullptr && dataSize != 0) {
                forceTransformState(Started);
                long pts = info.presentationTimeUs;
                if (DebugEnable && AUDIO_DECODER_DEBUG) {
                    DFLOGD(AUDIO_HARD_DECODER_TAG, "output buffer pts: %ld", pts);
                }
                int64_t presentationNano = pts * 1000;
                if (renderStart < 0) {
                    renderStart = systemNanoTime() - presentationNano;
                }
                AMediaCodec_releaseOutputBuffer(codec, status, info.size != 0);
                int64_t delay = (renderStart + presentationNano) - systemNanoTime();
                if (DebugEnable && AUDIO_DECODER_DEBUG) {
                    int time = delay > 0 ? (int) delay : 0;
                    DFLOGD(AUDIO_HARD_DECODER_TAG, "After play audio, decoder sleep %d us", time);
                }
                if (delay > 0) {
                    usleep(delay / 1000);
                }
            }
        } else if (status == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DLOGD(AUDIO_HARD_DECODER_TAG, "output buffers changed");
            }
        } else if (status == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
            format = AMediaCodec_getOutputFormat(codec);
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DFLOGD(AUDIO_HARD_DECODER_TAG, "format changed to: %s", AMediaFormat_toString(format));
            }
            AMediaFormat_delete(format);
            format = nullptr;
        } else if (status == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DLOGD(AUDIO_HARD_DECODER_TAG, "no output buffer right now");
            }
        } else {
            forceTransformState(Error);
            DFLOGE(AUDIO_HARD_DECODER_TAG, "unexpected info code: %zd", status);
        }
    }

    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGD(AUDIO_HARD_DECODER_TAG, "sawInputEOS = %d, sawOutputEOS = %d", sawInputEOS, sawOutputEOS);
    }

    if (!sawInputEOS || !sawOutputEOS) {
        sendMessage(kMsgCodecBuffer);
    }

    if (sawInputEOS && sawOutputEOS) {
        if (decodeState != Completed)  {
            forceTransformState(Completed);
        } else {
            if (extractor != nullptr && codec != nullptr) {
                AMediaExtractor_seekTo(extractor, 0, AMEDIAEXTRACTOR_SEEK_NEXT_SYNC);
                AMediaCodec_flush(codec);
            }
            renderStart = -1;
            sawInputEOS = false;
            sawOutputEOS = false;
        }
    }

    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::doDecodeWork() End~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::doDecodeWork() End decodeState = %d", decodeState);
    }
}

void AudioHardDecoder::doPauseWork() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::doPauseWork()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::doPauseWork() decodeState = %d", decodeState);
    }
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::doPauseWork() Start~~~\n");
    }
    if (decodeState == Started) {
        forceTransformState(Paused);
        // flush all outstanding codecbuffer messages with a no-op message
        sendMessage(kMsgPauseAck, true);
    }
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::doPauseWork() End~~~\n");
    }
}

void AudioHardDecoder::doResumeWork() {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::doResumeWork()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::doResumeWork() decodeState = %d", decodeState);
    }

    if (decodeState == Paused) {
        forceTransformState(Starting);
        renderStart = -1;
        sendMessage(kMsgCodecBuffer);
    }
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~VideoHardDecoder::doResumeWork() End~~~\n");
    }

}

void AudioHardDecoder::doSeekWork(int position) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::doSeekWork()~~~\n");
        DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::doSeekWork() decodeState = %d", decodeState);
    }
    if (outDuration < 0) {
        DLOGE(AUDIO_HARD_DECODER_TAG, "doSeekWork() please getDuration first!!!");
        return;
    }
    int64_t seekPosition = (outDuration * position * 1000000 / 100);
    DFLOGD(AUDIO_HARD_DECODER_TAG, "seek position to %ld us\n", (long) seekPosition);

    if (decodeState == Prepared
        || decodeState == Started
        || decodeState == Paused
        || decodeState == Completed) {
        forceTransformState(Starting);
        renderStart = -1;
        AMediaExtractor_seekTo(extractor, seekPosition, AMEDIAEXTRACTOR_SEEK_CLOSEST_SYNC);
        AMediaCodec_flush(codec);
        sendMessage(kMsgCodecBuffer, true);
    } else {
        DLOGE(AUDIO_HARD_DECODER_TAG, "doSeekWork() cannot be executed in this state!!!");
    }
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::doSeekWork() End~~~\n");
    }
}

void AudioHardDecoder::forceTransformState(AudioDecodeState targetState) {
    if (decodeState != targetState) {
        if (DebugEnable && AUDIO_DECODER_DEBUG) {
            DFLOGD(AUDIO_HARD_DECODER_TAG, "AudioHardDecoder::forceTransformState() source: %d to target: %d", decodeState, targetState);
        }
        decodeState = targetState;
    }
}

void AudioHardDecoder::sendMessage(int what, bool flush) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::sendMessage() what~~~\n");
    }
    if (decodeHandler != nullptr) {
        Message msg = Message();
        msg.what = what;
        msg.obj = this;
        decodeHandler->sendMessage(msg, flush);
    }
}

void AudioHardDecoder::sendMessage(Message &msg, bool flush) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioHardDecoder::sendMessage() msg~~~\n");
    }
    if (decodeHandler != nullptr) {
        msg.setObj(this);
        decodeHandler->sendMessage(msg, flush);
    }
}

void AudioDecodeHandler::handleMessage(Message &msg) {
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DLOGI(AUDIO_HARD_DECODER_TAG, "~~~AudioDecodeHandler::handleMessage~~~\n");
    }
    int what = msg.what;
    if (DebugEnable && AUDIO_DECODER_DEBUG) {
        DFLOGD(AUDIO_HARD_DECODER_TAG, "handleMessage msg what = %d", msg.what);
    }
    switch (what) {
        case kMsgCodecBuffer: {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DLOGD(AUDIO_HARD_DECODER_TAG, "handleMessage kMsgCodecBuffer");
            }
            auto decoder = (AudioHardDecoder *) msg.obj;
            decoder->doDecodeWork();
        }
            break;
        case kMsgResume: {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DLOGD(AUDIO_HARD_DECODER_TAG, "handleMessage kMsgResume");
            }
            auto decoder = (AudioHardDecoder *) msg.obj;
            decoder->doResumeWork();
        }
            break;
        case kMsgPause: {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DLOGD(AUDIO_HARD_DECODER_TAG, "handleMessage kMsgPause");
            }
            auto decoder = (AudioHardDecoder *) msg.obj;
            decoder->doPauseWork();
        }
            break;
        case kMsgPauseAck: {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DLOGD(AUDIO_HARD_DECODER_TAG, "handleMessage kMsgPauseAck");
            }
        }
            break;
        case kMsgSeek: {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DLOGD(AUDIO_HARD_DECODER_TAG, "handleMessage kMsgSeek");
            }
            auto decoder = (AudioHardDecoder *) msg.obj;
            int position = msg.arg1;
            decoder->doSeekWork(position);
        }
            break;
        case kMsgCodecDone: {
            if (DebugEnable && AUDIO_DECODER_DEBUG) {
                DLOGD(AUDIO_HARD_DECODER_TAG, "handleMessage kMsgDecodeDone");
            }
        }
            break;
        default:
            break;
    }
}