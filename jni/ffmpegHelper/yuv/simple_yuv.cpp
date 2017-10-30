#include "simple_yuv.h"

// 将 yuv420p 分离为 y u v 三分量
void simple_yuv420p_split(char* srcfile, char* path, int width, int height) {
    char *str_y = (char*)malloc(sizeof(char) * 1024);
    char *str_u = (char*)malloc(sizeof(char) * 1024);
    char *str_v = (char*)malloc(sizeof(char) * 1024);
    sprintf(str_y, "%s/simple_yuv420p_y.y", path);
    sprintf(str_u, "%s/simple_yuv420p_u.y", path);
    sprintf(str_v, "%s/simple_yuv420p_v.y", path);

    char *pixel = (char*)malloc(width * height * 3 / 2);

    FILE *file_yuv = fopen(srcfile, "rb+");
    FILE *file_y = fopen(str_y, "wb+");
    FILE *file_u = fopen(str_u, "wb+");
    FILE *file_v = fopen(str_v, "wb+");
    fread(pixel, 1, width * height * 3 / 2, file_yuv);
    fwrite(pixel, 1, width * height, file_y);
    fwrite(pixel + (width * height), 1, width * height / 4, file_u);
    fwrite(pixel + (width * height) + (width * height / 4), 1, width * height / 4, file_v);
    fclose(file_yuv);
    fclose(file_y);
    fclose(file_u);
    fclose(file_v);

    free(pixel);
    free(str_y);
    free(str_u);
    free(str_v);
}
// 将 yuv444p 分离为 y u v 三分量
void simple_yuv444p_split(char* srcfile, char* path, int width, int height) {
    char *str_y = (char*)malloc(sizeof(char) * 1024);
    char *str_u = (char*)malloc(sizeof(char) * 1024);
    char *str_v = (char*)malloc(sizeof(char) * 1024);
    sprintf(str_y, "%s/simple_yuv444p_y.y", path);
    sprintf(str_u, "%s/simple_yuv444p_u.y", path);
    sprintf(str_v, "%s/simple_yuv444p_v.y", path);

    char *pixel = (char*)malloc(width * height * 3);

    FILE *file_yuv = fopen(srcfile, "rb+");
    FILE *file_y = fopen(str_y, "wb+");
    FILE *file_u = fopen(str_u, "wb+");
    FILE *file_v = fopen(str_v, "wb+");
    fread(pixel, 1, width * height * 3, file_yuv);
    fwrite(pixel, 1, width * height, file_y);
    fwrite(pixel + (width * height), 1, width * height, file_u);
    fwrite(pixel + (width * height * 2), 1, width * height, file_v);

    fclose(file_yuv);
    fclose(file_y);
    fclose(file_u);
    fclose(file_v);

    free(pixel);
    free(str_y);
    free(str_u);
    free(str_v);
}
// 将 yuv420p 亮度减半
void simple_yuv420p_half(char* srcfile, char* dstfile, int width, int height) {
    FILE *file_yuv = fopen(srcfile, "rb+");
    FILE *file_half = fopen(dstfile, "wb+");

    char *pixel = (char*)malloc(width * height * 3 / 2);
    fread(pixel, 1, width * height * 3 / 2, file_yuv);
    for (int i = 0; i < width * height; i++) {
        unsigned char temp = pixel[i];
        temp = temp / 2;
        pixel[i] = temp;
    }
    fwrite(pixel, 1, width * height * 3 / 2, file_half);
    fclose(file_yuv);
    fclose(file_half);
    free(pixel);
}
// 将 yuv420p 去掉色度
void simple_yuv420p_gray(char* srcfile, char* dstfile, int width, int height) {
    FILE *file_yuv = fopen(srcfile, "rb+");
    FILE *file_gray = fopen(dstfile, "wb+");
    char *pixel = (char*)malloc(width * height * 3 / 2);
    fread(pixel, 1, width * height * 3 / 2, file_yuv);
    memset(pixel + (width * height), 128, width * height / 2);
    fwrite(pixel, 1, width * height * 3 / 2, file_gray);
    fclose(file_yuv);
    fclose(file_gray);
    free(pixel);
}
// 将 yuv420p 加上边框
void simple_yuv420p_board(char* srcfile, char* dstfile, int board, int width, int height) {
    FILE *file_yuv = fopen(srcfile, "rb+");
    FILE *file_board = fopen(dstfile, "wb+");

    char *pixel = (char*)malloc(width * height * 3 / 2);
    fread(pixel, 1, width * height * 3 / 2, file_yuv);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i < board || i > (height - board) || j < board || j > (width - board)) {
                pixel[i * width + j] = 255;
            }
        }
    }
    fwrite(pixel, 1, width * height * 3 / 2, file_board);
    fclose(file_yuv);
    fclose(file_board);
    free(pixel);
}
// 生成 yuv420p 灰阶图
void simple_yuv420p_graybar(char* dstfile, int barsize, int width, int height) {
    int uv_width  = width / 2;
    int uv_height = height / 2;
    int bar_width = width / barsize;
    int bar_color = 255 / barsize;

    FILE *file_graybar = fopen(dstfile, "wb+");
    char *pixel_y = (char*)malloc(width * height);
    char *pixel_u = (char*)malloc(uv_width * uv_height);
    char *pixel_v = (char*)malloc(uv_width * uv_height);
    // 写入 y 亮度分量
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int step = j / bar_width;
            pixel_y[i * width + j] = bar_color * step;
        }
    }
    // 写入 u 色度分量
    for (int i = 0; i < uv_height; i++) {
        for (int j = 0; j < uv_width; j++) {
            pixel_u[i * uv_width + j] = 128;
        }
    }
    // 写入 v 色度分量
    for (int i = 0; i < uv_height; i++) {
        for (int j = 0; j < uv_width; j++) {
            pixel_v[i * uv_width + j] = 128;
        }
    }
    fwrite(pixel_y, 1, width * height, file_graybar);
    fwrite(pixel_u, 1, uv_width * uv_height, file_graybar);
    fwrite(pixel_v, 1, uv_width * uv_height, file_graybar);
    fclose(file_graybar);
    free(pixel_y);
    free(pixel_u);
    free(pixel_v);
}
// 计算两个 yuv420p 的 psnr
void simple_yuv420p_psnr(char* srcfile, char* dstfile, int width, int height) {
    FILE *file_src = fopen(srcfile, "rb+");
    FILE *file_dst = fopen(dstfile, "rb+");

    char *pixel_src = (char*)malloc(width * height);
    char *pixel_dst = (char*)malloc(width * height);
    fread(pixel_src, 1, width * height, file_src);
    fread(pixel_dst, 1, width * height, file_dst);

    double mse_sum = 0;
    double mse     = 0;
    double psnr    = 0;
    for (int i = 0; i < width * height; i++) {
        mse_sum += pow((double)(pixel_src[i] - pixel_dst[i]), 2);
    }
    mse = mse_sum / (width * height);
    psnr = 10 * log10(255.0 * 255.0 / mse);
    // print psnr
    free(pixel_src);
    free(pixel_dst);
    fclose(file_src);
    fclose(file_dst);
}
// 将 rgb24 分离为 r g b 三分量
void simple_rgb24_split(char* srcfile, char* path, int width, int height) {
    char *str_r = (char*)malloc(sizeof(char) * 1024);
    char *str_g = (char*)malloc(sizeof(char) * 1024);
    char *str_b = (char*)malloc(sizeof(char) * 1024);
    sprintf(str_r, "%s/rgb24_r.rgb", path);
    sprintf(str_g, "%s/rgb24_g.rgb", path);
    sprintf(str_b, "%s/rgb24_b.rgb", path);

    FILE *file_rgb = fopen(srcfile, "rb+");
    FILE *file_r = fopen(str_r, "wb+");
    FILE *file_g = fopen(str_g, "wb+");
    FILE *file_b = fopen(str_b, "wb+");

    char *rgb = (char*)malloc(width * height * 3);
    fread(rgb, 1, width * height * 3, file_rgb);
    for (int i = 0; i < width * height * 3; i += 3) {
        fwrite(rgb + i + 0, 1, 1, file_r);
        fwrite(rgb + i + 1, 1, 1, file_g);
        fwrite(rgb + i + 2, 1, 1, file_b);
    }
    fclose(file_rgb);
    fclose(file_r);
    fclose(file_g);
    fclose(file_b);
    free(rgb);
    free(str_r);
    free(str_g);
    free(str_b);
}
// 将 rgb24 封装为 bmp 文件
void simple_rgb24_bmp(char* srcfile, char* dstfile, int width, int height) {
    typedef struct {
        long image_size;
        long blank;
        long start_position;
    } BitmapHeader;

    typedef struct {
        long length;
        long width;
        long height;
        unsigned short color_plate;
        unsigned short bit_color;
        long zip_format;
        long real_size;
        long x_pels;
        long y_pels;
        long color_use;
        long color_important;
    } BitmapInfo;

    // 打开获取 rgb 数据
    FILE *file_rgb = fopen(srcfile, "rb+");
    FILE *file_bmp = fopen(dstfile, "wb+");
    char *rgb = (char*)malloc(width * height * 3);
    fread(rgb, 1, width * height * 3, file_rgb);
    // 初始化 bmp 各个字段
    char bmp_sign[2] = {'B', 'M'};
    BitmapHeader header = {0};
    BitmapInfo info = {0};
    int header_size = sizeof(bmp_sign) + sizeof(BitmapHeader) + sizeof(BitmapInfo);

    header.image_size = header_size + (width * height * 3);
    header.start_position = header_size;

    info.length = sizeof(BitmapInfo);
    info.width = width;
    info.height = -height;
    info.color_plate = 1;
    info.bit_color = 24;
    info.real_size = width * height * 3;
    fwrite(bmp_sign, 1, 2, file_bmp);
    fwrite(&header, 1, sizeof(BitmapHeader), file_bmp);
    fwrite(&info, 1, sizeof(BitmapInfo), file_bmp);
    // 将 rgb 转换为 bgr
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char temp = rgb[(i * width + j) + 0];
            rgb[(i * width + j) + 0] = rgb[(i * width + j) + 2];
            rgb[(i * width + j) + 2] = temp;
        }
    }
    fwrite(rgb, 1, width * height * 3, file_bmp);
    fclose(file_rgb);
    fclose(file_bmp);
    free(rgb);
}

