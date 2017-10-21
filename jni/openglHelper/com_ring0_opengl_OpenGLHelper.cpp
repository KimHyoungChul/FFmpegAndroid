#include "com_ring0_opengl_OpenGLHelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

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

JNIEXPORT void JNICALL Java_com_ring0_opengl_OpenGLHelper_opengl_1init
  (JNIEnv *, jclass) {

}

JNIEXPORT void JNICALL Java_com_ring0_opengl_OpenGLHelper_opengl_1change
  (JNIEnv *, jclass, jint width, jint height) {
    setupShader(width, height);
}

JNIEXPORT void JNICALL Java_com_ring0_opengl_OpenGLHelper_opengl_1draw
  (JNIEnv *env, jclass, jstring jfilename, jint width, jint height, jint yuv_type) {
    if (!jfilename) {
        return;
    }
    // 读取 yuv 数据
    char *filename = (char*)env->GetStringUTFChars(jfilename, 0);
    long  filesize = 0;
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    filesize = ftell(f);
    rewind(f);
    char* buffer = (char*)malloc(sizeof(char) * filesize);
    fread(buffer, 1, filesize, f);
    // 加载贴图
    setupTexture(buffer, width, height);

    // 释放内存
    env->ReleaseStringUTFChars(jfilename, filename);
    free(buffer);
}
