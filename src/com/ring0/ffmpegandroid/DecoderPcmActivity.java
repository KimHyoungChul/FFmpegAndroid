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

public class DecoderPcmActivity extends Activity implements OnClickListener, OnItemSelectedListener {
    
    private LinearLayout llFileNameMain; // 文件名根布局
    private LinearLayout llFilePathMain; // 文件路径根布局
    private LinearLayout llChannelMain;  // 通道数根布局
    private LinearLayout llSampleMain;   // 采样率根布局
    private LinearLayout llConfigMain;   // 解码配置的根布局
    
    private TextView tvFileName;
    private EditText etFileName;
    private TextView tvFilePath;
    private EditText etFilePath;
    private TextView tvChannel;
    private EditText etChannel;
    private TextView tvSample;
    private EditText etSample;
    private EditText etFrame;
    private Spinner  spYuvType;
    private Button   btDecode;
    
    private int[] pcm_type;
    private int   pcm_idx;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.decoder_pcm);
        
        initView();
        initData();
    }
    
    private void initView() {
        llFileNameMain = (LinearLayout)findViewById(R.id.llFileNameMain);
        llFilePathMain = (LinearLayout)findViewById(R.id.llFilePathMain);
        llChannelMain  = (LinearLayout)findViewById(R.id.llChannelMain);
        llSampleMain   = (LinearLayout)findViewById(R.id.llSampleMain);
        llConfigMain   = (LinearLayout)findViewById(R.id.llConfigMain);
        
        tvFileName = (TextView)llFileNameMain.findViewById(R.id.tvFileName);
        etFileName = (EditText)llFileNameMain.findViewById(R.id.etFileName);
        
        tvFilePath = (TextView)llFilePathMain.findViewById(R.id.tvFilePath);
        etFilePath = (EditText)llFilePathMain.findViewById(R.id.etFilePath);
        
        tvChannel = (TextView)llChannelMain.findViewById(R.id.tvChannel);
        etChannel = (EditText)llChannelMain.findViewById(R.id.etChannel);
        
        tvSample  = (TextView)llSampleMain.findViewById(R.id.tvSample);
        etSample  = (EditText)llSampleMain.findViewById(R.id.etSample);
        
        etFrame = (EditText)llConfigMain.findViewById(R.id.etFrame);
        spYuvType = (Spinner)llConfigMain.findViewById(R.id.spPcmType);
        
        btDecode = (Button)findViewById(R.id.btDecode);
        
        tvFileName.setText(getResources().getString(R.string.decoder_pcm_filename));
        tvFilePath.setText(getResources().getString(R.string.decoder_pcm_filepath));
        etFileName.setHint(getResources().getString(R.string.decoder_pcm_filename_hint));
        etFilePath.setHint(getResources().getString(R.string.decoder_pcm_filepath_hint));
        tvChannel.setText(getResources().getString(R.string.decoder_pcm_channel));
        etChannel.setHint(getResources().getString(R.string.decoder_pcm_channel_hint));
        tvSample.setText(getResources().getString(R.string.decoder_pcm_sample));
        etSample.setHint(getResources().getString(R.string.decoder_pcm_sample_hint));
        etFrame.setHint(getResources().getString(R.string.decoder_pcm_frame));
        btDecode.setText(getResources().getString(R.string.decoder_pcm_decode));
        
        String yuv_list[] = getResources().getStringArray(R.array.pcm_list);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, yuv_list);
        spYuvType.setAdapter(adapter);
        spYuvType.setOnItemSelectedListener(this);
        
        btDecode.setOnClickListener(this);
    }

    private void initData() {
        pcm_type = new int[]{
                FFmpegHelper.FFMPEG_SAMPLE_FMT_U8,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S16,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S32,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_FLT,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_DBL,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S64,
                
                FFmpegHelper.FFMPEG_SAMPLE_FMT_U8P,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S16P,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S32P,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_FLTP,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_DBLP,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S64P
        };
        pcm_idx = 0;
    }
    
    @Override
    public void onClick(View arg0) {
        String filename = etFileName.getText().toString();
        String filepath = etFilePath.getText().toString();
        String channel  = etChannel.getText().toString();
        String sample   = etSample.getText().toString();
        String framestr = etFrame.getText().toString();
        
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.decoder_pcm_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filepath == null || filepath.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.decoder_pcm_toast_filepath), Toast.LENGTH_SHORT).show();
            return;
        }
        if (channel == null || channel.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.decoder_pcm_toast_channel), Toast.LENGTH_SHORT).show();
            return;
        }
        if (sample == null || sample.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.decoder_pcm_toast_sample), Toast.LENGTH_SHORT).show();
            return;
        }
        if (framestr == null || framestr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.decoder_pcm_toast_frame), Toast.LENGTH_SHORT).show();
            return;
        }
        int frame = Integer.parseInt(framestr);
        int channel_value = Integer.parseInt(channel);
        int sample_value = Integer.parseInt(sample);
        int pcm_value = pcm_type[pcm_idx];
        
        new MyThread(filename, filepath, channel_value, sample_value, frame, pcm_value).start();
    }

    @Override
    public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
        pcm_idx = position;
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
        private int    channel;
        private int    sample;
        private int    frame;
        private int    pcm;
        
        public MyThread(String filename, String filepath, int channel, int sample, int frame, int pcm) {
            this.filename = filename;
            this.filepath = filepath;
            this.channel  = channel;
            this.sample   = sample;
            this.frame    = frame;
            this.pcm      = pcm;
        }
        
        @Override
        public void run() {
            super.run();
            FFmpegHelper.decoderAudioToPcm(filename, filepath, frame, channel, sample, pcm);
        }
    }
}