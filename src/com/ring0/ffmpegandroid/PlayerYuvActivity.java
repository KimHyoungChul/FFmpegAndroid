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
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.ring0.ffmpeg.FFmpegHelper;
import com.ring0.opengl.OpenGLHelper;

/**
 * YUV 浏览器
 * @author ring0
 */
public class PlayerYuvActivity extends Activity implements OnClickListener, OnItemSelectedListener {
    
    private GLSurfaceView glSurface;    // opengl 视图
    private LinearLayout  llPathMain;   // 文件根布局
    private LinearLayout  llYuvMain;    // 类型根布局
    private LinearLayout  llWidthMain;  // 宽布局
    private LinearLayout  llHeightMain; // 高布局
    
    private TextView  tvFileName;
    private EditText  etFileName;
    private TextView  tvYuv;
    private Spinner   spYuvType;
    private TextView  tvWidth;
    private EditText  etWidth;
    private TextView  tvHeight;
    private EditText  etHeight;
    private Button    btPlay;
    // 临时
    private int       yuv_type[];
    private int       yuv_idx;
    // 显示设置
    private String     filename  = null;
    private int        width     = -1;
    private int        height    = -1;
    private int        yuv_value = -1;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.player_yuv);
        initView();
        initData();
    }
    
    private void initView() {
        glSurface    = (GLSurfaceView)findViewById(R.id.glSurface);
        llPathMain   = (LinearLayout)findViewById(R.id.llPathMain);
        llYuvMain    = (LinearLayout)findViewById(R.id.llYuvMain);
        llWidthMain  = (LinearLayout)findViewById(R.id.llWidthMain);
        llHeightMain = (LinearLayout)findViewById(R.id.llHeightMain);
        
        tvFileName   = (TextView)llPathMain.findViewById(R.id.tvFileName);
        etFileName   = (EditText)llPathMain.findViewById(R.id.etFileName);
        tvYuv        = (TextView)llYuvMain.findViewById(R.id.tvYuv);
        spYuvType    = (Spinner)llYuvMain.findViewById(R.id.spYuvType);
        tvWidth      = (TextView)llWidthMain.findViewById(R.id.tvWidth);
        etWidth      = (EditText)llWidthMain.findViewById(R.id.etWidth);
        tvHeight     = (TextView)llHeightMain.findViewById(R.id.tvHeight);
        etHeight     = (EditText)llHeightMain.findViewById(R.id.etHeight);
        btPlay       = (Button)findViewById(R.id.btPlay);
        
        tvFileName.setText(getResources().getString(R.string.player_yuv_filename));
        etFileName.setHint(getResources().getString(R.string.player_yuv_filename_hint));
        tvYuv.setText(getResources().getString(R.string.player_yuv_type));
        tvWidth.setText(getResources().getString(R.string.player_yuv_width));
        etWidth.setHint(getResources().getString(R.string.player_yuv_width_hint));
        tvHeight.setText(getResources().getString(R.string.player_yuv_height));
        etHeight.setHint(getResources().getString(R.string.player_yuv_height_hint));
        btPlay.setText(getResources().getString(R.string.player_yuv_play));
        btPlay.setOnClickListener(this);
        
        // 初始化 spinner
        String yuv_list[] = getResources().getStringArray(R.array.yuv_list);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, yuv_list);
        spYuvType.setAdapter(adapter);
        spYuvType.setOnItemSelectedListener(this);
        // 初始化 GLSurfaceView
        GLRenderer glRenderer = new GLRenderer();
        glSurface.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        glSurface.setEGLContextFactory(new GLContextFactory());
        glSurface.setEGLConfigChooser(new GLConfigChooser(8, 8, 8, 8, 16, 0));
        glSurface.setRenderer(glRenderer);
        glSurface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }
    
    private void initData() {
        yuv_type = new int[]{
                FFmpegHelper.FFMPEG_PIX_FMT_NV12,
                FFmpegHelper.FFMPEG_PIX_FMT_NV16,
                FFmpegHelper.FFMPEG_PIX_FMT_NV21,
                FFmpegHelper.FFMPEG_PIX_FMT_YUV410P,
                FFmpegHelper.FFMPEG_PIX_FMT_YUV411P,
                FFmpegHelper.FFMPEG_PIX_FMT_YUV420P,
                FFmpegHelper.FFMPEG_PIX_FMT_YUV422P,
                FFmpegHelper.FFMPEG_PIX_FMT_YUV440P,
                FFmpegHelper.FFMPEG_PIX_FMT_YUV444P
        };
        yuv_idx = 0;
    }
    
    private class GLRenderer implements GLSurfaceView.Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            OpenGLHelper.opengl_init();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            if (filename != null && width != -1 && height != -1 && yuv_value != -1) {
                OpenGLHelper.opengl_change(width, height);
                OpenGLHelper.opengl_draw(filename, width, height, yuv_value);
            }
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
    
    @Override
    public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
        yuv_idx = position;
    }

    @Override
    public void onNothingSelected(AdapterView<?> arg0) {
    }

    @Override
    public void onClick(View arg0) {
        String tempFile   = etFileName.getText().toString();
        String tempWidth  = etWidth.getText().toString();
        String tempHeight = etHeight.getText().toString();
        if (tempFile == null || tempFile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.player_yuv_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        if (tempWidth == null || tempWidth.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.player_yuv_toast_width), Toast.LENGTH_SHORT).show();
            return;
        }
        if (tempHeight == null || tempHeight.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.player_yuv_toast_height), Toast.LENGTH_SHORT).show();
            return;
        }
        filename  = tempFile;
        width     = Integer.parseInt(tempWidth);
        height    = Integer.parseInt(tempHeight);
        yuv_value = yuv_type[yuv_idx];
        glSurface.requestRender();
    }
}
