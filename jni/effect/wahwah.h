#ifndef __WAHWAH_H__
#define __WAHWAH_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {

#define WAHWAH_FREQ_MIN        0.1
#define WAHWAH_FREQ_MAX        4.0
#define WAHWAH_PHASE_MIN       0.0
#define WAHWAH_PHASE_MAX       360.0
#define WAHWAH_DEPTH_MIN       0
#define WAHWAH_DEPTH_MAX       100
#define WAHWAH_RESONANCE_MIN   0.1
#define WAHWAH_RESONANCE_MAX   10.0
#define WAHWAH_OFFSET_MIN      0
#define WAHWAH_OFFSET_MAX      100
#define WAHWAH_GAIN_MIN       -30.0
#define WAHWAH_GAIN_MAX        30

typedef struct {
    double freq;    // 低频振荡频率
    double phase;   // 低频振荡开始相位
    int    depth;   // 深度
    double res;     // 共鸣
    int    freqofs; // 哇哇频率偏移
    double gain;    // output gain
} wahwah_conf_t;

typedef struct {
    float          sample_rate;
    unsigned long  skip_count;
    double         depth;
    double         freqofs;
    double         phase;
    double         outgain;
    double         lfoskip;
    double         xn1, xn2, yn1, yn2;
    double         a0, a1, a2;
    double         b0, b1, b2;
    wahwah_conf_t *conf;             // "哇哇" 音效配置
} wahwah_t;

wahwah_conf_t *wahwah_init(int);
bool wahwah_conf(wahwah_conf_t*);
void wahwah_process(float**, float**, int);
}
#endif