unsigned char fix(unsigned char value, unsigned char max, unsigned char min) {
    if (value > max) {
        return max;
    }
    else if (value < max) {
        return min;
    }
    else {
        return value;
    }
}
// rgb24 转 yuv420p
void simple_rgb24_yuv420p(char* srcfile, char* dstfile, int width, int height) {
    // 打开文件并且读取数据
    FILE *file_rgb = fopen(srcfile, "rb+");
    FILE *file_yuv = fopen(dstfile, "wb+");
    unsigned char *rgb = (unsigned char*)malloc(width * height * 3);
    unsigned char *yuv = (unsigned char*)malloc(width * height * 3 / 2);
    fread(rgb, 1, width * height * 3, file_rgb);
    memset(yuv, 0, width * height * 3 / 2);
    // 生成 yuv 数据
    unsigned char *ptrY, *ptrU, *ptrV, *ptrRGB;
    unsigned char r, g, b, y, u, v;
    ptrY = yuv;
    ptrU = yuv + (width * height);
    ptrV = ptrU + ((width * height) / 4);
    for (int i = 0; i < height; i++) {
        ptrRGB = rgb + (i * width * 3);
        for (int j = 0; j < width; j++) {
            r = *(ptrRGB++);
            g = *(ptrRGB++);
            b = *(ptrRGB++);
            y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
            u = (unsigned char)((-38 * r - 74 * g - 18 * b + 128) >> 8) + 128;
            v = (unsigned char)((112 * r - 97 * g - 18 * b + 128) >> 8) + 128;
            *(ptrY++) = fix(y, 0, 255);
            if ((i % 2 == 0) && (j & 2 == 0)) {
                *(ptrU++) = fix(u, 0, 255);
            }
            else if (j % 2 == 0) {
                *(ptrV++) = fix(v, 0, 255);
            }
        }
    }
    // 写入数据
    fwrite(yuv, 1, width * height * 3 / 2, file_yuv);
    fclose(file_rgb);
    fclose(file_yuv);
    free(rgb);
    free(yuv);
}
// 生成 rgb24 彩条图
void simple_rgb24_colorbar(char* dstfile, int barsize, int width, int height) {
    FILE *file_rgb = fopen(dstfile, "wb+");
    char *rgb = (char*)malloc(width * height * 3);

    int bar_width = width / barsize;
    int bar_color = 255 / barsize;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int step = j / bar_width;
            rgb[(i * width + j) * 3 + 0] = bar_color * step;
            rgb[(i * width + j) * 3 + 1] = bar_color * step;
            rgb[(i * width + j) * 3 + 2] = bar_color * step;
        }
    }
    fwrite(rgb, 1, width * height * 3, file_rgb);
    fclose(file_rgb);
    free(rgb);
}

