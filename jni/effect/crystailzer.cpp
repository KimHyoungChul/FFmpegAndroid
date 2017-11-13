#include "crystailzer.h"
// "高保真均匀器" 音效增强器

static crystailzer_t *crystailzer = 0;

crystailzer_conf_t *crystailzer_init(int sample_rate, int channels) {
    if (!crystailzer) {
        crystailzer_conf_t *conf = (crystailzer_conf_t*)malloc(sizeof(crystailzer_conf_t));
        conf->intensity = 0.1;

        crystailzer = (crystailzer_t*)malloc(sizeof(crystailzer_t));
        crystailzer->sample_rate = sample_rate;
        crystailzer->channels = channels;
        crystailzer->cryst = (float*)calloc(channels, sizeof(float));
        crystailzer->conf = conf;
    }
    return crystailzer->conf;
}

bool crystailzer_conf(crystailzer_conf_t *conf) {
    bool ret = false;
    if (conf) {
        // 强度
        if (conf->intensity > CRYSTAILZER_INTENSITY_MAX || conf->intensity < CRYSTAILZER_INTENSITY_MIN) {
            conf->intensity = 0.1;
        }
        ret = true;
    }
    return ret;
}

void crystailzer_process(float** inblock, float** outblock, int sample_rate) {
    float value = crystailzer->conf->intensity;
    float *f    = inblock[0];
    float *end  = inblock[0] + sample_rate;
    float *out  = outblock[0];
    while (f < end) {
        for (int channel = 0; channel < crystailzer->channels; channel++) {
            float current = *f;
            *out++ = current + (current - crystailzer->cryst[channel]) * value;
            crystailzer->cryst[channel] = current;
            f++;
        }
    }
}
