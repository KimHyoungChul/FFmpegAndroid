#include "echo.h"
// "回音" 音效增强器

#define MAX_DELAY 1000
static echo_t *echo = 0;

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int clamp(int x, int low, int high) {
    return min(max(x, low), high);
}

int chsign(int x, int s) {
    return (x < 0) ^ (s < 0) ? -x : x;
}

int rdiv(int x, int y) {
    return (x + chsign(y / 2, x)) / y;
}

int rescale(int x, int old_scale, int new_scale) {
    return rdiv(x * new_scale, old_scale);
}

echo_conf_t *echo_init(int sample_rate, int channels) {
    if (!echo) {
        echo_conf_t *conf = (echo_conf_t*)malloc(sizeof(echo_conf_t));
        conf->delay    = 500;
        conf->feedback = 50;
        conf->volume   = 50;

        echo = (echo_t*)malloc(sizeof(echo_t));
        echo->sample_rate = sample_rate;
        echo->channels = channels;
        echo->buffer_size = rescale(MAX_DELAY, 1000, sample_rate) * channels;
        echo->buffer = (float*)calloc(1, echo->buffer_size);
        echo->conf = conf;
    }
    return echo->conf;
}

bool echo_conf(echo_conf_t *conf) {
    bool ret = false;
    if (conf) {
        // 延迟
        if (conf->delay > ECHO_DELAY_MAX || conf->delay < ECHO_DELAY_MIN) {
            conf->delay = 500;
        }
        // 反馈
        if (conf->feedback > ECHO_FEEDBACK_MAX || conf->feedback < ECHO_FEEDBACK_MIN) {
            conf->feedback = 50;
        }
        // 音量
        if (conf->volume > ECHO_VOLUME_MAX || conf->volume < ECHO_VOLUME_MIN) {
            conf->volume = 50;
        }
        ret = true;
    }
    return ret;
}

void echo_process(float **inblock, float **outblock, int sample_rate) {

    int   delay    = echo->conf->delay;
    float feedback = echo->conf->feedback / 100.0f;
    float volume   = echo->conf->volume / 100.0f;
    int interval = rescale(delay, 100, echo->sample_rate);
    interval = clamp(interval, 0, echo->buffer_size);
    int r_ofs = echo->w_ofs - interval;
    if (r_ofs < 0)
        r_ofs += echo->buffer_size;

    float *f   = inblock[0];
    float *end = inblock[0] + sample_rate;
    float *out = outblock[0];
    while (f < end) {
        float in = *f;
        float buf = echo->buffer[r_ofs];
        *out = in + buf * volume;
        echo->buffer[echo->w_ofs] = in + buf * feedback;
        r_ofs = (r_ofs + 1) % echo->buffer_size;
        echo->w_ofs = (echo->w_ofs + 1) % echo->buffer_size;

        f++;
        out++;
    }
}
