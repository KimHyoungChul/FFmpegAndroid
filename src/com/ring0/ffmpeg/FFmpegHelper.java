package com.ring0.ffmpeg;

public class FFmpegHelper {
    static {
        System.loadLibrary("x264");
        System.loadLibrary("ffmpeg");
        System.loadLibrary("ffmpegHelper");
    }
    
    // yuv type
    public static final int FFMPEG_PIX_FMT_NV12    = 0;
    public static final int FFMPEG_PIX_FMT_NV16    = 1;
    public static final int FFMPEG_PIX_FMT_NV21    = 2;
    public static final int FFMPEG_PIX_FMT_YUV410P = 3;
    public static final int FFMPEG_PIX_FMT_YUV411P = 4;
    public static final int FFMPEG_PIX_FMT_YUV420P = 5;
    public static final int FFMPEG_PIX_FMT_YUV422P = 6;
    public static final int FFMPEG_PIX_FMT_YUV440P = 7;
    public static final int FFMPEG_PIX_FMT_YUV444P = 8;
    public static final int FFMPEG_PIX_FMT_RGB24   = 9;
    // pcm type
    public static final int FFMPEG_SAMPLE_FMT_U8   = 0;
    public static final int FFMPEG_SAMPLE_FMT_S16  = 1;
    public static final int FFMPEG_SAMPLE_FMT_S32  = 2;
    public static final int FFMPEG_SAMPLE_FMT_FLT  = 3;
    public static final int FFMPEG_SAMPLE_FMT_DBL  = 4;
    public static final int FFMPEG_SAMPLE_FMT_S64  = 5;
    public static final int FFMPEG_SAMPLE_FMT_U8P  = 6;
    public static final int FFMPEG_SAMPLE_FMT_S16P = 7;
    public static final int FFMPEG_SAMPLE_FMT_S32P = 8;
    public static final int FFMPEG_SAMPLE_FMT_FLTP = 9;
    public static final int FFMPEG_SAMPLE_FMT_DBLP = 10;
    public static final int FFMPEG_SAMPLE_FMT_S64P = 11;
    // decoder_type
    public static final int FFMPEG_DECODER_MPEG4   = 0;
    public static final int FFMPEG_DECODER_H264    = 1;
    public static final int FFMPEG_DECODER_H265    = 2;
    public static final int FFMPEG_DECODER_VP8     = 3;
    public static final int FFMPEG_DECODER_VP9     = 4;
    // encoder_type
    public static final int FFMPEG_ENCODER_MPEG4   = 0;
    public static final int FFMPEG_ENCODER_H264    = 1;
    public static final int FFMPEG_ENCODER_H265    = 2;
    public static final int FFMPEG_ENCODER_VP8     = 3;
    public static final int FFMPEG_ENCODER_VP9     = 4;
    // 解码视频到 YUV
    // path:     yuv 保存路径
    // frame:    解码帧数
    // yuv_type: yuv 格式
    public static native void decoderVideoToYuv(String filename, String path, int frame, int yuv_type);
    
    // 解码音频到 PCM
    // filename: 音频文件
    // path: pcm 保存路径
    // frame: 解码帧数
    // pcm_type: pcm 格式
    public static native void decoderAudioToPcm(String filename, String path, int frame, int channel, int sample_rate, int pcm_type);
    
    // yuv420p 分离为 y u v 三分量
    public static native void simple_yuv420p_split(String srcfile, String path, int width, int height);
    // yuv444p 分离为 y u v 三分量
    public static native void simple_yuv444p_split(String srcfile, String path, int width, int height);
    // yuv420p 亮度减半
    public static native void simple_yuv420p_half(String srcfile, String dstfile, int width, int height);
    // yuv420p 去掉颜色
    public static native void simple_yuv420p_gray(String srcfile, String dstfile, int width, int height);
    // yuv420p 加上边框
    public static native void simple_yuv420p_board(String srcfile, String dstfile, int board, int width, int height);
    // 生成 yuv420p 灰阶图
    public static native void simple_yuv420p_graybar(String dstfile, int barsize, int width, int height);
    // 计算两个 yuv420p 的 psnr
    public static native void simple_yuv420p_psnr(String srcfile, String dstfile, int width, int height);
    // 将 rgb24 分离为 r g b 三分量
    public static native void simple_rgb24_split(String srcfile, String path, int width, int height);
    // 将 rgb24 封装为 bmp 文件
    public static native void simple_rgb24_bmp(String srcfile, String dstfile, int width, int height);
    // rgb24 转 yuv420p
    public static native void simple_rgb24_yuv420p(String srcfile, String dstfile, int width, int height);
    // 生成 rgb24 彩条图
    public static native void simple_rgb24_colorbar(String dstfile, int barsize, int width, int height);
    
    // 将 pcm16le 分离为 左右声道
    public static native void simple_pcm16le_split(String srcfile, String path);
    // 将 pcm16le 左声道音量降低一半
    public static native void simple_pcm16le_lefthalf(String srcfile, String dstfile);
    // 将 pcm16le 声音速度提高一倍
    public static native void simple_pcm16le_doublespeed(String srcfile, String dstfile);
    // 将 pcm16le 转换为 pcm8le
    public static native void simple_pcm16le_pcm8le(String srcfile, String dstfile);
    // 将 pcm16le 截取部分采样
    public static native void simple_pcm16le_corp(String srcfile, String dstfile, int startpoint, int count);
    // 将 pcm16le 保存为 wav
    public static native void simple_pcm16le_wave(String srcfile, String dstfile);

    // 视频播放回调
    public interface FFmpegPlayerInterface {
        public void OnRenderer();
    }
    // 简单的视频播放器
    public static native void simple_ffmpeg_player(String filename, boolean hard, FFmpegPlayerInterface interf);
    public static native void simple_ffmpeg_player_init();
    public static native void simple_ffmpeg_player_renderer();
    // 精简的解码器
    public static native void simple_ffmpeg_decoder(String srcfile, String path, int decoder_type);
    // 精简的编码器
    public static native void simple_ffmpeg_encoder(String srcfile, String path, int width, int hegiht, int encoder_type);
    // 精简的音频播放器
    public static native void simple_ffmpeg_audio_player(String filename);
    // yuv420p 转视频
    public static native void simple_yuv420p_to_video(String yuvfile, String videofile, int width, int height);
    // yuv420p 保存为图片
    public static native void simple_yuv420p_to_picture(String yuvfile, String picfile, int width, int height);
    // pcm 转 aac
    public static native void simple_pcm_to_aac(String pcmfile, String aacfile, int sample_rate, int channel);
    // yuv420p 转 h264
    public static native void simple_yuv420p_to_h264(String yuvfile, String h264file, int width, int height);
    // yuv420p 转 h265
    public static native void simple_yuv420p_to_h265(String yuvfile, String h265file, int width, int height);
    // 无编解码的转换封装格式
    public static native void simple_video_audio_remuxer(String srcfile, String dstfile);
    // 精简的水印叠加器
    public static native void simple_yuv420p_to_filter(String srcfile, String filterfile, int frame);
    // 添加指定的 filter
    public static native void simple_yuv420p_spec_filter(String srcfile, String filterfile, String filter, int width, int height);
    // simple avdevice
    public static native void simple_ffmpeg_avdevice();
    // simple swscale
    public static native void simple_ffmpeg_swscale(String srcfile, String dstfile, int width, int height);
}
