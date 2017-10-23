#include "com_ring0_ffmpeg_FFmpegHelper.h"
#include "simple_yuv.h"

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
                        FILE *f = fopen(yuv_file, "wb+");
                        fwrite(pDst->data[0], 1, width * height, f);
                        fwrite(pDst->data[1], 1, (width * height) / 4, f);
                        fwrite(pDst->data[2], 1, (width * height) / 4, f);
                        fclose(f);
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
