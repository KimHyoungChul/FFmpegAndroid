#include "wahwah.h"

#define DB_TO_LINEAR(x) (pow(10.0, (x) / 20.0))
#define lfoskipsamples 30

static wahwah_t *wahwah = 0;

wahwah_conf_t *wahwah_init(int sample_rate) {
    if (!wahwah) {
        wahwah_conf_t *conf = (wahwah_conf_t*)malloc(sizeof(wahwah_conf_t));
        conf->freq    =  1.5;
        conf->phase   =  0.0;
        conf->depth   =  70;
        conf->res     =  2.5;
        conf->freqofs =  30;
        conf->gain    = -6.0;

        wahwah = (wahwah_t*)malloc(sizeof(wahwah_t));
        wahwah->sample_rate = sample_rate;
        wahwah->lfoskip = conf->freq * 2 * M_PI / sample_rate;
        wahwah->skip_count = 0;
        wahwah->xn1 = 0;
        wahwah->xn2 = 0;
        wahwah->yn1 = 0;
        wahwah->yn2 = 0;
        wahwah->a0 = 0;
        wahwah->a1 = 0;
        wahwah->a2 = 0;
        wahwah->b0 = 0;
        wahwah->b1 = 0;
        wahwah->b2 = 0;
        wahwah->depth   = conf->depth / 100.0;
        wahwah->freqofs = conf->freqofs / 100.0;
        wahwah->phase   = conf->phase * M_PI / 180.0;
        wahwah->outgain = DB_TO_LINEAR(conf->gain);
        wahwah->conf    = conf;
    }
    return wahwah->conf;
}

bool wahwah_conf(wahwah_conf_t* conf) {
    bool ret = false;
    if (conf) {
        //double gain;    // output gain
        // 低频振荡频率
        if (conf->freq > WAHWAH_FREQ_MAX || conf->freq < WAHWAH_FREQ_MIN) {
            conf->freq = 1.5;
        }
        // 低频振荡开始相位
        if (conf->phase > WAHWAH_PHASE_MAX || conf->phase < WAHWAH_PHASE_MIN) {
            conf->phase = 0.0;
        }
        // 深度
        if (conf->depth > WAHWAH_DEPTH_MAX || conf->depth < WAHWAH_DEPTH_MIN) {
            conf->depth = 70;
        }
        // 共鸣
        if (conf->res > WAHWAH_RESONANCE_MAX || conf->res < WAHWAH_RESONANCE_MIN) {
            conf->res = 2.5;
        }
        // 哇哇频率偏移
        if (conf->freqofs > WAHWAH_OFFSET_MAX || conf->freqofs < WAHWAH_OFFSET_MIN) {
            conf->freqofs = 30;
        }
        // output gain
        if (conf->gain > WAHWAH_GAIN_MAX || conf->gain < WAHWAH_GAIN_MIN) {
            conf->gain = -6.0;
        }
        ret = true;
    }
    return ret;
}

void wahwah_process(float **inblock, float **outblock, int sample_rate) {
    float *ibuf = inblock[0];
    float *obuf = outblock[0];
    double frequency, omega, sn, cs, alpha;
    double in, out;

    wahwah->lfoskip = wahwah->conf->freq * 2 * M_PI / wahwah->sample_rate;
    wahwah->depth   = wahwah->conf->depth / 100.0;
    wahwah->freqofs = wahwah->conf->freqofs / 100.0;

    wahwah->phase = wahwah->conf->phase * M_PI / 180.0;
    wahwah->outgain = DB_TO_LINEAR(wahwah->conf->gain);

    for (int i = 0; i < sample_rate; i++) {
        in = (double) ibuf[i];

        if ((wahwah->skip_count++) % lfoskipsamples == 0) {
            frequency = (1 + cos(wahwah->skip_count * wahwah->lfoskip + wahwah->phase))
                    / 2;
            frequency = frequency * wahwah->depth * (1 - wahwah->freqofs) + wahwah->freqofs;
            frequency = exp((frequency - 1) * 6);
            omega = M_PI * frequency;
            sn = sin(omega);
            cs = cos(omega);
            alpha = sn / (2 * wahwah->conf->res);
            wahwah->b0 = (1 - cs) / 2;
            wahwah->b1 = 1 - cs;
            wahwah->b2 = (1 - cs) / 2;
            wahwah->a0 = 1 + alpha;
            wahwah->a1 = -2 * cs;
            wahwah->a2 = 1 - alpha;
        };
        out = (wahwah->b0 * in + wahwah->b1 * wahwah->xn1 + wahwah->b2 * wahwah->xn2 - wahwah->a1 * wahwah->yn1 - wahwah->a2 * wahwah->yn2) / wahwah->a0;
        wahwah->xn2 = wahwah->xn1;
        wahwah->xn1 = in;
        wahwah->yn2 = wahwah->yn1;
        wahwah->yn1 = out;
        out *= wahwah->outgain;

        obuf[i] = (float) out;
    }
}
