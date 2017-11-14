#ifndef __ECHO_H__
#define __ECHO_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ECHO_DELAY_MIN     10
#define ECHO_DELAY_MAX     1000
#define ECHO_FEEDBACK_MIN  0
#define ECHO_FEEDBACK_MAX  100
#define ECHO_VOLUME_MIN    0
#define ECHO_VOLUME_MAX    100

typedef struct {
    int delay;    // 延迟
    int feedback; // 反馈
    int volume;   // 音量
} echo_conf_t;

typedef struct {
    int          sample_rate;
    int          channels;
    int          w_ofs;
    float       *buffer;
    int          buffer_size;
    echo_conf_t *conf;
} echo_t;

echo_conf_t *echo_init(int, int);
bool         echo_conf(echo_conf_t*);
void         echo_process(float**, float**, int);

#endif
