#include "stereo.h"
// "立体声均匀器" 音效增强器

static stereo_t *stereo = 0;

stereo_conf_t *stereo_init(int sample_rate, int channels) {
    if (!stereo) {
        stereo_conf_t *conf = (stereo_conf_t*)malloc(sizeof(stereo_conf_t));
        conf->intensity     = 2.5;

        stereo = (stereo_t*)malloc(sizeof(stereo_t));
        stereo->sample_rate = sample_rate;
        stereo->channels    = channels;
        stereo->stereo      = (float*)calloc(channels, sizeof(float));
        stereo->conf        = conf;
    }
    return stereo->conf;
}

bool stereo_conf(stereo_conf_t *conf) {
    bool ret = false;
    if (conf) {
        if (conf->intensity > STEREO_INTENSITY_MAX || conf->intensity < STEREO_INTENSITY_MIN) {
            conf->intensity = 2.5;
        }
        ret = true;
    }
    return ret;
}

void stereo_process(float **inblock, float **outblock, int sample_rate) {
    float value = stereo->conf->intensity;
    float *f   = inblock[0];
    float *end = inblock[0] + sample_rate;
    float *out = outblock[0];
    if (stereo->channels != 2) {
        return;
    }
    while (f < end) {
        float center = (f[0] + f[1]) / 2;
        out[0] = center + (f[0] - center) * value;
        out[1] = center + (f[1] - center) * value;
        f   += 2;
        out += 2;
    }
}
