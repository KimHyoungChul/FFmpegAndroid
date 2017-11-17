/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_ring0_ffmpeg_FFmpegHelper */

#ifndef _Included_com_ring0_ffmpeg_FFmpegHelper
#define _Included_com_ring0_ffmpeg_FFmpegHelper
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_decoderVideoToYuv
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_decoderAudioToPcm
  (JNIEnv *, jclass, jstring, jstring, jint, jint, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1split
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv444p_1split
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1half
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1gray
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1board
  (JNIEnv *, jclass, jstring, jstring, jint, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1graybar
  (JNIEnv *, jclass, jstring, jint, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1psnr
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1rgb24_1split
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1rgb24_1bmp
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1rgb24_1yuv420p
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1rgb24_1colorbar
  (JNIEnv *, jclass, jstring, jint, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1split
  (JNIEnv *, jclass, jstring, jstring);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1lefthalf
  (JNIEnv *, jclass, jstring, jstring);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1doublespeed
  (JNIEnv *, jclass, jstring, jstring);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1pcm8le
  (JNIEnv *, jclass, jstring, jstring);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1corp
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1wave
  (JNIEnv *, jclass, jstring, jstring);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1player
  (JNIEnv *, jclass, jstring, jboolean, jobject);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1player_1init
  (JNIEnv *, jclass);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1player_1renderer
  (JNIEnv *, jclass);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1decoder
  (JNIEnv *, jclass, jstring, jstring, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1encoder
  (JNIEnv *, jclass, jstring, jstring, jint, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1audio_1player
  (JNIEnv *, jclass, jstring);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1video
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1picture
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm_1to_1aac
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1h264
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1h265
  (JNIEnv *, jclass, jstring, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1video_1audio_1remuxer
  (JNIEnv *, jclass, jstring, jstring);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1filter
  (JNIEnv *, jclass, jstring, jstring, jint);

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1spec_1filter
  (JNIEnv *, jclass, jstring, jstring, jstring, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
