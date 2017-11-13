#include "voiceremoval.h"
// "人声消除" 音效增强器

static voiceremoval_t *voice = 0;

void voiceremoval_init(int sample_rate, int channels) {
    if (!voice) {
        voice = (voiceremoval_t*)malloc(sizeof(voiceremoval_t));
        voice->sample_rate = sample_rate;
        voice->channels = channels;
    }
}

void voiceremoval_conf() {
}

void voiceremoval_process(float **inblock, float **outblock, int sample_rate) {
    float *f   = inblock[0];
    float *end = inblock[0] + sample_rate;
    float *out = outblock[0];
    while (f < end) {
        f[0]   -= f[1];
        f[1]    = f[0];
        out[0]  = f[0];
        out[1]  = f[1];

        f   += 2;
        out += 2;
    }
}
