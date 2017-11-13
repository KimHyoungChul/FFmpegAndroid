#ifndef __VOICE_REMOVAL_H__
#define __VOICE_REMOVAL_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {

typedef struct {
    int sample_rate;
    int channels;
} voiceremoval_t;

void voiceremoval_init(int, int);
void voiceremoval_conf();
void voiceremoval_process(float**, float**, int);

}
#endif
