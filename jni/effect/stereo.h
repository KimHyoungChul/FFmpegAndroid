#ifndef __STEREO_H__
#define __STEREO_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {

#define STEREO_INTENSITY_MIN 0
#define STEREO_INTENSITY_MAX 10

typedef struct {
    float intensity; // 强度
} stereo_conf_t;

typedef struct {
    int            sample_rate; // 采样率
    int            channels;    // 通道数
    float         *stereo;      // 临时计算区域
    stereo_conf_t *conf;        // 配置
} stereo_t;

stereo_conf_t *stereo_init(int, int);
bool           stereo_conf(stereo_conf_t*);
void           stereo_process(float**, float**, int);
}
#endif
