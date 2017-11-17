#include "com_ring0_ffmpeg_FFmpegHelper.h"
#include "simple_yuv.h"
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/buffersink.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
#include <libavutil/samplefmt.h>
#include <libavutil/opt.h>
}
#include <android/log.h>

// yuv type
#define FFMPEG_PIX_FMT_NV12    0
#define FFMPEG_PIX_FMT_NV16    1
#define FFMPEG_PIX_FMT_NV21    2
#define FFMPEG_PIX_FMT_YUV410P 3
#define FFMPEG_PIX_FMT_YUV411P 4
#define FFMPEG_PIX_FMT_YUV420P 5
#define FFMPEG_PIX_FMT_YUV422P 6
#define FFMPEG_PIX_FMT_YUV440P 7
#define FFMPEG_PIX_FMT_YUV444P 8
#define FFMPEG_PIX_FMT_RGB24   9
// pcm type
#define FFMPEG_SAMPLE_FMT_U8   0
#define FFMPEG_SAMPLE_FMT_S16  1
#define FFMPEG_SAMPLE_FMT_S32  2
#define FFMPEG_SAMPLE_FMT_FLT  3
#define FFMPEG_SAMPLE_FMT_DBL  4
#define FFMPEG_SAMPLE_FMT_S64  5
#define FFMPEG_SAMPLE_FMT_U8P  6
#define FFMPEG_SAMPLE_FMT_S16P 7
#define FFMPEG_SAMPLE_FMT_S32P 8
#define FFMPEG_SAMPLE_FMT_FLTP 9
#define FFMPEG_SAMPLE_FMT_DBLP 10
#define FFMPEG_SAMPLE_FMT_S64P 11
// decoder_type
#define FFMPEG_DECODER_MPEG4   0
#define FFMPEG_DECODER_H264    1
#define FFMPEG_DECODER_H265    2
#define FFMPEG_DECODER_VP8     3
#define FFMPEG_DECODER_VP9     4
// encoder_type
#define FFMPEG_ENCODER_MPEG4   0
#define FFMPEG_ENCODER_H264    1
#define FFMPEG_ENCODER_H265    2
#define FFMPEG_ENCODER_VP8     3
#define FFMPEG_ENCODER_VP9     4

static       SLObjectItf                   g_sl_object          = 0;
static       SLEngineItf                   g_sl_engine          = 0;
static       SLObjectItf                   g_sl_mixobject       = 0;
static       SLEnvironmentalReverbItf      g_sl_reverb          = 0;
static const SLEnvironmentalReverbSettings g_sl_reverb_settings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

static       SLObjectItf                   l_sl_player          = 0;
static       SLPlayItf                     l_sl_play            = 0;
static       SLSeekItf                     l_sl_seek            = 0;
static       SLMuteSoloItf                 l_sl_solo            = 0;
static       SLVolumeItf                   l_sl_volume          = 0;
static       SLAndroidSimpleBufferQueueItf l_sl_queue           = 0;
static       SLEffectSendItf               l_sl_effect          = 0;

static       AVFormatContext              *pFormatCtx           = 0;
static       AVCodecContext               *pCodecCtx            = 0;
static       AVCodec                      *pCodec               = 0;
static       AVFrame                      *pFrame               = 0;
static       AVPacket                     *pPacket              = 0;
static       SwrContext                   *pSwrCtx              = 0;
static       int                           audio_index          = -1;
static       int                           got_picture          = 0;

static       AVSampleFormat                out_sample_fmt       = AV_SAMPLE_FMT_U8;
static       int                           out_sample_rate      = 44100;
static       int                           out_channel_layout   = 2;

struct Data {
    void *buffer;
    int   size;
};
/**
 *  返回 ffmpeg 中的像素格式
 */
AVPixelFormat get_yuv_type(int yuv_type) {
    AVPixelFormat format = AV_PIX_FMT_NONE;
    switch (yuv_type) {
    case FFMPEG_PIX_FMT_NV12:
        format = AV_PIX_FMT_NV12;
        break;
    case FFMPEG_PIX_FMT_NV16:
        format = AV_PIX_FMT_NV16;
        break;
    case FFMPEG_PIX_FMT_NV21:
        format = AV_PIX_FMT_NV21;
        break;
    case FFMPEG_PIX_FMT_YUV410P:
        format = AV_PIX_FMT_YUV410P;
        break;
    case FFMPEG_PIX_FMT_YUV411P:
        format = AV_PIX_FMT_YUV411P;
        break;
    case FFMPEG_PIX_FMT_YUV420P:
        format = AV_PIX_FMT_YUV420P;
        break;
    case FFMPEG_PIX_FMT_YUV422P:
        format = AV_PIX_FMT_YUV422P;
        break;
    case FFMPEG_PIX_FMT_YUV440P:
        format = AV_PIX_FMT_YUV440P;
        break;
    case FFMPEG_PIX_FMT_YUV444P:
        format = AV_PIX_FMT_YUV444P;
        break;
    case FFMPEG_PIX_FMT_RGB24:
        format = AV_PIX_FMT_RGB24;
        break;
    }
    return format;
}
/**
 *  返回 ffmpeg 中的采样格式
 */
AVSampleFormat get_pcm_type(int pcm_type) {
    AVSampleFormat format = AV_SAMPLE_FMT_NONE;
    switch (pcm_type) {
    case FFMPEG_SAMPLE_FMT_U8:
        format = AV_SAMPLE_FMT_U8;
        break;
    case FFMPEG_SAMPLE_FMT_S16:
        format = AV_SAMPLE_FMT_S16;
        break;
    case FFMPEG_SAMPLE_FMT_S32:
        format = AV_SAMPLE_FMT_S32;
        break;
    case FFMPEG_SAMPLE_FMT_FLT:
        format = AV_SAMPLE_FMT_FLT;
        break;
    case FFMPEG_SAMPLE_FMT_DBL:
        format = AV_SAMPLE_FMT_DBL;
        break;
    case FFMPEG_SAMPLE_FMT_S64:
        format = AV_SAMPLE_FMT_S64;
        break;
    case FFMPEG_SAMPLE_FMT_U8P:
        format = AV_SAMPLE_FMT_U8P;
        break;
    case FFMPEG_SAMPLE_FMT_S16P:
        format = AV_SAMPLE_FMT_S16P;
        break;
    case FFMPEG_SAMPLE_FMT_S32P:
        format = AV_SAMPLE_FMT_S32P;
        break;
    case FFMPEG_SAMPLE_FMT_FLTP:
        format = AV_SAMPLE_FMT_FLTP;
        break;
    case FFMPEG_SAMPLE_FMT_DBLP:
        format = AV_SAMPLE_FMT_DBLP;
        break;
    case FFMPEG_SAMPLE_FMT_S64P:
        format = AV_SAMPLE_FMT_S64P;
        break;
    }
    return format;
}

AVCodecID get_decoder_type(int decoder_type) {
    AVCodecID result = AV_CODEC_ID_NONE;
    switch (decoder_type) {
    case FFMPEG_DECODER_MPEG4:
        result = AV_CODEC_ID_MPEG4;
        break;
    case FFMPEG_DECODER_H264:
        result = AV_CODEC_ID_H264;
        break;
    case FFMPEG_DECODER_H265:
        result = AV_CODEC_ID_H265;
        break;
    case FFMPEG_DECODER_VP8:
        result = AV_CODEC_ID_VP8;
        break;
    case FFMPEG_DECODER_VP9:
        result = AV_CODEC_ID_VP9;
        break;
    }
    return result;
}
AVCodecID get_encoder_type(int encoder_type) {
    AVCodecID result = AV_CODEC_ID_NONE;
    switch (encoder_type) {
    case FFMPEG_ENCODER_MPEG4:
        result = AV_CODEC_ID_MPEG4;
        break;
    case FFMPEG_ENCODER_H264:
        result = AV_CODEC_ID_H264;
        break;
    case FFMPEG_ENCODER_H265:
        result = AV_CODEC_ID_H265;
        break;
    case FFMPEG_ENCODER_VP8:
        result = AV_CODEC_ID_VP8;
        break;
    case FFMPEG_ENCODER_VP9:
        result = AV_CODEC_ID_VP9;
        break;
    }
    return result;
}
/**
 *  返回 ffmpeg 的像素描述字符串
 */
char* get_yuv_char(int yuv_type) {
    char *result = 0;
    switch (yuv_type) {
    case FFMPEG_PIX_FMT_NV12:
        result = "nv12";
        break;
    case FFMPEG_PIX_FMT_NV16:
        result = "nv16";
        break;
    case FFMPEG_PIX_FMT_NV21:
        result = "nv21";
        break;
    case FFMPEG_PIX_FMT_YUV410P:
        result = "yuv410p";
        break;
    case FFMPEG_PIX_FMT_YUV411P:
        result = "yuv411p";
        break;
    case FFMPEG_PIX_FMT_YUV420P:
        result = "yuv420p";
        break;
    case FFMPEG_PIX_FMT_YUV422P:
        result = "yuv422p";
        break;
    case FFMPEG_PIX_FMT_YUV440P:
        result = "yuv440p";
        break;
    case FFMPEG_PIX_FMT_YUV444P:
        result = "yuv444p";
        break;
    case FFMPEG_PIX_FMT_RGB24:
        result = "rgb24";
        break;
    }
    return result;
}
/**
 *  返回 ffmepg 采样字符串
 */
