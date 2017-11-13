#ifndef __CRYSTAILZER_H__
#define __CRYSTAILZER_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {

#define CRYSTAILZER_INTENSITY_MIN  0
#define CRYSTAILZER_INTENSITY_MAX  10

typedef struct {
    float intensity; // 强度
} crystailzer_conf_t;

typedef struct {
    int                 sample_rate; // 采样率
    int                 channels;    // 通道数
    float              *cryst;       // 临时计算区域
    crystailzer_conf_t *conf;        // 配置
} crystailzer_t;

crystailzer_conf_t *crystailzer_init(int, int);
bool                crystailzer_conf(crystailzer_conf_t*);
void                crystailzer_process(float**, float**, int);

}
#endif
