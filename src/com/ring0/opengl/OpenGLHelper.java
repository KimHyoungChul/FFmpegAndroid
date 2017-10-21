package com.ring0.opengl;

public class OpenGLHelper {
    static {
        System.loadLibrary("openglHelper");
    }
    
    // 初始化 opengl
    public static native void opengl_init();
    // 安装 shader
    public static native void opengl_change(int width, int height);
    // 绘制贴图数据
    public static native void opengl_draw(String filename, int weight, int height, int yuv_type);
}