char* get_pcm_char(int pcm_type) {
    char* result = 0;
    switch (pcm_type) {
    case FFMPEG_SAMPLE_FMT_U8:
        result = "u8";
        break;
    case FFMPEG_SAMPLE_FMT_S16:
        result = "s16";
        break;
    case FFMPEG_SAMPLE_FMT_S32:
        result = "s32";
        break;
    case FFMPEG_SAMPLE_FMT_FLT:
        result = "flt";
        break;
    case FFMPEG_SAMPLE_FMT_DBL:
        result = "dbl";
        break;
    case FFMPEG_SAMPLE_FMT_S64:
        result = "s64";
        break;
    case FFMPEG_SAMPLE_FMT_U8P:
        result = "u8p";
        break;
    case FFMPEG_SAMPLE_FMT_S16P:
        result = "s16p";
        break;
    case FFMPEG_SAMPLE_FMT_S32P:
        result = "s32p";
        break;
    case FFMPEG_SAMPLE_FMT_FLTP:
        result = "fltp";
        break;
    case FFMPEG_SAMPLE_FMT_DBLP:
        result = "dblp";
        break;
    case FFMPEG_SAMPLE_FMT_S64P:
        result = "s64p";
        break;
    }
    return result;
}
char* get_decoder_char(int decoder_type) {
    char *result = "";
    switch (decoder_type) {
    case FFMPEG_DECODER_MPEG4:
        result = "mpeg4";
        break;
    case FFMPEG_DECODER_H264:
        result = "h264";
        break;
    case FFMPEG_DECODER_H265:
        result = "h265";
        break;
    case FFMPEG_DECODER_VP8:
        result = "vp8";
        break;
    case FFMPEG_DECODER_VP9:
        result = "vp9";
        break;
    }
    return result;
}
char* get_encoder_char(int encoder_type) {
    char *result = "";
    switch (encoder_type) {
    case FFMPEG_ENCODER_MPEG4:
        result = "mpeg4";
        break;
    case FFMPEG_ENCODER_H264:
        result = "h264";
        break;
    case FFMPEG_ENCODER_H265:
        result = "h265";
        break;
    case FFMPEG_ENCODER_VP8:
        result = "vp8";
        break;
    case FFMPEG_ENCODER_VP9:
        result = "vp9";
        break;
    }
    return result;
}

void android_opensles_create_engine();
void android_opensles_stop();
void android_opensles_play_buffer(int, int);
void android_opensles_callback(SLAndroidSimpleBufferQueueItf, void*);

void ff_log_callback(void *ptr, int level, const char*fmt, va_list va) {
    char *info = (char*)malloc(sizeof(char) * 4096);
    vsprintf(info, fmt, va);
    __android_log_write(ANDROID_LOG_INFO, "zd-ffmpeg", info);
    free(info);
}
/**
 *  libffmpegHelper.so 被加载
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    android_opensles_create_engine();
    return JNI_VERSION_1_4;
}
/**
 *  libffmpegHelper.so 被卸载
 */
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    android_opensles_stop();
}
/**
 *  将媒体文件分离出 yuv 像素数据
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_decoderVideoToYuv
  (JNIEnv *env, jclass, jstring jfilename, jstring jpath, jint frame, jint yuv_type) {
    if (!jfilename) {
        return;
    }
    if (!jpath) {
        return;
    }
    char *filename = (char*)env->GetStringUTFChars(jfilename, 0);
    char *path = (char*)env->GetStringUTFChars(jpath, 0);
    AVFormatContext   *pFormatCtx  =  0;
    AVCodecContext    *pCodecCtx   =  0;
    AVCodec           *pCodec      =  0;
    AVFrame           *pSrc        =  0;
    AVFrame           *pDst        =  0;
    AVPacket          *pPck        =  0;
    SwsContext        *pSwsCtx     =  0;
    uint8_t           *out         =  0;
    int                video_index = -1;
    int                got_picture =  0;
    int                width       =  0;
    int                height      =  0;
    int                length      =  0;
    int                dec_cnt     =  0;
    AVPixelFormat      pix_fmt     = get_yuv_type(yuv_type);
    av_register_all();
    avcodec_register_all();
    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, filename, 0, 0) != 0) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not open input filename!");
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, 0) < 0) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not found stream info!");
        return;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            break;
        }
    }
    if (video_index == -1) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not found video stream!");
        return;
    }
    pCodecCtx = pFormatCtx->streams[video_index]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == 0) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not find decoder");
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not open decoder");
        return;
    }
    pPck = (AVPacket*)av_malloc(sizeof(AVPacket));
    pSrc = av_frame_alloc();
    pDst = av_frame_alloc();
    width  = pCodecCtx->width;
    height = pCodecCtx->height;
    length = av_image_get_buffer_size(pix_fmt, width, height, 1);
    out = (unsigned char*)av_malloc(length);
    av_image_fill_arrays(pDst->data, pDst->linesize, (const unsigned char*)out, pix_fmt, width, height, 1);
    av_init_packet(pPck);
    pSwsCtx = sws_getContext(width, height, pCodecCtx->pix_fmt, width, height, pix_fmt, SWS_BICUBIC, 0, 0, 0);
    while (av_read_frame(pFormatCtx, pPck) >= 0) {
        if (pPck->stream_index == video_index) {
            if (avcodec_decode_video2(pCodecCtx, pSrc, &got_picture, pPck) >= 0) {
                if (got_picture) {
                    dec_cnt++;
                    if (dec_cnt <= frame) {
                        sws_scale(pSwsCtx, (const uint8_t* const *)pSrc->data, pSrc->linesize, 0, height, pDst->data, pDst->linesize);
                        // 格式化文件名
                        char *yuv_file = (char*)malloc(sizeof(char) * 4096);
                        sprintf(yuv_file, "%s/%s_%dx%d_%d.yuv",
                                path, get_yuv_char(yuv_type), width, height, dec_cnt);
                        // 保存视频像素数据
                        if (pix_fmt == AV_PIX_FMT_YUV420P) {
                            FILE *f = fopen(yuv_file, "wb+");
                            fwrite(pDst->data[0], 1, width * height, f);
                            fwrite(pDst->data[1], 1, (width * height) / 4, f);
                            fwrite(pDst->data[2], 1, (width * height) / 4, f);
                            fclose(f);
                        }
                        else if (pix_fmt == AV_PIX_FMT_YUV444P) {
                            FILE *f = fopen(yuv_file, "wb+");
                            fwrite(pDst->data[0], 1, width * height, f);
                            fwrite(pDst->data[1], 1, width * height, f);
                            fwrite(pDst->data[2], 1, width * height, f);
                            fclose(f);
                        }
                        else if (pix_fmt == AV_PIX_FMT_RGB24) {
                            FILE *f = fopen(yuv_file, "wb+");
                            fwrite(pDst->data[0], 1, width * height * 3, f);
                            fclose(f);
                        }
                        free(yuv_file);
                    }
                    else {
                        av_free_packet(pPck); // 这里防止最后一个包没有释放
                        break;
                    }
                }
            }
        }
        av_free_packet(pPck);
    }
    sws_freeContext(pSwsCtx);
    av_frame_free(&pSrc);
    av_frame_free(&pDst);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    env->ReleaseStringUTFChars(jfilename, filename);
    env->ReleaseStringUTFChars(jpath, path);
}
/**
 *  将媒体文件分理出 pcm 采样数据
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_decoderAudioToPcm
  (JNIEnv *env, jclass, jstring jfilename, jstring jpath, jint frame, jint channel, jint sample, jint pcm_type) {
    if (!jfilename) {
        return;
    }
    if (!jpath) {
        return;
    }
    char *filename = (char*)env->GetStringUTFChars(jfilename, 0);
    char *path = (char*)env->GetStringUTFChars(jpath, 0);

    AVFormatContext *pFormatCtx  =  0;
    AVCodecContext  *pCodecCtx   =  0;
    AVCodec         *pCodec      =  0;
    AVFrame         *pSrc        =  0;
    AVPacket        *pPck        =  0;
    SwrContext      *pSwrCtx     =  0;
    uint8_t         *out         =  0;
    int              audio_index = -1;
    int              got_picture =  0;
    int              dec_cnt     =  0;
    AVSampleFormat   sample_fmt  = get_pcm_type(pcm_type);

    av_register_all();
    avcodec_register_all();
    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, filename, 0, 0) != 0) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not open input file!");
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, 0) < 0) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not find stream info!");
        return;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_index = i;
            break;
        }
    }
    if (audio_index == -1) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not find audio stream!");
        return;
    }
    pCodecCtx = pFormatCtx->streams[audio_index]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (!pCodec) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not find decoder!");
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        __android_log_write(ANDROID_LOG_INFO, "zd-info", "not open decoder!");
        return;
    }
    pSrc = av_frame_alloc();
    pPck = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(pPck);
    pSwrCtx = swr_alloc();
    av_opt_set_int(pSwrCtx, "in_channel_layout",  pCodecCtx->channel_layout, 0);
    av_opt_set_int(pSwrCtx, "in_sample_rate",     pCodecCtx->sample_rate,    0);
    av_opt_set_int(pSwrCtx, "in_sample_fmt",      pCodecCtx->sample_fmt,     0);
    av_opt_set_int(pSwrCtx, "out_channel_layout", channel,                   0);
    av_opt_set_int(pSwrCtx, "out_sample_rate",    sample,                    0);
    av_opt_set_int(pSwrCtx, "out_sample_fmt",     sample_fmt,                0);
    swr_init(pSwrCtx);
    while (av_read_frame(pFormatCtx, pPck) >= 0) {
        if (pPck->stream_index == audio_index) {
            if (avcodec_decode_audio4(pCodecCtx, pSrc, &got_picture, pPck) >= 0) {
                if (got_picture) {
                    dec_cnt++;
                    if (dec_cnt <= frame) {
                        int length = av_samples_get_buffer_size(pSrc->linesize, channel, sample, sample_fmt, 1);
                        out = (uint8_t*)malloc(sizeof(uint8_t) * length);
                        swr_convert(pSwrCtx, &out, sample, (uint8_t const**)pSrc->extended_data, pSrc->nb_samples);
                        // 格式化文件名
                        char *file = (char*)malloc(sizeof(char) * 4096);
                        sprintf(file, "%s/%s_%d_%d_%d.pcm", path, get_pcm_char(pcm_type), channel, sample, dec_cnt);
                        // 保存音频像素数据
                        FILE *f = fopen(file, "wb+");
                        fwrite(out, 1, length, f);
                        fclose(f);
                        free(file);
                        free(out);
                    }
                    else {
                        av_free_packet(pPck);
                        break;
                    }
                }
            }
        }
        av_free_packet(pPck);
    }
    av_frame_free(&pSrc);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    swr_close(pSwrCtx);
    env->ReleaseStringUTFChars(jfilename, filename);
    env->ReleaseStringUTFChars(jpath, path);
}
/**
 *  将 yuv420p 分离出 y u v 三分量
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1split
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jpath, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *path = (char*)env->GetStringUTFChars(jpath, 0);

    simple_yuv420p_split(srcfile, path, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jpath, path);
}
/**
 *  将 yuv444p 分离出 y u v 三分量
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv444p_1split
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jpath, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *path = (char*)env->GetStringUTFChars(jpath, 0);

    simple_yuv444p_split(srcfile, path, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jpath, path);
}
/**
 *  将 yuv420p 像素数据的亮度减半
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1half
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_yuv420p_half(srcfile, dstfile, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}
/**
 *  将 yuv420p 像素数据去掉颜色
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1gray
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_yuv420p_gray(srcfile, dstfile, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}
/**
 *  将 yuv420p 加上边框
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1board
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile, jint board, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_yuv420p_board(srcfile, dstfile, board, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}
/**
 *  生成 yuv420p 灰阶图
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1graybar
  (JNIEnv * env, jclass, jstring jdstfile, jint barsize, jint width, jint height) {
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_yuv420p_graybar(dstfile, barsize, width, height);

    env->ReleaseStringUTFChars(jdstfile, dstfile);
}
/**
 *  计算两个 yuv420p 的 psnr
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1psnr
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_yuv420p_psnr(srcfile, dstfile, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}
/**
 *  将 rgb24 分离为 r g b 三分量
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1rgb24_1split
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jpath, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *path = (char*)env->GetStringUTFChars(jpath, 0);

    simple_rgb24_split(srcfile, path, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jpath, path);
}
/**
 *  将 rgb24 封装为 bmp 文件
 */
JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1rgb24_1bmp
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_rgb24_bmp(srcfile, dstfile, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1rgb24_1yuv420p
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile, jint width, jint height) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_rgb24_yuv420p(srcfile, dstfile, width, height);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1rgb24_1colorbar
  (JNIEnv *env, jclass, jstring jdstfile, jint barsize, jint width, jint height) {
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_rgb24_colorbar(dstfile, barsize, width, height);

    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1split
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jpath) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *path = (char*)env->GetStringUTFChars(jpath, 0);

    simple_pcm16le_split(srcfile, path);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jpath, path);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1lefthalf
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_pcm16le_lefthalf(srcfile, dstfile);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1doublespeed
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_pcm16le_doublespeed(srcfile, dstfile);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1pcm8le
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_pcm16le_pcm8le(srcfile, dstfile);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1corp
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile, jint startpoint, jint count) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_pcm16le_corp(srcfile, dstfile, startpoint, count);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm16le_1wave
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    simple_pcm16le_wave(srcfile, dstfile);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

struct Frame {
    int   width;
    int   height;
    char *frame;
};

static jclass           FFmpegPlayerInterface;
static jmethodID        FFOnRenderer;
static pthread_mutex_t  mutex;
static std::list<Frame*> frames;
static int frame_width;
static int frame_height;

const char *vertex_shader =
     "attribute vec4 vertexIn;   \n"
     "attribute vec2 textureIn;  \n"
     "varying   vec2 textureOut; \n"
     "void main(void)            \n"
     "{                          \n"
     "  gl_Position = vertexIn;  \n"
     "  textureOut = textureIn;  \n"
     "}                          \n";

const char *frag_shader =
     "precision mediump float;                                                \n"
     "uniform   sampler2D tex_y;                                              \n"
     "uniform   sampler2D tex_u;                                              \n"
     "uniform   sampler2D tex_v;                                              \n"
     "varying   vec2      textureOut;                                         \n"
     "void main()                                                             \n"
     "{                                                                       \n"
     "   vec4 c = vec4((texture2D(tex_y, textureOut).r - 16./255.) * 1.164);  \n"
     "   vec4 U = vec4( texture2D(tex_u, textureOut).r - 128./255.);          \n"
     "   vec4 V = vec4( texture2D(tex_v, textureOut).r - 128./255.);          \n"
     "   c += V * vec4(1.596, -0.813, 0, 0);                                  \n"
     "   c += U * vec4(0, -0.392, 2.017, 0);                                  \n"
     "   c.a = 1.0;                                                           \n"
     "   gl_FragColor = c;                                                    \n"
     "}                                                                       \n";

static GLfloat vertexVertices[] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f
};

static GLfloat textureVertices[] = {
     0.0f,  1.0f,
     1.0f,  1.0f,
     0.0f,  0.0f,
     1.0f,  0.0f
};

GLuint  texture_y, texture_u, texture_v;
GLuint  textureUniformY, textureUniformU, textureUniformV;
#define GL_ATTRIB_VERTEX  3
#define GL_ATTRIB_TEXTURE 4

void setupShader(int width, int height) {
    GLint vertCompiled, fragCompiled, linked;
    GLint v = glCreateShader(GL_VERTEX_SHADER);
    GLint f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(v, 1, &vertex_shader, 0);
    glShaderSource(f, 1, &frag_shader, 0);

    glCompileShader(v);
    glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);

    glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);

    GLuint program = glCreateProgram();
    glAttachShader(program, v);
    glAttachShader(program, f);

    glBindAttribLocation(program, GL_ATTRIB_VERTEX, "vertexIn");
    glBindAttribLocation(program, GL_ATTRIB_TEXTURE, "textureIn");
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    glUseProgram(program);

    textureUniformY = glGetUniformLocation(program, "tex_y");
    textureUniformU = glGetUniformLocation(program, "tex_u");
    textureUniformV = glGetUniformLocation(program, "tex_v");
    // 设置数据
    glVertexAttribPointer(GL_ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
    glVertexAttribPointer(GL_ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
    glEnableVertexAttribArray(GL_ATTRIB_VERTEX);
    glEnableVertexAttribArray(GL_ATTRIB_TEXTURE);

    // 初始化贴图
    glGenTextures(1, &texture_y);
    glBindTexture(GL_TEXTURE_2D, texture_y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);

    glGenTextures(1, &texture_u);
    glBindTexture(GL_TEXTURE_2D, texture_u);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0,
                GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);

    glGenTextures(1, &texture_v);
    glBindTexture(GL_TEXTURE_2D, texture_v);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0,
                GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
}

void setupTexture(char *buffer, int width, int height) {
    int uvwidth  = width  / 2;
    int uvheight = height / 2;

    char *y_pixel = buffer;
    char *u_pixel = (char*)y_pixel + (width * height);
    char *v_pixel = (char*)u_pixel + (uvwidth * uvheight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // y
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_y);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, y_pixel);
    glUniform1i(textureUniformY, 0);
    // u
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_u);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, uvwidth, uvheight, GL_LUMINANCE, GL_UNSIGNED_BYTE, u_pixel);
    glUniform1i(textureUniformU, 1);
    // v
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_v);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, uvwidth, uvheight, GL_LUMINANCE, GL_UNSIGNED_BYTE, v_pixel);
    glUniform1i(textureUniformV, 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
}

