#ifndef __PHASE_H__
#define __PHASE_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {

#define NUM_STAGES 24

#define PHASE_STAGE_MIN     2
#define PHASE_STAGE_MAX     NUM_STAGES
#define PHASE_DRYWET_MIN    0
#define PHASE_DRYWET_MAX    255
#define PHASE_FREQ_MIN      0.001
#define PHASE_FREQ_MAX      4.0
#define PHASE_PHASE_MIN     0.0
#define PHASE_PHASE_MAX     360.0
#define PHASE_DEPTH_MIN     0
#define PHASE_DEPTH_MAX     255
#define PHASE_FEEDBACK_MIN -100
#define PHASE_FEEDBACK_MAX  100
#define PHASE_GAIN_MIN     -30.0
#define PHASE_GAIN_MAX      30

typedef struct {
    int    stages;      // 阶段
    int    dry_wet;     // 干/湿
    double freq;        // 低频振荡频率
    double phase;       // 低频振荡开始相位
    int    depth;       // 深度
    int    feedback;    // 反馈
    int    gain;        // output gain
} phase_conf_t;

typedef struct {
    double        old[NUM_STAGES];
    double        gain;
    double        fbout;
    double        outgain;
    double        lfoskip;
    double        phase;
    float         sample_rate;
    int           skip_count;
    int           last_stages;
    phase_conf_t *conf;            // phase 音效配置器
} phase_t;

phase_conf_t *phase_init(int);
bool phase_conf(phase_conf_t*);
void phase_process(float**, float**, int);

}
#endif
