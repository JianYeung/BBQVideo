//
// Created by jian.yeung on 2020/9/13.
//

#include <jni.h>
#include <unistd.h>
#include "VideoHardDecoder.h"

VideoHardDecoder::VideoHardDecoder() : VideoDecoder() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::VideoHardDecoder()~~~\n");
    }
    decodeHandler = new DecodeHandler();
}

VideoHardDecoder::~VideoHardDecoder() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::~VideoHardDecoder()~~~\n");
    }
}

void VideoHardDecoder::setRender(GLRender *render)  {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::setRender()~~~\n");
    }
    if (render == nullptr) {
        DLOGE(HARD_DECODER_TAG, "Invalid GLRender");
        return;
    }
    this->glRender = render;
    this->filter = render->getFilter();
}

void VideoHardDecoder::setDataSource(std::string url) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::setDataSource()~~~\n");
    }
    videoUrl = std::move(url);
    restartCodec();
}

void VideoHardDecoder::setSurface(ANativeWindow *nativeWindow, const int width, const int height) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::setSurface()~~~\n");
    }
    if (surfaceWindow != nullptr) {
        ANativeWindow_release(surfaceWindow);
        surfaceWindow = nullptr;
    }
    surfaceWindow = nativeWindow;
    surfaceWidth = width;
    surfaceHeight = height;
}

void VideoHardDecoder::start() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::start()~~~\n");
    }
    if (isPlaying) {
        DLOGI(HARD_DECODER_TAG, "It is playing video, please stop to play first!!!");
        return;
    }
    if (!videoUrl.empty() && surfaceWindow != nullptr) {
        if (isCodecReady) {
            sendMessage(kMsgCodecBuffer);
        } else {
            if (initCodec()) {
                sendMessage(kMsgCodecBuffer);
            } else {
                DLOGE(HARD_DECODER_TAG, "Init Codec Failed!!!");
            }
            //TODO 先修复Handler不能处理定时信息，再使用异步方式创建解码器
            //sendMessage(kMsgWaitingCodec);
        }
    } else {
        DLOGE(HARD_DECODER_TAG, "Has not video url or surface window!!!");
    }
}

void VideoHardDecoder::resume() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::resume()~~~\n");
    }
    sendMessage(kMsgResume);
}

void VideoHardDecoder::pause() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::pause()~~~\n");
    }
    sendMessage(kMsgPause);
}

void VideoHardDecoder::seek(int position) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::seek()~~~\n");
    }
    if (position < 0 || position > 100) {
        DLOGE(HARD_DECODER_TAG, "seek position is illegal");
        return;
    }

    Message msg = Message();
    msg.what = kMsgSeek;
    msg.arg1 = position;
    sendMessage(msg);
}

void VideoHardDecoder::stop() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::stop()~~~\n");
    }
    isCodecRelease = true;
    sendMessage(kMsgCodecDone, true);
    releaseCodec();
}

void VideoHardDecoder::release() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::release()~~~\n");
    }
    isCodecRelease = true;
    if (codec != nullptr) {
        releaseCodec();
        codec = nullptr;
        extractor = nullptr;
    }
    if (decodeHandler != nullptr) {
        decodeHandler->quit();
        delete decodeHandler;
        decodeHandler = nullptr;
    }
    if (surfaceWindow != nullptr) {
        ANativeWindow_release(surfaceWindow);
        surfaceWindow = nullptr;
    }
}