void FFmpeg_OnRenderer(JNIEnv *env, jobject jinterface, int width, int height, AVFrame *frame) {
    char *buffer = (char*) malloc(width * height * 3 / 2);
    char *frame_y = buffer;
    char *frame_u = frame_y + (width * height);
    char *frame_v = frame_u + ((width * height) / 4);
    memcpy(frame_y, frame->data[0], width * height);
    memcpy(frame_u, frame->data[1], (width * height) / 4);
    memcpy(frame_v, frame->data[2], (width * height) / 4);

    Frame *f = (Frame*)malloc(sizeof(Frame));
    f->width  = width;
    f->height = height;
    f->frame  = buffer;

    pthread_mutex_lock(&mutex);
    frames.push_back(f);
    pthread_mutex_unlock(&mutex);

    env->CallVoidMethod(jinterface, FFOnRenderer);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1player_1init
  (JNIEnv *env, jclass) {
    jobject jlocal        = env->FindClass("com/ring0/ffmpeg/FFmpegHelper$FFmpegPlayerInterface");
    FFmpegPlayerInterface = (jclass)env->NewGlobalRef(jlocal);
    FFOnRenderer          = env->GetMethodID(FFmpegPlayerInterface, "OnRenderer", "()V");
    env->DeleteLocalRef(jlocal);

    JavaVM *vm = 0;
    env->GetJavaVM((JavaVM**)&vm);
    av_jni_set_java_vm(vm, 0);

    frame_width  = 0;
    frame_height = 0;
    pthread_mutex_init(&mutex, 0);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1player_1renderer
  (JNIEnv *, jclass) {
    // get Frame
    Frame *frame = 0;
    pthread_mutex_lock(&mutex);
    if (!frames.empty()) {
        frame = frames.front();
        frames.pop_front();
    }
    pthread_mutex_unlock(&mutex);
    // renderer
    if (frame) {
        if (frame->width != frame_width || frame->height != frame_height) {
            frame_width  = frame->width;
            frame_height = frame->height;
            setupShader(frame_width, frame_height);
        }
        if (frame->frame) {
            setupTexture(frame->frame, frame_width, frame_height);
        }
        free(frame->frame);
        free(frame);
        frame = 0;
    }
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1player
  (JNIEnv *env, jclass, jstring jfilename, jboolean hard, jobject jinterface) {
    char *filename = (char*)env->GetStringUTFChars(jfilename, 0);

    AVFormatContext *pFormatCtx  =  0;
    AVCodecContext  *pCodecCtx   =  0;
    AVCodec         *pCodec      =  0;
    AVFrame         *pFrame      =  0;
    AVFrame         *pFrameYuv   =  0;
    AVPacket        *pPackage    =  0;
    SwsContext      *pSwsCtx     =  0;
    int              got_picture =  0;
    int              video_index = -1;
    uint8_t         *out = 0;
    av_register_all();
    avformat_network_init();
    avcodec_register_all();
    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, filename, 0, 0) != 0) {
        // avformat_open_input error
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, 0) < 0) {
        // avformat_find_stream_info error
        return;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            break;
        }
    }
    if (video_index == -1) {
        // not found video stream
        return;
    }
    pCodecCtx = pFormatCtx->streams[video_index]->codec;
    if (hard) {
        // 查询可用的硬件解码器
        switch (pCodecCtx->codec_id) {
        case AV_CODEC_ID_MPEG4:
            pCodec = avcodec_find_decoder_by_name("mpeg4_mediacodec");
            break;
        case AV_CODEC_ID_H264:
            pCodec = avcodec_find_decoder_by_name("h264_mediacodec");
            break;
        case AV_CODEC_ID_H265: // AV_CODEC_ID_HEVC
            pCodec = avcodec_find_decoder_by_name("hevc_mediacodec");
            break;
        case AV_CODEC_ID_VP8:
            pCodec = avcodec_find_decoder_by_name("vp8_mediacodec");
            break;
        case AV_CODEC_ID_VP9:
            pCodec = avcodec_find_decoder_by_name("vp9_mediacodec");
            break;
        default:
            pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
            break;
        }
    }
    else {
        // 软件解码
        pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    }

    if (pCodec == 0) {
        // not found coded decoder
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        // avcodec_open2 error
        return;
    }
    pPackage = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(pPackage);
    pFrame = av_frame_alloc();
    pFrameYuv = av_frame_alloc();
    out = (uint8_t*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameYuv->data, pFrameYuv->linesize,
            (const uint8_t*)out,
            AV_PIX_FMT_YUV420P,
            pCodecCtx->width,
            pCodecCtx->height, 1);
    pSwsCtx = sws_getContext(
            pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
            pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
            SWS_BICUBIC, 0, 0, 0);

    // 回调到 java 层
    while (av_read_frame(pFormatCtx, pPackage) >= 0) {
        if (pPackage->stream_index == video_index) {
            int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pPackage);
            if (ret >= 0 && got_picture) {
                sws_scale(
                        pSwsCtx,
                        (const uint8_t* const*)pFrame->data, pFrame->linesize,
                        0, pCodecCtx->height,
                        pFrameYuv->data, pFrameYuv->linesize);
                FFmpeg_OnRenderer(
                        env, jinterface,
                        pCodecCtx->width, pCodecCtx->height, pFrameYuv);
            }
        }
        av_free_packet(pPackage);
    }
    while (1) {
        int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pPackage);
        if (ret < 0) {
            break;
        }
        if (!got_picture) {
            break;
        }
        sws_scale(pSwsCtx, (const uint8_t* const *) pFrame->data,
                pFrame->linesize, 0, pCodecCtx->height, pFrameYuv->data,
                pFrameYuv->linesize);
        FFmpeg_OnRenderer(
                env, jinterface,
                pCodecCtx->width, pCodecCtx->height, pFrameYuv);
        av_free_packet(pPackage);
    }
    av_frame_free(&pFrame);
    av_frame_free(&pFrameYuv);
    sws_freeContext(pSwsCtx);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    env->ReleaseStringUTFChars(jfilename, filename);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1decoder
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jpath, jint decoder_type) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *path    = (char*)env->GetStringUTFChars(jpath, 0);

    char *dstfile = (char*)malloc(sizeof(char) * 1024);
    sprintf(dstfile, "%s/%s.yuv", path, get_decoder_char(decoder_type));

    FILE *file_src = fopen(srcfile, "rb+");
    FILE *file_dst = fopen(dstfile, "wb+");
    int   in_size = 4096;
    char *in_buf = (char*)malloc(in_size + FF_INPUT_BUFFER_PADDING_SIZE);
    int   cur_size = 0;
    char *cur_buf = 0;

    AVCodecParserContext *pParserCtx = 0;
    AVCodecContext       *pCodecCtx  = 0;
    AVCodec              *pCodec     = 0;
    AVFrame              *pFrame     = 0;
    AVPacket             *pPacket    = 0;
    AVCodecID             codec_id   = get_decoder_type(decoder_type);

    avcodec_register_all();
    pCodec = avcodec_find_decoder(codec_id);
    pCodecCtx = avcodec_alloc_context3(pCodec);
    pParserCtx = av_parser_init(codec_id);
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        // avcodec_open2 error
        return;
    }
    pFrame = av_frame_alloc();
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(pPacket);

    while (1) {
        cur_size = fread(in_buf, 1, in_size, file_src);
        cur_buf  = in_buf;
        if (cur_size == 0) {
            break;
        }
        while (cur_size > 0) {
            int len = av_parser_parse2(
                    pParserCtx, pCodecCtx,
                    &pPacket->data, &pPacket->size,
                    (const unsigned char*)cur_buf, cur_size,
                    AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
            cur_buf  += len;
            cur_size -= len;
            if (pPacket->size == 0) {
                continue;
            }
            int got_picture = 0;
            int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pPacket);
            if (ret < 0) {
                break;
            }
            if (!got_picture) {
                break;
            }
            __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "decoder frame");
            // save y
            for (int i = 0; i < pFrame->height; i++) {
                fwrite(pFrame->data[0] + (pFrame->linesize[0] * i), 1, pFrame->linesize[0], file_dst);
            }
            // save u
            for (int i = 0; i < pFrame->height / 2; i++) {
                fwrite(pFrame->data[1] + (pFrame->linesize[1] * i), 1, pFrame->linesize[1], file_dst);
            }
            // save v
            for (int i = 0; i < pFrame->height / 2; i++) {
                fwrite(pFrame->data[2] + (pFrame->linesize[2] * i), 1, pFrame->linesize[2], file_dst);
            }
        }
    }
    pPacket->data = 0;
    pPacket->size = 0;
    while (1) {
        int got_picture = 0;
        int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pPacket);
        if (ret < 0) {
            break;
        }
        if (!got_picture) {
            break;
        }
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "flush decoder frame");
        // save y
        for (int i = 0; i < pFrame->height; i++) {
            fread(pFrame->data[0] + (pFrame->linesize[0] * i), 1, pFrame->linesize[0], file_dst);
        }
        // save u
        for (int i = 0; i < pFrame->height / 2; i++) {
            fread(pFrame->data[1] + (pFrame->linesize[1] * i), 1, pFrame->linesize[1], file_dst);
        }
        // save u
        for (int i = 0; i < pFrame->height / 2; i++) {
            fread(pFrame->data[2] + (pFrame->linesize[2] * i), 1, pFrame->linesize[2], file_dst);
        }
    }
    free(dstfile);
    free(in_buf);
    fclose(file_src);
    fclose(file_dst);
    av_free_packet(pPacket);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    av_parser_close(pParserCtx);
    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jpath, path);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1encoder
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jpath, jint width, jint height, jint encoder_type) {

    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *path = (char*)env->GetStringUTFChars(jpath, 0);

    char *dstfile = (char*)malloc(sizeof(char) * 1024);
    sprintf(dstfile, "%s/%s_%dx%d.%s", path, get_encoder_char(encoder_type), width, height, get_encoder_char(encoder_type));
    FILE *file_src = fopen(srcfile, "rb+");
    FILE *file_dst = fopen(dstfile, "wb+");

    AVCodecContext *pCodecCtx   = 0;
    AVCodec        *pCodec      = 0;
    AVFrame        *pFrame      = 0;
    AVPacket       *pPacket     = 0;
    AVCodecID       codec_id    = get_encoder_type(encoder_type);
    int             frame_cnt   = 0;
    int             got_picture = 0;
    avcodec_register_all();
    pCodec = avcodec_find_encoder(codec_id);
    if (!pCodec) {
        // avcodec_find_encoder error;
        return;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        // avcodec_alloc_context3 error
        return;
    }
    pCodecCtx->bit_rate      = 400000;
    pCodecCtx->width         = width;
    pCodecCtx->height        = height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->gop_size      = 10;
    pCodecCtx->max_b_frames  = 1;
    pCodecCtx->pix_fmt       = AV_PIX_FMT_YUV420P;
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        // avcodec_open2 error;
        return;
    }
    if (codec_id == AV_CODEC_ID_H264) {
        av_opt_set(pCodecCtx->priv_data, "preset", "slow", 0);
    }
    pFrame = av_frame_alloc();
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_image_alloc(pFrame->data, pFrame->linesize, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, 16);
    // 读取数据并且编码
    while (1) {
    	if (fread(pFrame->data[0], 1, width * height, file_src) <= 0 ||
    			fread(pFrame->data[1], 1, (width * height) / 4, file_src) <= 0 ||
    			fread(pFrame->data[2], 1, (width * height) / 4, file_src) <= 0) {
    		break;
    	}
        pFrame->width  = width;
        pFrame->height = height;
        pFrame->format = AV_PIX_FMT_YUV420P;
        pFrame->pts    = frame_cnt;

        av_init_packet(pPacket);
        pPacket->data = 0;
        pPacket->size = 0;
        int ret = avcodec_encode_video2(pCodecCtx, pPacket, pFrame, &got_picture);
        if (ret < 0) {
            break;
        }
        if (got_picture) {
			frame_cnt++;
			fwrite(pPacket->data, 1, pPacket->size, file_dst);
			av_free_packet(pPacket);
        }
    }
    while (1) {
        int ret = avcodec_encode_video2(pCodecCtx, pPacket, pFrame, &got_picture);
        if (ret < 0) {
            break;
        }
        if (got_picture) {
			fwrite(pPacket->data, 1, pPacket->size, file_dst);
			av_free_packet(pPacket);
        }
    }

    avcodec_close(pCodecCtx);
    av_frame_free(&pFrame);
    av_packet_free(&pPacket);
    fclose(file_src);
    fclose(file_dst);
    free(dstfile);
    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jpath, path);
}

