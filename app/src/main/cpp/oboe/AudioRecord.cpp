//
// Created by DELL on 2022/6/18.
//

#include "AudioRecord.h"

AudioRecord::AudioRecord() {

}

AudioRecord::~AudioRecord() {

}

void *startOfThread(void *args) {
    AudioRecord *audioEngine = (AudioRecord *) args;
    audioEngine->startRecord();
    //释放线程
    pthread_exit(&audioEngine->recordThread);
}


void AudioRecord::start(char *pcmPath) {
    this->pcmPath = pcmPath;
    //耗时操作，放到子线程里面
    pthread_create(&recordThread, NULL, startOfThread, this);
}

void AudioRecord::stop() {
    isRecoding = false;
}

void AudioRecord::startRecord() {
    LOGI("当前pcmPath:%s", pcmPath);

    isRecoding = true;

    oboe::AudioStreamBuilder builder;

    builder.setDirection(oboe::Direction::Input)
            ->setAudioApi(oboe::AudioApi::AAudio)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setFormat(oboe::AudioFormat::I16)
            ->setSampleRate(32000)
//            ->setDeviceId(17)
            ->setChannelCount(1)
//            ->setDataCallback(new MyCallback(this))
            ;


    oboe::Result r = builder.openStream(stream);
    if (r != oboe::Result::OK) {
        LOGI("初始化stream 失败");
    }

    r = stream->requestStart();
    if (r != oboe::Result::OK) {
        LOGI("requestStart 失败");
    }

    constexpr int kMillisecondsToRecord = 2;
    const auto requestedFrames = (int32_t)
            (kMillisecondsToRecord * stream->getSampleRate() / kMillisecondsToRecord);
    int16_t mybuffer[requestedFrames];
    constexpr int64_t kTimeoutValue = 3 * kMillisecondsToRecord;

    int framesRead = 0;
    do {
        auto result = stream->read(mybuffer, stream->getBufferCapacityInFrames(), 0);
        if (result != oboe::Result::OK) {
            LOGI("当前获取录音数据异常");
            break;
        }
        framesRead = result.value();
    } while (framesRead != 0);

    FILE *outFile = fopen(pcmPath, "wb+");
    while (isRecoding) {
        auto result = stream->read(mybuffer, requestedFrames, kTimeoutValue);
        if (result == oboe::Result::OK) {
//            for (int i = 0; i < result.value(); ++i) {
//                LOGI("当前获取录音数据,%hd", mybuffer[i]);
//            }
            LOGI("当前获取录音数据,%d", result.value());
            fwrite(mybuffer, result.value() * sizeof(int16_t), 1, outFile);
        } else {
            LOGE("当前获取录音数据失败：%s", convertToText(result.error()));
        }
    }
    LOGI("录音结束");

    if (stream != nullptr) {
        stream->close();
        fclose(outFile);
    }
}