bool VideoHardDecoder::initCodec() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::initCodec() Start~~~\n");
    }
    if (extractor == nullptr) {
        extractor = AMediaExtractor_new();
    }

    AMediaExtractor_setDataSource(extractor, videoUrl.c_str());
    int numTracks = AMediaExtractor_getTrackCount(extractor);
    codec = nullptr;
    for (int i = 0; i < numTracks; i++) {
        format = AMediaExtractor_getTrackFormat(extractor, i);
        const char *s = AMediaFormat_toString(format);
        if (DebugEnable && VIDEO_DECODER_DEBUG) {
            DFLOGD(HARD_DECODER_TAG, "Get track %d format: %s", i, s);
        }
        const char *mime;
        if (!AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mime)) {
            DLOGE(HARD_DECODER_TAG, "Has no mime type!!!");
            return JNI_FALSE;
        } else if (!strncmp(mime, "video/avc", 9)) {
            // Omitting most error handling for clarity.
            // Production code should check for errors.
            int64_t durationUs;
            AMediaFormat_getInt64(format, AMEDIAFORMAT_KEY_DURATION, &durationUs);
            outDuration = (long) (durationUs / 1000000);

            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DFLOGD(HARD_DECODER_TAG, "video duration %lld us", (long long) durationUs);
            }
            AMediaExtractor_selectTrack(extractor, i);
            codec = AMediaCodec_createDecoderByType(mime);
            //surface目前设置为nullptr;
            AMediaCodec_configure(codec, format, nullptr, nullptr, 0);
            isCodecReady = true;
            renderStart = -1;
            isPlaying = false;
            sawInputEOS = false;
            sawOutputEOS = false;
            AMediaCodec_start(codec);
            break;
        }
    }
    if (format != nullptr) {
        AMediaFormat_delete(format);
        format = nullptr;
    }
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::initCodec() End~~~\n");
    }
    return true;
}

int64_t systemNanoTime() {
    timespec now{};
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000LL + now.tv_nsec;
}

void VideoHardDecoder::doDecodeWork() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::doDecodeWork() Start~~~\n");
    }

    if (!isCodecReady) {
        DLOGE(HARD_DECODER_TAG, "doDecodeWork() Codec has not initialized，please init codec first!!!");
        return;
    }

    if (!sawInputEOS) {
        ssize_t bufIdx = AMediaCodec_dequeueInputBuffer(codec, 2000);
        if (DebugEnable && VIDEO_DECODER_DEBUG) {
            DFLOGD(HARD_DECODER_TAG, "input buffer index: %zd", bufIdx);
        }
        if (bufIdx >= 0) {
            size_t bufsize;
            auto buf = AMediaCodec_getInputBuffer(codec, bufIdx, &bufsize);
            auto sampleSize = AMediaExtractor_readSampleData(extractor, buf, bufsize);
            if (sampleSize < 0) {
                sampleSize = 0;
                sawInputEOS = true;
                DLOGE(HARD_DECODER_TAG, "input EOS");
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
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DFLOGD(HARD_DECODER_TAG, "output buffer index: %zd", status);
            }

            if (info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
                DLOGE(HARD_DECODER_TAG, "output EOS");
                sawOutputEOS = true;
            }

            uint8_t *outputBuf = AMediaCodec_getOutputBuffer(codec, status, nullptr/* out_size */);
            size_t dataSize = info.size;
            if (outputBuf != nullptr && dataSize != 0) {
                isPlaying = true;
                long pts = info.presentationTimeUs;
                if (DebugEnable && VIDEO_DECODER_DEBUG) {
                    DFLOGD(HARD_DECODER_TAG, "output buffer pts: %ld", pts);
                }
                int64_t presentationNano = pts * 1000;
                if (renderStart < 0) {
                    renderStart = systemNanoTime() - presentationNano;
                }
                if (filter != nullptr) {
                    VideoFrame frame = VideoFrame();
                    frame.updateFrameInfo(outputBuf, outFormat, outWidth, outHeight);
                    filter->updatePreviewFrame(&frame);
                    if (glRender != nullptr && glRender->getRenderMode() == RenderMode::RENDERMODE_WHEN_DIRTY) {
                        glRender->requestRender();
                    }
                }
                AMediaCodec_releaseOutputBuffer(codec, status, info.size != 0);
                int64_t delay = (renderStart + presentationNano) - systemNanoTime();
                if (DebugEnable && VIDEO_DECODER_DEBUG) {
                    int time = delay > 0 ? (int) delay: 0;
                    DFLOGD(HARD_DECODER_TAG, "After render, decoder sleep %d us", time);
                }
                if (delay > 0) {
                    usleep(delay / 1000);
                }
            }
        } else if (status == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "output buffers changed");
            }
        } else if (status == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
            format = AMediaCodec_getOutputFormat(codec);
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DFLOGD(HARD_DECODER_TAG, "format changed to: %s", AMediaFormat_toString(format));
            }
            int32_t val32;
            if (format != nullptr) {
                AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, &val32);
                outFormat = val32;
                AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_WIDTH, &val32);
                outWidth = val32;
                AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_HEIGHT, &val32);
                outHeight = val32;
                if (DebugEnable && VIDEO_DECODER_DEBUG) {
                    DFLOGD(HARD_DECODER_TAG, "color format: %d, corp width: %d, corp height: %d", outFormat, outWidth, outFormat);
                }
                //TODO 优化项
