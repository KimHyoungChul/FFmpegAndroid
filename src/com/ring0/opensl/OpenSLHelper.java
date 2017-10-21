package com.ring0.opensl;

public class OpenSLHelper {
    static {
        System.loadLibrary("openslHelper");
    }
    
    public static void init() {
        
    }
    
    public static native void opensl_play(byte[] buffer, int size, int channel, int sample, int pcm_type);
}
