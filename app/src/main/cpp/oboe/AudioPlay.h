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
    pthread_t audioThread;
private:
    std::mutex         mLock;
    std::shared_ptr<oboe::AudioStream> stream;

    // Stream params
    static int constexpr kChannelCount = 1;
    static int constexpr kSampleRate = 32000;
    // Wave params, these could be instance variables in order to modify at runtime
    static float constexpr kAmplitude = 0.5f;
    static float constexpr kFrequency = 440;
    static float constexpr kPI = M_PI;
    static float constexpr kTwoPi = kPI * 2;
    static double constexpr mPhaseIncrement = kFrequency * kTwoPi / (double) kSampleRate;
    // Keeps track of where the wave is
    float mPhase = 0.0;

};

#endif //OBOEDEMO_AUDIOPLAY_H