//                if (glRender != nullptr) {
//                    glRender->onSurfaceChanged(surfaceWindow, outFormat, outWidth, outHeight);
//                }
            }
            AMediaFormat_delete(format);
            format = nullptr;
        } else if (status == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "no output buffer right now");
            }
        } else {
            DFLOGE(HARD_DECODER_TAG, "unexpected info code: %zd", status);
        }
    }

    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGD(HARD_DECODER_TAG, "sawInputEOS = %d, sawOutputEOS = %d", sawInputEOS, sawOutputEOS);
    }

    if (!sawInputEOS || !sawOutputEOS) {
        sendMessage(kMsgCodecBuffer);
    }

    if (sawInputEOS && sawOutputEOS && !isCodecRelease) {
        if (extractor != nullptr && codec != nullptr) {
            AMediaExtractor_seekTo(extractor, 0, AMEDIAEXTRACTOR_SEEK_NEXT_SYNC);
            AMediaCodec_flush(codec);
        }
        isPlaying = false;
        renderStart = -1;
        sawInputEOS = false;
        sawOutputEOS = false;
    }

    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::doDecodeWork() End~~~\n");
    }
}

void VideoHardDecoder::doResumeWork() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~DecodeHandler::doResumeWork() Start~~~\n");
    }
    if (!isPlaying) {
        renderStart = -1;
        sendMessage(kMsgCodecBuffer);
    }

    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~DecodeHandler::doResumeWork() End~~~\n");
    }
}

void VideoHardDecoder::doPauseWork() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~DecodeHandler::doPauseWork() Start~~~\n");
    }
    isPlaying = false;
    // flush all outstanding codecbuffer messages with a no-op message
    sendMessage(kMsgPauseAck, true);
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~DecodeHandler::doPauseWork() End~~~\n");
    }
}


void VideoHardDecoder::doSeekWork(int position) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~DecodeHandler::doSeekWork() Start~~~\n");
    }

    int64_t seekPosition = (outDuration * position * 1000000 / 100);
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DFLOGD(HARD_DECODER_TAG, "seek position to %ld us\n", (long) seekPosition);
    }
    if (isCodecReady) {
        AMediaExtractor_seekTo(extractor, seekPosition, AMEDIAEXTRACTOR_SEEK_CLOSEST_SYNC);
        AMediaCodec_flush(codec);
        renderStart = -1;
        sawInputEOS = false;
        sawOutputEOS = false;
        sendMessage(kMsgCodecBuffer, true);
    } else {
        DLOGE(HARD_DECODER_TAG, "doSeekWork() Codec has not initialized，please init codec first!!!");
    }
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~DecodeHandler::doSeekWork() End~~~\n");
    }
}

void VideoHardDecoder::releaseCodec() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::releaseCodec() Start~~~\n");
    }
    isCodecReady = false;
    isPlaying = false;
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
    isCodecRelease = false;
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::releaseCodec() End~~~\n");
    }
}

