#ifndef ACOUSTIC_H
#define ACOUSTIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
    
typedef struct {
    int samplate; // 32000
    
    float echo_level; // .05f
    int is_strict; // 1
    
    int nb_bands; // 24
    
    int noise_sup; // -15
    int echo_sup; // -40
    int echo_sup_active; // -15
} AcousticInfor;

typedef struct Acoustic_ *Acoustic;

int acoustic_new(Acoustic *ac,int len1,int len2);
int acoustic_newWithData(Acoustic *ac,int len1,int len2,AcousticInfor *infor);

void acoustic_cal(Acoustic ac,short *data1,short *data2);
void acoustic_calFile(Acoustic ac,FILE *file1,FILE *file2,FILE *file3);

// 针对标准形式
void acoustic_changeStatus(Acoustic ac,int flag);

void acoustic_getData(Acoustic ac,short **data);
void acoustic_copyData(Acoustic ac,short **data);

void acoustic_free(Acoustic ac);

#ifdef __cplusplus
}
#endif

#endif
