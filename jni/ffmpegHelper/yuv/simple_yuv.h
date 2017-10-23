#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __SIMPLE_YUV__
#define __SIMPLE_YUV__

extern "C" {

void simple_yuv420p_split(char*, char*, int, int);
void simple_yuv444p_split(char*, char*, int, int);
void simple_yuv420p_half (char*, char*, int, int);
void simple_yuv420p_gray (char*, char*, int, int);
void simple_yuv420p_board(char*, char*, int, int, int);

}

#endif
