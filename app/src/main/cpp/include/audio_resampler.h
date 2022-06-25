#ifndef AUDIORESAMPLER_H
#define AUDIORESAMPLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct AudioResampler_ *AudioResampler;

// quality 0~10 default 4
int audioResampler_new(AudioResampler *ar,int sSamplate,int dSamplate,int quality);

void audioResampler_process(AudioResampler ar,short *sData,int *sLen,short *dData,int *dLen);

void audioResampler_free(AudioResampler ar);

#ifdef __cplusplus
}
#endif

#endif
