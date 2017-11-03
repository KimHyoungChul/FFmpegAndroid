/* Copyright (c) 2011 The WebM project authors. All Rights Reserved. */
/*  */
/* Use of this source code is governed by a BSD-style license */
/* that can be found in the LICENSE file in the root of the source */
/* tree. An additional intellectual property rights grant can be found */
/* in the file PATENTS.  All contributing project authors may */
/* be found in the AUTHORS file in the root of the source tree. */
#include "vpx/vpx_codec.h"
static const char* const cfg = "--prefix=android/armeabi-v7a --target=armv7-android-gcc --sdk-path=/home/ring0/develop/android-ndk-r15c --enable-runtime-cpu-detect --disable-install-docs --disable-examples --disable-tools --disable-docs --enable-vp8 --enable-vp9";
const char *vpx_codec_build_config(void) {return cfg;}