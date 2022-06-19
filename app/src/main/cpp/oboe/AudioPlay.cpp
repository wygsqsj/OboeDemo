//
// Created by gaoruijia on 2022/6/19.
//

#include <oboe/AudioStream.h>
#include "AudioPlay.h"

AudioPlay::AudioPlay() {

}


AudioPlay::~AudioPlay() {

}

void *startOfPlay(void *data) {
    AudioPlay *audioPlay = static_cast<AudioPlay *>(data);
    audioPlay->startPlay();
}

void AudioPlay::start(char *mp3Path) {
    this->mp3Path = mp3Path;
    pthread_create(&audioThread, NULL, startOfPlay, this);
}

void AudioPlay::startPlay() {

    // 1. 音频流构建器
//    oboe::AudioStreamBuilder builder = oboe::AudioStreamBuilder();

    oboe::AudioStreamBuilder builder;
    builder.setDirection(oboe::Direction::Output)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)    // 设置性能优先级
            ->setSharingMode(oboe::SharingMode::Exclusive)    // 设置共享模式 , 独占
            ->setFormat(oboe::AudioFormat::I16)    // 设置音频采样格式
            ->setChannelCount(oboe::ChannelCount::Stereo)      // 设置声道数 , 单声道/立体声
            ->setSampleRate(32000)    // 设置采样率
            ->setChannelCount(1)//声道数量
            ->setCallback(this)  // 设置回调对象 ,注意要设置 AudioStreamCallback * 指针类型
            ;


    // 2. 通过 AudioStreamBuilder 打开 Oboe 音频流
    oboe::Result result = builder.openStream(stream);
    stream.reset();
    if (result != oboe::Result::OK) {
        LOGI("初始化stream 失败");
    }

    //设置缓冲区大小
//    auto setBufferSizeResult = stream->setBufferSizeInFrames(stream->getFramesPerBurst() * 2);
    auto setBufferSizeResult = stream->setBufferSizeInFrames(187*1024);
    if (setBufferSizeResult) {
        LOGI("当前设置的缓冲区大小为%d", setBufferSizeResult.value());
    }

    // 3. 开始播放
//    result = stream->requestStart();
    if (result != oboe::Result::OK) {
        LOGI("requestStart 失败");
    }

    // 返回数据到
    std::string hello =
            "Oboe Test " + std::to_string(static_cast<int>(oboe::PerformanceMode::LowLatency)) +
            " Result : " + oboe::convertToText(result);
}

void AudioPlay::stop() {

}

FILE *file = fopen("/storage/emulated/0/Android/data/com.example.oboedemo/files/audio.pcm", "ab");
int index = 0;

//音频播放回调方法，此处塞入数据
oboe::DataCallbackResult
AudioPlay::onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
    if (index < 187*1024){
        int  read = fread(audioData, 187*1024 * sizeof(int16_t), 1, file);
        index  = 187*1024;
        return oboe::DataCallbackResult::Continue;
    }else{
        fclose(file);
        return oboe::DataCallbackResult::Stop;
    }

}


