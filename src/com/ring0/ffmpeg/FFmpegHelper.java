package com.ring0.ffmpeg;

public class FFmpegHelper {
    static {
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
}
