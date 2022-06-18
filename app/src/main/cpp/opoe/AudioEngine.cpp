//
// Created by DELL on 2022/6/18.
//

#include "AudioEngine.h"
#include "android_log.h"

AudioEngine::AudioEngine() {

}

AudioEngine::~AudioEngine() {

}

void AudioEngine::start() {

    oboe::AudioStreamBuilder builder;

    builder.setDirection(oboe::Direction::Input)
            ->setAudioApi(oboe::AudioApi::OpenSLES)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setFormat(oboe::AudioFormat::Float)
            ->setSampleRate(32000)
//            ->setDeviceId()
            ->setChannelCount(oboe::ChannelCount::Mono);
//            ->setCallback(myCallback);

    oboe::AudioStream  *stream;
    oboe::Result r = builder.openStream(&stream);
    if (r != oboe::Result::OK) {
        LOGI("初始化stream 失败");
    }



}
