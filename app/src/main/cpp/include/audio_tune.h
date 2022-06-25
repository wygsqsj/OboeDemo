/***
	audio_tune 2016/5/11 20:55
	copyright: buluobang
***/

#ifndef AUDIO_TUNE_H
#define AUDIO_TUNE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


float __audio_tune(float *dataArr,int length,float samplate);
float __audio_tuneWeak(float *dataArr,int length,float samplate);
int __audio_tuneReferArr(float *dataArr,
                         int length,
                         float samplate,
                         float *freArr,
                         int freLen,
                         int **outStatusArr,
                         float **outFreArr,
                         int *status,
                         int *isValidExtend,
                         float *validFreArr,
                         int validFreLen,
                         int *validStatus);


#ifdef __cplusplus
}
#endif

#endif
