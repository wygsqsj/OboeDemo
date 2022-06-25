/***
	DSP base 2016/7/21 18:05
	copyright: buluobang
***/

#ifndef DSP_BASE_H
#define DSP_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float *dsp_createHann(int length,int flag);
float *dsp_createHamm(int length);
float *dsp_createBlackman(int length);
float *dsp_createKaiser(int length,float alpha);
// will deprecate,use dsp_createSincCoffForLowpass
float *dsp_createSincNodd(int length,float percent);
// replace dsp_createSincNodd
float *dsp_createSincCoffForLowpass(int n,float fc);

float dsp_sinc(float x);
float *dsp_conv(float *xnArr,float *hnArr,int xnLen,int hnLen);
float *dsp_xcorr(float *xnArr,float *hnArr,int length);
float dsp_calMaxXcorrPercent(float *xnArr,float *hnArr,int length,int *index);
float *dsp_cohere(float *xnArr,float *hnArr,int length);



int dsp_calRadix2(int length);
void dsp_vdot(float *data1,float *data2,float *outArr,int length);
void dsp_cdot(float *realArr1,float *imagArr1,float *realArr2,float *imagArr2,float *outRealArr,float *outImagArr,int length);

void dsp_debugArr(float *arr,int length,char *name);


#ifdef __cplusplus
}
#endif

#endif
