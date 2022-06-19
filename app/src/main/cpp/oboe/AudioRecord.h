//
// Created by DELL on 2022/6/18.
//

#ifndef OBOEDEMO_AUDIORECORD_H
#define OBOEDEMO_AUDIORECORD_H

#include <oboe/AudioStream.h>
#include <oboe/AudioStreamBuilder.h>
#include "AudioRecord.h"
#include "android_log.h"
#include <iostream>
#include <fstream>

class AudioRecord {


public:
    AudioRecord();

    ~AudioRecord();

    void start(char *pcmPath);

    void startRecord();

    void stop();

public:
    bool isRecoding;
    std::shared_ptr<oboe::AudioStream> stream;
    char *pcmPath;
    pthread_t recordThread;

private:

};


#endif //OBOEDEMO_AUDIORECORD_H