void android_opensles_create_engine() {
    // 创建播放引擎
    slCreateEngine(&g_sl_object, 0, 0, 0, 0, 0);
    // 非同步
    (*g_sl_object)->Realize(g_sl_object, SL_BOOLEAN_FALSE);
    // 获取 opensles SLEngineITF 的对象
    (*g_sl_object)->GetInterface(g_sl_object, SL_IID_ENGINE, &g_sl_engine);

    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean     req[1] = {SL_BOOLEAN_FALSE};
    // 获取混音器
    (*g_sl_engine)->CreateOutputMix(g_sl_engine, &g_sl_mixobject, 1, ids, req);
    // 初始化混音器
    (*g_sl_mixobject)->Realize(g_sl_mixobject, SL_BOOLEAN_FALSE);
    SLresult result = (*g_sl_mixobject)->GetInterface(g_sl_mixobject, SL_IID_ENVIRONMENTALREVERB, &g_sl_reverb);
    if (SL_RESULT_SUCCESS == result) {
        (*g_sl_reverb)->SetEnvironmentalReverbProperties(g_sl_reverb, &g_sl_reverb_settings);
    }
}

void android_opensles_stop() {
    if (l_sl_play) {
        (*l_sl_play)->SetPlayState(l_sl_play, SL_PLAYSTATE_STOPPED);
        (*l_sl_player)->Destroy(l_sl_player);
        l_sl_play   = 0;
        l_sl_player = 0;
        l_sl_seek   = 0;
        l_sl_solo   = 0;
        l_sl_volume = 0;
    }
}

void android_opensles_play_buffer(int channel, int sample) {
    SLDataLocator_AndroidSimpleBufferQueue sl_queue = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM sl_pcm = {
            SL_DATAFORMAT_PCM,
            channel,                      // numChannels
            sample * 1000,                // samplesPerSec
            SL_PCMSAMPLEFORMAT_FIXED_8,   // bitsPerSample
            SL_PCMSAMPLEFORMAT_FIXED_8,   // containerSize
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, // channelMask
            SL_BYTEORDER_LITTLEENDIAN     // endianness
    };
    SLDataSource            sl_source = {&sl_queue, &sl_pcm};
    SLDataLocator_OutputMix sl_mix    = {SL_DATALOCATOR_OUTPUTMIX, g_sl_mixobject};
    SLDataSink              sl_sink   = {&sl_mix, 0};

    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean     req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    (*g_sl_engine)->CreateAudioPlayer(g_sl_engine, &l_sl_player,  &sl_source, &sl_sink, 3, ids, req);
    (*l_sl_player)->Realize(        l_sl_player, SL_BOOLEAN_FALSE);
    (*l_sl_player)->GetInterface(   l_sl_player, SL_IID_PLAY,                 &l_sl_play);
    (*l_sl_player)->GetInterface(   l_sl_player, SL_IID_BUFFERQUEUE,          &l_sl_queue);
    (*l_sl_player)->GetInterface(   l_sl_player, SL_IID_EFFECTSEND,           &l_sl_effect);
    (*l_sl_player)->GetInterface(   l_sl_player, SL_IID_VOLUME,               &l_sl_volume);

    (*l_sl_queue)->RegisterCallback(l_sl_queue,  android_opensles_callback,   0);
    (*l_sl_play)->SetPlayState(     l_sl_play,   SL_PLAYSTATE_PLAYING);
    android_opensles_callback(l_sl_queue, 0);
}

