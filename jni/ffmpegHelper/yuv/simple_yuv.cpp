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
    FILE *file_u = fopen(str_y, "wb+");
    FILE *file_v = fopen(str_y, "wb+");
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
    FILE *file_u = fopen(str_y, "wb+");
    FILE *file_v = fopen(str_y, "wb+");
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