void simple_pcm16le_split(char* srcfile, char* path) {
    char *pcm_l = (char*)malloc(sizeof(char) * 1024);
    char *pcm_r = (char*)malloc(sizeof(char) * 1024);
    sprintf(pcm_l, "%s/pcm16le_l.pcm", pcm_l);
    sprintf(pcm_r, "%s/pcm16le_r.pcm", pcm_r);
    char *sample = (char*)malloc(sizeof(char) * 4);

    FILE *file_pcm = fopen(srcfile, "rb+");
    FILE *file_l = fopen(pcm_l, "wb+");
    FILE *file_r = fopen(pcm_r, "wb+");
    while (!feof(file_pcm)) {
        fread(sample, 1, 4, file_pcm);
        fwrite(sample, 1, 2, file_l);
        fwrite(sample + 2, 1, 2, file_r);
    }
    fclose(file_pcm);
    fclose(file_l);
    fclose(file_r);
    free(sample);
}
void simple_pcm16le_lefthalf(char* srcfile, char* dstfile) {
    char *sample = (char*)malloc(sizeof(char) * 4);

    FILE *file_src = fopen(srcfile, "rb+");
    FILE *file_dst = fopen(dstfile, "wb+");
    while (!feof(file_src)) {
        fread(sample, 1, 4, file_src);
        unsigned short *left = (unsigned short*)sample;
        *left = *left / 2;
        fwrite(sample, 1, 4, file_dst);
    }
    fclose(file_src);
    fclose(file_dst);
    free(sample);
}
void simple_pcm16le_doublespeed(char* srcfile, char* dstfile) {
    char *sample = (char*)malloc(sizeof(char) * 4);
    FILE *file_src = fopen(srcfile, "rb+");
    FILE *file_dst = fopen(dstfile, "wb+");
    int cnt = 0;
    while (!feof(file_src)) {
        fread(sample, 1, 4, file_src);
        if (cnt % 2 == 0) {
            fwrite(sample, 1, 4, file_dst);
        }
        cnt++;
    }
    fclose(file_src);
    fclose(file_dst);
    free(sample);
}
void simple_pcm16le_pcm8le(char* srcfile, char* dstfile) {
    char *sample = (char*)malloc(sizeof(char) * 4);
    FILE *file_src = fopen(srcfile, "rb+");
    FILE *file_dst = fopen(dstfile, "wb+");
    while (!feof(file_src)) {
        fread(sample, 1, 4, file_src);
        short *temp_l = (short*)sample;
        short *temp_r = (short*)sample + 2;
        char sample_l = (*temp_l >> 8) + 128;
        char sample_r = (*temp_r >> 8) + 128;
        fwrite(&sample_l, 1, 1, file_dst);
        fwrite(&sample_r, 1, 1, file_dst);
    }
    fclose(file_src);
    fclose(file_dst);
    free(sample);
}
void simple_pcm16le_corp(char* srcfile, char* dstfile, int startpoint, int count) {
    char *sample = (char*)malloc(sizeof(char) * 4);
    FILE *file_src = fopen(srcfile, "rb+");
    FILE *file_dst = fopen(dstfile, "wb+");
    int cnt = 0;
    while (!feof(file_src)) {
        fread(sample, 1, 4, file_src);
        if (cnt > startpoint && cnt < startpoint + count) {
            fwrite(sample, 1, 4, file_dst);
        }
        cnt++;
    }
    fclose(file_src);
    fclose(file_dst);
    free(sample);
}
void simple_pcm16le_wave(char* srcfile, char* dstfile) {
    typedef struct wave_header {
        char fcc_id[4];
        unsigned long size;
        char fcc_type[4];
    };

    typedef struct wave_fmt {
        char fcc_id[4];
        unsigned long size;
        unsigned short format_tag;
        unsigned short channels;
        unsigned long sample_per_sec;
        unsigned long avg_bytes_per_sec;
        unsigned short block_align;
        unsigned short bits_per_sample;
    };

    typedef struct wave_data {
        char fcc_id[4];
        unsigned long size;
    };
    FILE *file_src = fopen(srcfile, "rb+");
    FILE *file_dst = fopen(dstfile, "wb+");

    int channels = 2;
    int sample = 44100;
    int bits = 16;

    wave_header header = {0};
    wave_fmt    fmt    = {0};
    wave_data   data   = {0};

    memcpy(header.fcc_id, "RIFF", 4);
    memcpy(header.fcc_type, "WAVE", 4);
    fseek(file_dst, sizeof(wave_header), 1);

    fmt.sample_per_sec = sample;
    fmt.avg_bytes_per_sec = fmt.sample_per_sec * sizeof(short);
    fmt.bits_per_sample = bits;
    memcpy(fmt.fcc_id, "fmt ", 4);
    fmt.size = 16;
    fmt.block_align = 2;
    fmt.channels = channels;
    fmt.format_tag = 1;
    fwrite(&fmt, 1, sizeof(wave_fmt), file_dst);

    memcpy(data.fcc_id, "data", 4);
    data.size = 0;
    fseek(file_dst, sizeof(wave_data), SEEK_CUR);

    while (!feof(file_src)) {
        unsigned short samples;
        fread(&samples, 1, sizeof(unsigned short), file_src);
        data.size += 2;
        fwrite(&samples, 1, sizeof(unsigned short), file_dst);
    }
    header.size += 44 + data.size;
    fwrite(&header, 1, sizeof(wave_header), file_dst);
    fseek(file_dst, sizeof(wave_fmt), SEEK_CUR);
    fwrite(&data, 1, sizeof(wave_data), file_dst);

    fclose(file_src);
    fclose(file_dst);
}