#include "phase.h"

#define phaserlfoshape 4.0
#define lfoskipsamples 20
#define DB_TO_LINEAR(x) (pow(10.0, (x) / 20.0))

static phase_t *phase = 0;

phase_conf_t *phase_init(int sample_rate) {
    if (!phase) {
        phase_conf_t *conf = (phase_conf_t*)malloc(sizeof(phase_conf_t));

        phase = (phase_t*)malloc(sizeof(phase_t));
        phase->sample_rate = sample_rate;
        phase->skip_count  = 0;
        phase->gain        = 0;
        phase->fbout       = 0;
        phase->last_stages = 0;
        phase->outgain     = 0;
        phase->conf        = conf;
        for (int i = 0; i < NUM_STAGES; i++) {
            phase->old[i] = 0;
        }
        conf->stages   =  2;
        conf->dry_wet  =  128;
        conf->freq     =  0.4;
        conf->phase    =  0.0;
        conf->depth    =  100;
        conf->feedback =  0;
        conf->gain     = -6.0;
    }
    return phase->conf;
}

bool phase_conf(phase_conf_t* conf) {
    bool ret = false;
    if (conf) {
        // 阶段
        if (conf->stages > PHASE_STAGE_MAX || conf->stages < PHASE_STAGE_MIN) {
            conf->stages = 2;
        }
        // 干/湿
        if (conf->dry_wet > PHASE_DRYWET_MAX || conf->dry_wet < PHASE_DRYWET_MIN) {
            conf->dry_wet = 128;
        }
        // 低频振荡频率
        if (conf->freq > PHASE_FREQ_MAX || conf->freq < PHASE_FREQ_MIN) {
            conf->freq = 0.4;
        }
        // 低频振荡开始相位
        if (conf->phase > PHASE_PHASE_MAX || conf->phase < PHASE_PHASE_MIN) {
            conf->phase = 0.0;
        }
        // 深度
        if (conf->depth > PHASE_PHASE_MAX || conf->depth < PHASE_PHASE_MIN) {
            conf->depth = 100;
        }
        // 反馈
        if (conf->feedback > PHASE_PHASE_MAX || conf->feedback < PHASE_PHASE_MIN) {
            conf->feedback = 0;
        }
        // output gain
        if (conf->gain > PHASE_GAIN_MAX || conf->gain < PHASE_GAIN_MIN) {
            conf->gain = -6.0;
        }
        ret = true;
    }
    return ret;
}

void phase_process(float **inblock, float **outblock, int sample_rate) {
    float *ibuf = inblock[0];
    float *obuf = outblock[0];

    for (int j = phase->last_stages; j < phase->conf->stages; j++) {
        phase->old[j] = 0;
    }
    phase->last_stages = phase->conf->stages;
    phase->lfoskip     = phase->conf->freq * 2 * M_PI / phase->sample_rate;
    phase->phase       = phase->conf->phase * M_PI / 180;
    phase->outgain     = DB_TO_LINEAR(phase->conf->gain);
    for (int i = 0; i < sample_rate; i++) {
        double in = ibuf[i];
        // Feedback must be less than 100% to avoid infinite gain.
        double m = in + phase->fbout * phase->conf->feedback / 101;
        if (((phase->skip_count++) % lfoskipsamples) == 0) {
            //compute sine between 0 and 1
            phase->gain = (1.0 + cos(phase->skip_count * phase->lfoskip + phase->phase)) / 2.0;
            // change lfo shape
            phase->gain = expm1(phase->gain * phaserlfoshape) / expm1(phaserlfoshape);
            // attenuate the lfo
            phase->gain = 1.0 - phase->gain / 255.0 * phase->conf->depth;
        }
        // phasing routine
        for (int j = 0; j < phase->conf->stages; j++) {
            double tmp = phase->old[j];
            phase->old[j] = phase->gain * tmp + m;
            m = tmp - phase->gain * phase->old[j];
        }
        phase->fbout = m;
        obuf[i] = (float) (phase->outgain * (m * phase->conf->dry_wet + in * (255 - phase->conf->dry_wet)) / 255);
    }
}
