#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef __SIMPLE_YUV__
#define __SIMPLE_YUV__

extern "C" {

void simple_yuv420p_split(char*, char*, int, int);
void simple_yuv444p_split(char*, char*, int, int);
void simple_yuv420p_half (char*, char*, int, int);
void simple_yuv420p_gray (char*, char*, int, int);
void simple_yuv420p_board(char*, char*, int, int, int);
void simple_yuv420p_graybar(char*, int, int, int);
void simple_yuv420p_psnr(char*, char*, int, int);
void simple_rgb24_split(char*, char*, int, int);
void simple_rgb24_bmp(char*, char*, int, int);
void simple_rgb24_yuv420p(char*, char*, int, int);
void simple_rgb24_colorbar(char*, int, int, int);

void simple_pcm16le_split(char*, char*);
void simple_pcm16le_lefthalf(char*, char*);
void simple_pcm16le_doublespeed(char*, char*);
void simple_pcm16le_pcm8le(char*, char*);
void simple_pcm16le_corp(char*, char*, int, int);
void simple_pcm16le_wave(char*, char*);
}

#endif
