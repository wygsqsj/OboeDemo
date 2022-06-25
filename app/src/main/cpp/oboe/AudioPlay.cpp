//
// Created by gaoruijia on 2022/6/19.
//

#include <oboe/AudioStream.h>
#include "AudioPlay.h"

using namespace oboe;

AudioPlay::AudioPlay() {

}


AudioPlay::~AudioPlay() {

}

FILE *file = nullptr;
int index = 0;
long fileLength = 0;

void *startOfPlay(void *data) {
    AudioPlay *audioPlay = static_cast<AudioPlay *>(data);
    audioPlay->startPlay();
}

void AudioPlay::start(char *mp3Path) {
    LOGI("当前线程 %d", gettid());
    this->mp3Path = mp3Path;
    startPlay();
}

void AudioPlay::startPlay() {
    /* LOGI("startPlay 开始设置");

     // 1. 音频流构建器
 //    oboe::AudioStreamBuilder builder = oboe::AudioStreamBuilder();

     oboe::AudioStreamBuilder builder;
     builder.setDirection(oboe::Direction::Output)
             ->setPerformanceMode(oboe::PerformanceMode::LowLatency)    // 设置性能优先级
             ->setSharingMode(oboe::SharingMode::Exclusive)    // 设置共享模式 , 独占
             ->setFormat(oboe::AudioFormat::Float)    // 设置音频采样格式
             ->setSampleRate(32000)    // 设置采样率
             ->setChannelCount(1)//声道数量
             ->setCallback(this)  // 设置回调对象 ,注意要设置 AudioStreamCallback * 指针类型
             ;


     // 2. 通过 AudioStreamBuilder 打开 Oboe 音频流
     oboe::Result result = builder.openStream(stream);
     if (result != oboe::Result::OK) {
         LOGI("初始化stream 失败");
     }

     //设置缓冲区大小
     auto setBufferSizeResult = stream->setBufferSizeInFrames(stream->getFramesPerBurst() * 2);
 //    auto setBufferSizeResult = stream->setBufferSizeInFrames(187 * 1024);
     if (setBufferSizeResult) {
         LOGI("当前设置的缓冲区大小为%d", setBufferSizeResult.value());
     }

     // 3. 开始播放
     result = stream->requestStart();
     if (result != oboe::Result::OK) {
         LOGI("requestStart 失败");
     }*/
    index = 0;
//    file = fopen("/sdcard/Android/data/com.example.oboedemo/files/audio.pcm", "rb+");
    file = fopen("/sdcard/Android/data/com.example.oboedemo/files/convertLaterFloat.pcm", "rb+");
//    file = fopen("/sdcard/Android/data/com.example.oboedemo/files/convertLaterShort.pcm", "rb+");
    if (file != nullptr) {
        LOGI("获取file成功");
        //获取pcm数据长度
        int fd1 = fileno(file);
        //将读写位置移到文件尾为了获取文件大小
        fileLength = lseek(fd1, 0, SEEK_END);
        //再设置偏移到文件开头
        fseek(file, 0, SEEK_SET);
    } else {
        LOGI("获取file失败");
    }
    LOGI("获取file长度,%ld", fileLength);


    std::lock_guard<std::mutex> lock(mLock);
    oboe::AudioStreamBuilder builder;
    // The builder set methods can be chained for convenience.
    Result result = builder.setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setChannelCount(kChannelCount)
            ->setSampleRate(kSampleRate)
            ->setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Medium)
            ->setFormat(oboe::AudioFormat::Float)
            ->setDataCallback(this)
            ->openStream(stream);

    if (result != Result::OK) {
        LOGI("初始化stream 失败");
        return;
    }

    // Typically, start the stream after querying some stream information, as well as some input from the user
    stream->requestStart();
}

void AudioPlay::stop() {
    std::lock_guard<std::mutex> lock(mLock);
    if (stream) {
        stream->stop();
        stream->close();
        stream.reset();
    }
}


//音频播放回调方法，此处塞入数据
oboe::DataCallbackResult
AudioPlay::onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
    LOGI("播放回调，numFrames：%d,当前 index,%d,当前线程 %d", numFrames, index, gettid());

    //float数据先将pcm转换成float再塞入
//    int16_t *pcmData = static_cast<int16_t *>(malloc(numFrames * sizeof(int16_t)));
//    fread(pcmData, numFrames * sizeof(int16_t), 1, file);
//    oboe::convertPcm16ToFloat(pcmData, static_cast<float *>(audioData), numFrames);
//    oboe::convertFloatToPcm16(static_cast<float *>(audioData), pcmData, numFrames);

//    //short数据,数据源是float，先将float转换成 short 再塞入
//    float *pcmData = static_cast<float *>(malloc(numFrames * sizeof(float)));
//    fread(pcmData, numFrames * sizeof(int16_t), 1, file);
//    oboe::convertPcm16ToFloat(pcmData, static_cast<float *>(audioData), numFrames);
//    oboe::convertFloatToPcm16(static_cast<float *>(audioData), pcmData, numFrames);


    //直接写入float数据
//    float *pcmData = static_cast<float *>(malloc(numFrames * sizeof(float)));
    fread(audioData, numFrames * sizeof(float), 1, file);


//    // int16_t类型数据塞入数据
//    fread(audioData, numFrames * sizeof(int16_t), 1, file);

    index += numFrames * sizeof(int16_t);
//    index += numFrames * sizeof(float);
    LOGI("当前 index,%d", index);
    if (index < fileLength) {
        return oboe::DataCallbackResult::Continue;
    } else {
        return oboe::DataCallbackResult::Stop;
    }
}


