#include "com_ring0_ffmpeg_FFmpegHelper.h"
#include "simple_yuv.h"
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // y
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_y);
    for (int row = 0; row < height; ++row) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, row, width, 1, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                y_pixel + (row * width));
    }
    glUniform1i(textureUniformY, 0);
    // u
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_u);
    for (int row = 0; row < uvheight; ++row) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, row, uvwidth, 1, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                u_pixel + (row * uvwidth));
    }
    glUniform1i(textureUniformU, 1);
    // v
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_v);
    for (int row = 0; row < uvheight; ++row) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, row, uvwidth, 1, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                v_pixel + (row * uvwidth));
    }
    glUniform1i(textureUniformV, 2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
}

void FFmpeg_OnRenderer(JNIEnv *env, jobject jinterface, int width, int height, AVFrame *frame) {
    char *buffer = (char*) malloc(width * height * 3 / 2);
    char *frame_y = buffer;
    char *frame_u = buffer + (width * height);
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
  (JNIEnv *env, jclass, jstring jfilename, jobject jinterface) {
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
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
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