void VideoHardDecoder::restartCodec() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::restartCodec()~~~\n");
    }
    if (!videoUrl.empty() && surfaceWindow != nullptr) {
        if (isCodecReady) {
            releaseCodec();
        }
        initCodec();
    }
}

bool VideoHardDecoder::getCodecReadyState() {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::getCodecReadyState()~~~\n");
    }
    return this->isCodecReady;
}

void VideoHardDecoder::sendMessage(int what, bool flush) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::sendMessage() what~~~\n");
    }
    if (decodeHandler != nullptr) {
        if (flush) {
            decodeHandler->removeCallbacksAndMessages();
        }
        Message msg = Message();
        msg.what = what;
        msg.obj = this;
        decodeHandler->sendMessage(msg);
    }
}

void VideoHardDecoder::sendMessage(Message &msg, bool flush) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~VideoHardDecoder::sendMessage() msg~~~\n");
    }
    if (decodeHandler != nullptr) {
        if (flush) {
            decodeHandler->removeCallbacksAndMessages();
        }
        msg.setObj(this);
        decodeHandler->sendMessage(msg);
    }
}

void DecodeHandler::handleMessage(Message &msg) {
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DLOGI(HARD_DECODER_TAG, "~~~DecodeHandler::handleMessage~~~\n");
    }
    int what = msg.what;
    if (DebugEnable && VIDEO_DECODER_DEBUG) {
        DFLOGD(HARD_DECODER_TAG, "handleMessage msg what = %d", msg.what);
    }
    switch (what) {
        case kMsgInitCodec: {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "handleMessage kMsgInitCodec");
            }
            auto decoder = (VideoHardDecoder *) msg.obj;
            decoder->restartCodec();
        }
            break;
        case kMsgWaitingCodec: {
            auto decoder = (VideoHardDecoder *) msg.obj;
            DFLOGD(HARD_DECODER_TAG, "Init codec state %d", decoder->getCodecReadyState());
            if (decoder->getCodecReadyState()) {
                decoder->sendMessage(kMsgCodecBuffer,true);
                retryCount = 0;
            } else {
                if (retryCount < maxRetryCount) {
                    sleep(20);
                    decoder->sendMessage(kMsgWaitingCodec);
                    retryCount++;
                    DFLOGE(HARD_DECODER_TAG, "Wait %d time", retryCount);
                } else {
                    DLOGE(HARD_DECODER_TAG, "Init Codec Failed!!!");
                    retryCount = 0;
                }
            }
        }
            break;
        case kMsgCodecBuffer: {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "handleMessage kMsgCodecBuffer");
            }
            auto decoder = (VideoHardDecoder *) msg.obj;
            decoder->doDecodeWork();
        }
            break;
        case kMsgResume: {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "handleMessage kMsgResume");
            }
            auto decoder = (VideoHardDecoder *) msg.obj;
            decoder->doResumeWork();
        }
            break;
        case kMsgPause: {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "handleMessage kMsgPause");
            }
            auto decoder = (VideoHardDecoder *) msg.obj;
            decoder->doPauseWork();
        }
            break;
        case kMsgPauseAck: {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "handleMessage kMsgPauseAck");
            }
        }
            break;
        case kMsgSeek: {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "handleMessage kMsgSeek");
            }
            auto decoder = (VideoHardDecoder *) msg.obj;
            int position = msg.arg1;
            decoder->doSeekWork(position);
        }
            break;
        case kMsgCodecDone: {
            if (DebugEnable && VIDEO_DECODER_DEBUG) {
                DLOGD(HARD_DECODER_TAG, "handleMessage kMsgDecodeDone");
            }
            //TODO 由于Codec不是在这个线程创建的，所以就不能在这个线程销毁，后面会优化这个实现
//            auto decoder = (VideoHardDecoder *) msg.obj;
//            decoder->releaseCodec();
        }
            break;
        default:
            break;
    }
}