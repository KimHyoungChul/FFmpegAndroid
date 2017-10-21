package com.ring0.ffmpegandroid;

import android.app.Activity;
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

public class DecoderYuvActivity extends Activity implements OnClickListener, OnItemSelectedListener {
    private LinearLayout llFileNameMain; // 文件名根布局
    private LinearLayout llFilePathMain; // 文件路径根布局
    private LinearLayout llConfigMain;   // 解码配置的根布局
    
    private TextView tvFileName;
    private EditText etFileName;
    private TextView tvFilePath;
    private EditText etFilePath;
    private EditText etFrame;
    private Spinner  spYuvType;
    private Button   btDecode;
    
    private int[] yuv_type;
    private int   yuv_idx;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.decoder_yuv);
        
        initView();
        initData();
    }
    
    private void initView() {
        llFileNameMain = (LinearLayout)findViewById(R.id.llFileNameMain);
        llFilePathMain = (LinearLayout)findViewById(R.id.llFilePathMain);
        llConfigMain   = (LinearLayout)findViewById(R.id.llConfigMain);
        
        tvFileName = (TextView)llFileNameMain.findViewById(R.id.tvFileName);
        etFileName = (EditText)llFileNameMain.findViewById(R.id.etFileName);
        
        tvFilePath = (TextView)llFilePathMain.findViewById(R.id.tvFilePath);
        etFilePath = (EditText)llFilePathMain.findViewById(R.id.etFilePath);
        
        etFrame = (EditText)llConfigMain.findViewById(R.id.etFrame);
        spYuvType = (Spinner)llConfigMain.findViewById(R.id.spYuvType);
        
        btDecode = (Button)findViewById(R.id.btDecode);
        
        tvFileName.setText(getResources().getString(R.string.decoder_yuv_filename));
        tvFilePath.setText(getResources().getString(R.string.decoder_yuv_filepath));
        etFileName.setHint(getResources().getString(R.string.decoder_yuv_filename_hint));
        etFilePath.setHint(getResources().getString(R.string.decoder_yuv_filepath_hint));
        etFrame.setHint(getResources().getString(R.string.decoder_yuv_frame));
        btDecode.setText(getResources().getString(R.string.decoder_yuv_decode));
        
        String yuv_list[] = getResources().getStringArray(R.array.yuv_list);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, yuv_list);
        spYuvType.setAdapter(adapter);
        spYuvType.setOnItemSelectedListener(this);
        
        btDecode.setOnClickListener(this);
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
    
    @Override
    public void onClick(View arg0) {
        String filename = etFileName.getText().toString();
        String filepath = etFilePath.getText().toString();
        String framestr = etFrame.getText().toString();
        
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.decoder_yuv_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filepath == null || filepath.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.decoder_yuv_toast_filepath), Toast.LENGTH_SHORT).show();
            return;
        }
        if (framestr == null || framestr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.decoder_yuv_toast_frame), Toast.LENGTH_SHORT).show();
            return;
        }
        int frame = Integer.parseInt(framestr);
        int yuv_value = yuv_type[yuv_idx];
        
        new MyThread(filename, filepath, frame, yuv_value).start();
    }

    @Override
    public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
        yuv_idx = position;
    }

    @Override
    public void onNothingSelected(AdapterView<?> arg0) {
    }
    
    /**
     * 解码线程
     */
    private class MyThread extends Thread {
        private String filename;
        private String filepath;
        private int    frame;
        private int    yuv;
        
        public MyThread(String filename, String filepath, int frame, int yuv) {
            this.filename = filename;
            this.filepath = filepath;
            this.frame    = frame;
            this.yuv      = yuv;
        }
        
        @Override
        public void run() {
            super.run();
            FFmpegHelper.decoderVideoToYuv(filename, filepath, frame, yuv);
        }
    }
}