void android_opensles_callback(SLAndroidSimpleBufferQueueItf queue, void *data) {
    while (av_read_frame(pFormatCtx, pPacket) >= 0) {
        if (pPacket->stream_index == audio_index) {
            int ret = avcodec_decode_audio4(pCodecCtx, pFrame, &got_picture, pPacket);
            if (ret < 0) {
                break;
            }
            if (got_picture) {
                int size = av_samples_get_buffer_size(pFrame->linesize, 2, 44100, out_sample_fmt, 1);
                char *pcm = (char*) malloc(sizeof(char) * size);
                swr_convert(pSwrCtx, (uint8_t**) &pcm, out_sample_rate, (const uint8_t**) pFrame->extended_data, pFrame->nb_samples);
                (*queue)->Enqueue(queue, pcm, size);
                //free(pcm);
                return;
            }
        }
        av_free_packet(pPacket);
    }
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);
    swr_free(&pSwrCtx);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1ffmpeg_1audio_1player
  (JNIEnv *env, jclass, jstring jfilename) {
    char *filename = (char*)env->GetStringUTFChars(jfilename, 0);
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, filename, 0, 0) != 0) {
        // avformat_open_input error
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, 0) < 0) {
        // avformat_find_stream_info error
        return;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_index = i;
            break;
        }
    }
    if (audio_index == -1) {
        // not found audio stream
        return;
    }
    pCodecCtx = pFormatCtx->streams[audio_index]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (!pCodec) {
        // not found decoder
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        // avcodec_open2 error
        return;
    }
    pFrame = av_frame_alloc();
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(pPacket);
    pSwrCtx = swr_alloc();
    av_opt_set_int(pSwrCtx, "in_sample_fmt",      pCodecCtx->sample_fmt,     0);
    av_opt_set_int(pSwrCtx, "in_sample_rate",     pCodecCtx->sample_rate,    0);
    av_opt_set_int(pSwrCtx, "in_channel_layout",  pCodecCtx->channel_layout, 0);
    av_opt_set_int(pSwrCtx, "out_sample_fmt",     out_sample_fmt,            0);
    av_opt_set_int(pSwrCtx, "out_sample_rate",    out_sample_rate,           0);
    av_opt_set_int(pSwrCtx, "out_channel_layout", out_channel_layout,        0);
    swr_init(pSwrCtx);
    android_opensles_play_buffer(out_channel_layout, out_sample_rate);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1video
  (JNIEnv *env, jclass, jstring jyuvfile, jstring jvideofile, jint width, jint height) {
    char *yuvfile = (char*)env->GetStringUTFChars(jyuvfile, 0);
    char *videofile = (char*)env->GetStringUTFChars(jvideofile, 0);

    FILE *file_yuv = fopen(yuvfile, "rb+");
    char *yuv_buff = (char*)malloc(width * height * 3 / 2);

    AVFormatContext *pFormatCtx = 0;
    AVCodecContext  *pCodecCtx  = 0;
    AVOutputFormat  *pOutCtx    = 0;
    AVCodec         *pCodec     = 0;
    AVStream        *pStream    = 0;
    AVFrame         *pFrame     = 0;
    av_register_all();
    avcodec_register_all();
    av_log_set_callback(ff_log_callback);

    pFormatCtx = avformat_alloc_context();
    pOutCtx = av_guess_format("mp4", 0, 0);
    pFormatCtx->oformat = pOutCtx;
    if (avio_open(&pFormatCtx->pb, videofile, AVIO_FLAG_READ_WRITE) < 0) {
        return;
    }
    if (pOutCtx->video_codec != AV_CODEC_ID_NONE) {
        pStream = avformat_new_stream(pFormatCtx, 0);
        pCodecCtx = pStream->codec;
        pCodecCtx->codec_id = pOutCtx->video_codec;
        pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;

        pCodecCtx->bit_rate = 4000;
        pCodecCtx->width  = width;
        pCodecCtx->height = height;
        pCodecCtx->time_base.num = 1;
        pCodecCtx->time_base.den = 25;
        pCodecCtx->gop_size = 12;
        pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
        if (pOutCtx->video_codec == AV_CODEC_ID_H264) {
            pCodecCtx->me_range = 16;
            pCodecCtx->max_qdiff = 4;
            pCodecCtx->qmin = 10;
            pCodecCtx->qmax = 51;
            pCodecCtx->qcompress = 0.6;
        }
        if (pOutCtx->video_codec == AV_CODEC_ID_MPEG2VIDEO) {
            pCodecCtx->max_b_frames = 2;
        }
        if (pOutCtx->video_codec == AV_CODEC_ID_MPEG1VIDEO) {
            pCodecCtx->mb_decision = 2;
        }
        if (!strcmp(pOutCtx->name, "mp4") || !strcmp(pOutCtx->name, "mov") || !strcmp(pOutCtx->name, "3gp")) {
            pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
    }
    if (!pStream) {
        return;
    }
    if (!pCodecCtx) {
        return;
    }
    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        return;
    }
    if (pCodecCtx->codec_id == AV_CODEC_ID_H264) {
        av_opt_set(pCodecCtx->priv_data, "preset", "slow", 0);
    }
    pFrame = av_frame_alloc();
    int size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1);
    av_image_fill_arrays(pFrame->data, pFrame->linesize, (const uint8_t*)av_malloc(sizeof(char) * size), AV_PIX_FMT_YUV420P, width, height, 1);
    avformat_write_header(pFormatCtx, 0);
    int frame_cnt = 0;
    while (!feof(file_yuv)) {
        fread(pFrame->data[0], 1, width * height, file_yuv);
        fread(pFrame->data[1], 1, (width * height) / 4, file_yuv);
        fread(pFrame->data[2], 1, (width * height) / 4, file_yuv);
        pFrame->width = width;
        pFrame->height = height;
        pFrame->format = AV_PIX_FMT_YUV420P;
        pFrame->pts = frame_cnt++;

        if (pFormatCtx->oformat->flags & AVFMT_RAWPICTURE) {
            AVPacket packet;
            av_init_packet(&packet);
            packet.flags |= AV_PKT_FLAG_KEY;
            packet.stream_index = pStream->index;
            packet.data = (uint8_t*)pFrame->data;
            packet.size = width * height * 3 / 2;
            av_write_frame(pFormatCtx, &packet);
            __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "write frame(raw)");
        }
        else {
            AVPacket packet;
            int got_picture;
            av_init_packet(&packet);
            int ret = avcodec_encode_video2(pCodecCtx, &packet, pFrame, &got_picture);
            if (ret < 0) {
                break;
            }
            if (got_picture) {
                packet.pts = av_rescale_q(pCodecCtx->coded_frame->pts, pCodecCtx->time_base, pStream->time_base);
                packet.stream_index = pStream->index;
                if (pCodecCtx->coded_frame->key_frame) {
                    packet.flags |= AV_PKT_FLAG_KEY;
                }
                av_write_frame(pFormatCtx, &packet);
                __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "write frame(encoder)");
            }
        }
    }
    av_write_trailer(pFormatCtx);
    avcodec_close(pCodecCtx);
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

    fclose(file_yuv);
    free(yuv_buff);
    env->ReleaseStringUTFChars(jyuvfile, yuvfile);
    env->ReleaseStringUTFChars(jvideofile, videofile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1picture
  (JNIEnv *env, jclass, jstring jyuvfile, jstring jpicfile, jint width, jint height) {
    char *yuvfile = (char*)env->GetStringUTFChars(jyuvfile, 0);
    char *picfile = (char*)env->GetStringUTFChars(jpicfile, 0);

    FILE *file_yuv  = fopen(yuvfile, "rb+");
    char *file_buff = (char*)malloc(width * height * 3 / 2);
    fread(file_buff, 1, width * height * 3 / 2, file_yuv);

    AVFormatContext *pFormatCtx  = 0;
    AVOutputFormat  *pOutputCtx  = 0;
    AVCodecContext  *pCodecCtx   = 0;
    AVStream        *pStream     = 0;
    AVCodec         *pCodec      = 0;
    AVPacket        *pPacket     = 0;
    AVFrame         *pFrame      = 0;
    int              got_picture = 0;

    av_log_set_callback(ff_log_callback);
    av_register_all();
    avcodec_register_all();
    pOutputCtx = av_guess_format("mjpeg", 0, 0);
    pFormatCtx = avformat_alloc_context();
    pFormatCtx->oformat = pOutputCtx;
    if (avio_open(&pFormatCtx->pb, picfile, AVIO_FLAG_READ_WRITE) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-info", "%s", "avio_open error");
        return;
    }
    pStream = avformat_new_stream(pFormatCtx, 0);
    if (!pStream) {
        __android_log_print(ANDROID_LOG_INFO, "zd-info", "%s", "avformat_new_stream error");
        return;
    }
    pCodecCtx = pStream->codec;
    pCodecCtx->codec_id = pOutputCtx->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->width = width;
    pCodecCtx->height = height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        __android_log_print(ANDROID_LOG_INFO, "zd-info", "%s", "avcodec_find_encoder error");
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-info", "%s", "avcodec_open2 error");
        return;
    }
    avformat_write_header(pFormatCtx, 0);
    pFrame = av_frame_alloc();
    int size = av_image_get_buffer_size(AV_PIX_FMT_YUVJ420P, width, height, 1);
    av_image_fill_arrays(
            pFrame->data, pFrame->linesize,
            (const uint8_t*)av_malloc(size),
            AV_PIX_FMT_YUVJ420P, width, height, 1);
    pFrame->width = width;
    pFrame->height = height;
    pFrame->format = AV_PIX_FMT_YUVJ420P;
    pFrame->pts = 1;
    pFrame->data[0] = (uint8_t*)file_buff;
    pFrame->data[1] = (uint8_t*)file_buff + (width * height);
    pFrame->data[2] = (uint8_t*)pFrame->data[1] + ((width * height) / 4);

    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_new_packet(pPacket, width * height * 3);
    int ret = avcodec_encode_video2(pCodecCtx, pPacket, pFrame, &got_picture);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-info", "%s", "avcodec_encode_video2 error");
        return;
    }
    if (got_picture) {
        pPacket->stream_index = pStream->index;
        av_write_frame(pFormatCtx, pPacket);
    }
    av_write_trailer(pFormatCtx);

    avcodec_close(pCodecCtx);
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);
    av_packet_unref(pPacket);
    free(file_buff);
    fclose(file_yuv);

    env->ReleaseStringUTFChars(jyuvfile, yuvfile);
    env->ReleaseStringUTFChars(jpicfile, picfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1pcm_1to_1aac
  (JNIEnv *env, jclass, jstring jpcmfile, jstring jaacfile, jint sample_rate, jint channel) {
    char *pcmfile = (char*)env->GetStringUTFChars(jpcmfile, 0);
    char *aacfile = (char*)env->GetStringUTFChars(jaacfile, 0);

    FILE *file_pcm  = 0;
    char *file_buff = 0;

    AVFormatContext *pFormatCtx  = 0;
    AVOutputFormat  *pOutputCtx  = 0;
    AVCodecContext  *pCodecCtx   = 0;
    AVCodec         *pCodec      = 0;
    AVStream        *pStream     = 0;
    AVPacket        *pPacket     = 0;
    AVFrame         *pFrame      = 0;
    int              frame_cnt   = 0;
    int              got_picture = 0;

    av_log_set_callback(ff_log_callback);
    av_register_all();
    avcodec_register_all();
    pOutputCtx = av_guess_format("aac", aacfile, 0);
    pFormatCtx = avformat_alloc_context();
    pFormatCtx->oformat = pOutputCtx;
    if (avio_open(&pFormatCtx->pb, aacfile, AVIO_FLAG_READ_WRITE) != 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avio_open error");
        return;
    }
    pStream = avformat_new_stream(pFormatCtx, 0);
    pCodecCtx = pStream->codec;
    pCodecCtx->codec_id   = pOutputCtx->audio_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    pCodecCtx->sample_rate = sample_rate;
    pCodecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    pCodecCtx->channel_layout = AV_CH_LAYOUT_MONO;
    pCodecCtx->channels = channel;
    pCodecCtx->bit_rate = 64000;
    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avcodec_find_encoder error");
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) != 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avcodec_open2 error");
        return;
    }
    // 创建 AVFrame
    pFrame = av_frame_alloc();
    int size = av_samples_get_buffer_size(pFrame->linesize, channel, sample_rate, AV_SAMPLE_FMT_FLTP, 1);
    av_samples_fill_arrays(
            pFrame->data, pFrame->linesize,
            (const uint8_t*)av_malloc(size),
            channel, sample_rate, AV_SAMPLE_FMT_FLTP, 1);
    // 创建 AVPacket
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_new_packet(pPacket, size);
    // 打开文件
    file_pcm = fopen(pcmfile, "rb+");
    file_buff = (char*)malloc(size);
    // 写入文件头
    avformat_write_header(pFormatCtx, 0);
    while (!feof(file_pcm)) {
        fread(file_buff, 1, size, file_pcm);
        pFrame->data[0] = (uint8_t*)file_buff;
        pFrame->pts = frame_cnt++;

        int ret = avcodec_encode_audio2(pCodecCtx, pPacket, pFrame, &got_picture);
        if (ret < 0) {
            break;
        }
        if (got_picture) {
            pPacket->stream_index = pStream->index;
            av_write_frame(pFormatCtx, pPacket);
        }
        av_free_packet(pPacket);
    }
    /*
    while (1) {
        pPacket->data = 0;
        pPacket->size = 0;
        av_init_packet(pPacket);
        int ret = avcodec_encode_audio2(pCodecCtx, pPacket, pFrame, &got_picture);
        if (ret < 0) {
            break;
        }
        if (got_picture) {
            pPacket->stream_index = pStream->index;
            av_write_frame(pFormatCtx, pPacket);
        }
        av_free_packet(pPacket);
    }*/
    av_write_trailer(pFormatCtx);
    avcodec_close(pCodecCtx);
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);
    fclose(file_pcm);
    free(file_buff);

    env->ReleaseStringUTFChars(jpcmfile, pcmfile);
    env->ReleaseStringUTFChars(jaacfile, aacfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1h264
  (JNIEnv *env, jclass, jstring jyuvfile, jstring jh264file, jint width, jint height) {
    char *yuvfile = (char*)env->GetStringUTFChars(jyuvfile, 0);
    char *h264file = (char*)env->GetStringUTFChars(jh264file, 0);

    FILE *file_yuv  = 0;
    char *file_buff = 0;
    AVFormatContext *pFormatCtx  = 0;
    AVOutputFormat  *pOutputCtx  = 0;
    AVCodecContext  *pCodecCtx   = 0;
    AVStream        *pStream     = 0;
    AVCodec         *pCodec      = 0;
    AVPacket        *pPacket     = 0;
    AVFrame         *pFrame      = 0;
    int              got_picture = 0;
    int              frame_cnt   = 0;
    av_log_set_callback(ff_log_callback);
    av_register_all();
    avcodec_register_all();
    pOutputCtx = av_guess_format("h264", 0, 0);
    pFormatCtx = avformat_alloc_context();
    pFormatCtx->oformat = pOutputCtx;
    if (avio_open(&pFormatCtx->pb, h264file, AVIO_FLAG_READ_WRITE) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avio_open error");
        return;
    }
    pStream = avformat_new_stream(pFormatCtx, 0);
    pCodecCtx = pStream->codec;
    pCodecCtx->codec_id = pOutputCtx->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = width;
    pCodecCtx->height = height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->gop_size = 10;
    pCodecCtx->max_b_frames = 2;
    pCodecCtx->bit_rate = 400000;
    // params
    pCodecCtx->me_range = 16;
    pCodecCtx->max_qdiff = 4;
    pCodecCtx->qmin = 20;
    pCodecCtx->qmax = 51;
    pCodecCtx->qcompress = 0.6;
    if (pCodecCtx->codec_id == AV_CODEC_ID_H264) {
        av_opt_set(pCodecCtx->priv_data, "preset",      "slow",         0);
        av_opt_set(pCodecCtx->priv_data, "tune",        "zerolatency",  0);
        av_opt_set(pCodecCtx->priv_data, "profile",     "main",         0);
    }
    if (pCodecCtx->codec_id == AV_CODEC_ID_H265) {
        av_opt_set(pCodecCtx->priv_data, "x264-params", "qp=20",        0);
        av_opt_set(pCodecCtx->priv_data, "preset",      "ultrafast",    0);
        av_opt_set(pCodecCtx->priv_data, "tune",        "zero-latency", 0);
    }
    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avcodec_find_encoder error");
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avcodec_open2 error");
        return;
    }
    avformat_write_header(pFormatCtx, 0);
    // 创建 AVFrame
    pFrame = av_frame_alloc();
    int size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1);
    av_image_fill_arrays(
            pFrame->data, pFrame->linesize, (const uint8_t*)av_malloc(size),
            AV_PIX_FMT_YUV420P, width, height, 1);
    // 创建 AVPacket
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_new_packet(pPacket, size);
    // 准备数据
    file_yuv  = fopen(yuvfile, "rb+");
    file_buff = (char*)malloc(size);
    while (!feof(file_yuv)) {
        int ret = fread(file_buff, 1, (width * height * 3) / 2, file_yuv);
        if (ret < 0) {
            break;
        }
        pFrame->width   = width;
        pFrame->height  = height;
        pFrame->format  = AV_PIX_FMT_YUV420P;
        pFrame->pts     = frame_cnt++ * ((pStream->time_base.den) / (pStream->time_base.num * 25));
        pFrame->data[0] = (uint8_t*)file_buff;
        pFrame->data[1] = (uint8_t*)(pFrame->data[0] + (width * height));
        pFrame->data[2] = (uint8_t*)(pFrame->data[1] + (width * height / 4));
        ret = avcodec_encode_video2(pCodecCtx, pPacket, pFrame, &got_picture);
        if (ret < 0) {
            break;
        }
        if (got_picture) {
            pPacket->stream_index = pStream->index;
            av_write_frame(pFormatCtx, pPacket);
            av_free_packet(pPacket);
        }
    }
    av_write_trailer(pFormatCtx);
    //avcodec_close(pCodecCtx);
    //avio_close(pFormatCtx->pb);
    //avformat_free_context(pFormatCtx);

    fclose(file_yuv);
    free(file_buff);
    env->ReleaseStringUTFChars(jyuvfile, yuvfile);
    env->ReleaseStringUTFChars(jh264file, h264file);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1h265
  (JNIEnv *env, jclass, jstring jyuvfile, jstring jh265file, jint width, jint height) {
    char *yuvfile = (char*)env->GetStringUTFChars(jyuvfile, 0);
    char *h265file = (char*)env->GetStringUTFChars(jh265file, 0);

    FILE *file_yuv  = 0;
    char *file_buff = 0;

    AVFormatContext *pFormatCtx  = 0;
    AVOutputFormat  *pOutputCtx  = 0;
    AVCodecContext  *pCodecCtx   = 0;
    AVStream        *pStream     = 0;
    AVCodec         *pCodec      = 0;
    AVPacket        *pPacket     = 0;
    AVFrame         *pFrame      = 0;
    int              got_picture = 0;
    av_log_set_callback(ff_log_callback);
    av_register_all();
    avcodec_register_all();
    pOutputCtx = av_guess_format("libx265", 0, 0);
    pFormatCtx = avformat_alloc_context();
    pFormatCtx->oformat = pOutputCtx;
    if (avio_open(&pFormatCtx->pb, h265file, AVIO_FLAG_READ_WRITE) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avio_open error");
        return;
    }
    pStream = avformat_new_stream(pFormatCtx, 0);
    pCodecCtx = pStream->codec;
    pCodecCtx->codec_id      = pOutputCtx->video_codec;
    pCodecCtx->codec_type    = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt       = AV_PIX_FMT_YUV420P;
    pCodecCtx->width         = width;
    pCodecCtx->height        = height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->gop_size      = 120;
    pCodecCtx->max_b_frames  = 12;
    // params
    pCodecCtx->me_range  = 16;
    pCodecCtx->max_qdiff = 4;
    pCodecCtx->qmin      = 20;
    pCodecCtx->qmax      = 51;
    pCodecCtx->qcompress = 0.6;
    if (pCodecCtx->codec_id == AV_CODEC_ID_H264) {
        av_opt_set(pCodecCtx->priv_data, "preset",      "slow",         0);
        av_opt_set(pCodecCtx->priv_data, "tune",        "zerolatency",  0);
        av_opt_set(pCodecCtx->priv_data, "profile",     "main",         0);
    }
    if (pCodecCtx->codec_id == AV_CODEC_ID_H265) {
        av_opt_set(pCodecCtx->priv_data, "x264-params", "qp=20",        0);
        av_opt_set(pCodecCtx->priv_data, "preset",      "ultrafast",    0);
        av_opt_set(pCodecCtx->priv_data, "tune",        "zero-latency", 0);
    }
    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avcodec_find_encode error");
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avcodec_open2 error");
        return;
    }
    avformat_write_header(pFormatCtx, 0);
    // 创建 AVFrame
    pFrame = av_frame_alloc();
    int size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1);
    av_image_fill_arrays(
            pFrame->data, pFrame->linesize, (const uint8_t*)av_malloc(size),
            AV_PIX_FMT_YUV420P, width, height, 1);
    // 创建 AVPacket
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_new_packet(pPacket, size);
    // 准备数据
    file_yuv = fopen(yuvfile, "rb+");
    file_buff = (char*)malloc(size);
    while (!feof(file_yuv)) {
        int ret = fread(file_buff, 1, size, file_yuv);
        if (ret < 0) {
            break;
        }
        pFrame->width = width;
        pFrame->height = height;
        pFrame->format = AV_PIX_FMT_YUV420P;
        pFrame->data[0] = (uint8_t*)file_buff;
        pFrame->data[1] = (uint8_t*)(pFrame->data[0] + (width * height));
        pFrame->data[2] = (uint8_t*)(pFrame->data[1] + (width * height / 4));

        ret = avcodec_encode_video2(pCodecCtx, pPacket, pFrame, &got_picture);
        if (ret < 0) {
            break;
        }
        if (got_picture) {
            pPacket->stream_index = pStream->index;
            av_write_frame(pFormatCtx, pPacket);
            av_free_packet(pPacket);
        }
    }
    av_write_trailer(pFormatCtx);
    avcodec_close(pCodecCtx);
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

    fclose(file_yuv);
    free(file_buff);

    env->ReleaseStringUTFChars(jyuvfile, yuvfile);
    env->ReleaseStringUTFChars(jh265file, h265file);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1video_1audio_1remuxer
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jdstfile) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *dstfile = (char*)env->GetStringUTFChars(jdstfile, 0);

    AVFormatContext *pInputCtx  = 0;
    AVFormatContext *pOutputCtx = 0;
    AVOutputFormat  *pOutFmtCtx = 0;
    AVStream        *pInStream  = 0;
    AVStream        *pOutStream = 0;
    AVPacket        *pPacket    = 0;

    //av_log_set_callback(ff_log_callback);
    av_register_all();
    avcodec_register_all();
    pInputCtx = avformat_alloc_context();
    pOutputCtx = avformat_alloc_context();
    pOutFmtCtx = av_guess_format(0, dstfile, 0);
    pOutputCtx->oformat = pOutFmtCtx;
    if (avio_open(&pOutputCtx->pb, dstfile, AVIO_FLAG_READ_WRITE) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avio_open error");
        return;
    }
    if (avformat_open_input(&pInputCtx, srcfile, 0, 0) != 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avformat_open_input error");
        return;
    }
    if (avformat_find_stream_info(pInputCtx, 0) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avformat_find_stream_info error");
        return;
    }
    for (int i = 0; i < pInputCtx->nb_streams; i++) {
        pInStream = pInputCtx->streams[i];
        pOutStream = avformat_new_stream(pOutputCtx, pInStream->codec->codec);
        if (!pOutStream) {
            __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avformat_new_stream error");
            return;
        }
        int ret = avcodec_copy_context(pOutStream->codec, pInStream->codec);
        if (ret != 0) {
            __android_log_print(ANDROID_LOG_INFO, "zd-ff", "%s", "avcodec_copy_context error");
            return;
        }
        pOutStream->codec->codec_tag = 0;
        if (!(pOutputCtx->oformat->flags & AVFMT_GLOBALHEADER)) {
             pOutStream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
    }
    avformat_write_header(pOutputCtx, 0);
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(pPacket);
    while (1) {
        AVStream *inStream  = 0;
        AVStream *outStream = 0;
        int ret = av_read_frame(pInputCtx, pPacket);
        if (ret < 0) {
            break;
        }
        inStream  = pInputCtx->streams[pPacket->stream_index];
        outStream = pOutputCtx->streams[pPacket->stream_index];
        pPacket->dts = av_rescale_q_rnd(
                pPacket->dts,
                inStream->time_base, outStream->time_base,
                (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pPacket->pts = av_rescale_q_rnd(
                pPacket->pts,
                inStream->time_base, outStream->time_base,
                (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pPacket->duration = av_rescale_q_rnd(
                pPacket->duration,
                inStream->time_base, outStream->time_base,
                (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pPacket->pos = -1;
        av_interleaved_write_frame(pOutputCtx, pPacket);
        av_free_packet(pPacket);
    }
    av_write_trailer(pOutputCtx);
    avio_close(pOutputCtx->pb);
    avformat_free_context(pOutputCtx);
    avformat_close_input(&pInputCtx);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jdstfile, dstfile);
}

JNIEXPORT void JNICALL Java_com_ring0_ffmpeg_FFmpegHelper_simple_1yuv420p_1to_1filter
  (JNIEnv *env, jclass, jstring jsrcfile, jstring jfilterfile, jint frame) {
    char *srcfile = (char*)env->GetStringUTFChars(jsrcfile, 0);
    char *filterfile = (char*)env->GetStringUTFChars(jfilterfile, 0);

    AVFormatContext       *pFormatCtx    = 0;
    AVCodecContext        *pCodecCtx     = 0;
    AVCodec               *pCodec        = 0;
    AVPacket              *pPacket       = 0;
    AVFrame               *pFrameIn      = 0;
    AVFrame               *pFrameOut     = 0;

    AVFilterContext       *pFilterInCtx  = 0;
    AVFilterContext       *pFilterOutCtx = 0;
    AVFilter              *pBufferSrc    = 0;
    AVFilter              *pBufferSink   = 0;
    AVFilterGraph         *pFilterGraph  = 0;
    AVFilterInOut         *pInput        = 0;
    AVFilterInOut         *pOutput       = 0;
    AVBufferSinkParams    *pParams       = 0;

    AVPixelFormat pix_fmts[] = {AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE};
    int video_index = -1;
    int got_picture = 0;
    int frame_cnt   = 0;

    av_log_set_callback(ff_log_callback);
    av_register_all();
    avcodec_register_all();
    avfilter_register_all();
    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, srcfile, 0, 0) != 0) {
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, 0) < 0) {
        return;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            break;
        }
    }
    if (video_index == -1) {
        return;
    }
    pCodecCtx = pFormatCtx->streams[video_index]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (!pCodec) {
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, 0) < 0) {
        return;
    }
    // 初始化 AVFrame
    pFrameIn = av_frame_alloc();
    pFrameOut = av_frame_alloc();
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    // 初始化 AVFilter
    pFilterInCtx  = (AVFilterContext*)av_malloc(sizeof(AVFilterContext));
    pFilterOutCtx = (AVFilterContext*)av_malloc(sizeof(AVFilterContext));
    pBufferSrc    = avfilter_get_by_name("buffer");
    pBufferSink   = avfilter_get_by_name("buffersink");
    pFilterGraph  = avfilter_graph_alloc();
    pInput        = avfilter_inout_alloc();
    pOutput       = avfilter_inout_alloc();
    pParams       = av_buffersink_params_alloc();
    pParams->pixel_fmts = pix_fmts;

    char *inArgs  = (char*)malloc(sizeof(char) * 4096);
    char *outArgs = (char*)malloc(sizeof(char) * 4096);
    sprintf(inArgs, "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
            pCodecCtx->width, pCodecCtx->height,
            pCodecCtx->pix_fmt,
            pCodecCtx->time_base.num, pCodecCtx->time_base.den,
            pCodecCtx->sample_aspect_ratio.num, pCodecCtx->sample_aspect_ratio.den);
    sprintf(outArgs, "movie=%s[wm];[in][wm]overlay=5:5[out]", filterfile);
    if (avfilter_graph_create_filter(&pFilterInCtx, pBufferSrc, "in", inArgs, 0, pFilterGraph) != 0) {
        return;
    }
    if (avfilter_graph_create_filter(&pFilterOutCtx, pBufferSink, "out", 0, pParams, pFilterGraph) != 0) {
        return;
    }
    pOutput->name       = av_strdup("in");
    pOutput->filter_ctx = pFilterInCtx;
    pOutput->pad_idx    = 0;
    pOutput->next       = 0;

    pInput->name       = av_strdup("out");
    pInput->filter_ctx = pFilterOutCtx;
    pInput->pad_idx    = 0;
    pInput->next       = 0;

    if (avfilter_graph_parse_ptr(pFilterGraph, outArgs, &pInput, &pOutput, 0) < 0) {
        return;
    }
    if (avfilter_graph_config(pFilterGraph, 0) < 0) {
        return;
    }
    while (av_read_frame(pFormatCtx, pPacket) >= 0 && frame_cnt < frame) {
        if (pPacket->stream_index == video_index) {
            int ret = avcodec_decode_video2(pCodecCtx, pFrameIn, &got_picture, pPacket);
            if (ret > 0 & got_picture) {
                pFrameIn->pts = av_frame_get_best_effort_timestamp(pFrameIn);
                if (av_buffersrc_add_frame(pFilterInCtx, pFrameIn) < 0) {
                    break;
                }
                while (1) {
                    int ret = av_buffersink_get_frame(pFilterOutCtx, pFrameOut);
                    if (ret < 0) {
                        break;
                    }
                    // 保存 yuv420p
                    char *filename = (char*)malloc(sizeof(char) * 1024);
                    sprintf(filename, "/mnt/sdcard/test/%d_%dx%d.yuv", frame_cnt++, pCodecCtx->width, pCodecCtx->height);
                    FILE *f = fopen(filename, "wb+");
                    int width = pCodecCtx->width;
                    int height = pCodecCtx->height;
                    for (int i = 0; i < height; i++) {
                        fwrite(pFrameOut->data[0] + (i * pFrameOut->linesize[0]), 1, pFrameOut->linesize[0], f);
                    }
                    for (int i = 0; i < height / 2; i++) {
                        fwrite(pFrameOut->data[1] + (i * pFrameOut->linesize[1]), 1, pFrameOut->linesize[1], f);
                    }
                    for (int i = 0; i < height / 2; i++) {
                        fwrite(pFrameOut->data[2] + (i * pFrameOut->linesize[2]), 1, pFrameOut->linesize[2], f);
                    }
                    fclose(f);
                    free(filename);
                    av_frame_unref(pFrameOut);
                }
                av_frame_unref(pFrameIn);
            }
            av_free_packet(pPacket);
        }
    }
    avfilter_graph_free(&pFilterGraph);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    env->ReleaseStringUTFChars(jsrcfile, srcfile);
    env->ReleaseStringUTFChars(jfilterfile, filterfile);
}
