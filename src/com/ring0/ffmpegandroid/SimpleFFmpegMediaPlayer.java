package com.ring0.ffmpegandroid;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.ring0.ffmpeg.FFmpegHelper;
import com.ring0.ffmpeg.FFmpegHelper.FFmpegPlayerInterface;

public class SimpleFFmpegMediaPlayer extends Activity implements OnClickListener {
    
    private LinearLayout llDstFile;
    private TextView tvDstfile;
    private EditText etDstfile;
    private CheckBox cbHard;
    private Button btnStart;
    private GLSurfaceView glSurface;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.simple_ffmpeg_mediaplayer);
        
        initView();
        initData();
    }
    
    private void initView() {
        llDstFile = (LinearLayout)findViewById(R.id.llDstfile);
        tvDstfile = (TextView)llDstFile.findViewById(R.id.tvDstfile);
        tvDstfile.setText(getResources().getString(R.string.simple_ffmpeg_mediaplayer_filename));
        
        etDstfile = (EditText)llDstFile.findViewById(R.id.etDstfile);
        etDstfile.setHint(getResources().getString(R.string.simple_ffmpeg_mediaplayer_filename_hint));
        
        cbHard = (CheckBox)findViewById(R.id.cbHard);
        cbHard.setText(getResources().getString(R.string.simple_ffmpeg_mediaplayer_hard));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_ffmpeg_mediaplayer_start));
        btnStart.setOnClickListener(this);
        
        GLRenderer glRenderer = new GLRenderer();
        glSurface = (GLSurfaceView)findViewById(R.id.glSurface);
        glSurface.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        glSurface.setEGLContextFactory(new GLContextFactory());
        glSurface.setEGLConfigChooser(new GLConfigChooser(8, 8, 8, 8, 16, 0));
        glSurface.setRenderer(glRenderer);
        glSurface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String filename = etDstfile.getText().toString();
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_mediaplayer_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        new MyThread(filename, cbHard.isChecked()).start();
    }
    
    private class MyThread extends Thread {
        String filename = null;
        boolean hard = false;
        
        public MyThread(String filename, boolean hard) {
            this.filename = filename;
            this.hard = hard;
            
            FFmpegHelper.simple_ffmpeg_mediaplayer_init();
        }
        
        @Override
        public void run() {
            super.run();
            FFmpegHelper.simple_ffmpeg_mediaplayer_play(this.filename, hard, new FFmpegPlayerInterface() {
                @Override
                public void OnRenderer() {
                    glSurface.requestRender();
                }
            });
        }
    }
    
    private class GLRenderer implements GLSurfaceView.Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            FFmpegHelper.simple_ffmpeg_mediaplayer_renderer();
        }
    }

    private class GLContextFactory implements GLSurfaceView.EGLContextFactory {
        private int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

        @Override
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            int attribute[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            return egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attribute);
        }

        @Override
        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }

    private class GLConfigChooser implements GLSurfaceView.EGLConfigChooser {
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];
        private int EGL_OPENGL_ES2_BIT = 4;
        private int[] s_configAttribs2 = {
                EGL10.EGL_RED_SIZE, 4, 
                EGL10.EGL_GREEN_SIZE, 4,
                EGL10.EGL_BLUE_SIZE, 4,
                EGL10.EGL_RENDERABLE_TYPE,
                EGL_OPENGL_ES2_BIT,
                EGL10.EGL_NONE };
        
        public GLConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

            int numConfigs = num_config[0];
            if (numConfigs <= 0) {
                throw new IllegalArgumentException("");
            }
            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);
            return chooseConfig(egl, display, configs);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
            for (EGLConfig config : configs) {
                int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);
                if (d < mDepthSize || s < mStencilSize)
                    continue;
                int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

                if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                    return config;
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue) {
            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                return mValue[0];
            }
            return defaultValue;
        }
    }
}
