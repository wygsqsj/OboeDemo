//
// Created by gaoruijia on 2022/6/19.
//

#ifndef OBOEDEMO_AUDIOPLAY_H
#define OBOEDEMO_AUDIOPLAY_H

#include "android_log.h"
#include <oboe/AudioStreamBuilder.h>
#include <oboe/AudioStream.h>

class AudioPlay : public oboe::AudioStreamCallback{

public:
    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override;

public:
    AudioPlay();

    ~AudioPlay();

    void start(char *mp3Path);

    void startPlay();

    void stop();

public:
    char *mp3Path;
    bool isPlay;
    std::shared_ptr<oboe::AudioStream> stream = nullptr;
    pthread_t audioThread;
private:

};

#endif //OBOEDEMO_AUDIOPLAY_H
