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

public class SimpleFFmpegDecoderActivity extends Activity implements OnClickListener, OnItemSelectedListener {
    private LinearLayout llFileNameMain; // 文件名根布局
    private LinearLayout llFilePathMain; // 文件路径根布局
    
    private TextView tvFileName;
    private EditText etFileName;
    private TextView tvFilePath;
    private EditText etFilePath;
    private Spinner  spDecoderType;
    private Button   btDecode;
    
    private int[] decoder_type;
    private int   decoder_idx;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.simple_ffmpeg_decoder);
        
        initView();
        initData();
    }
    
    private void initView() {
        llFileNameMain = (LinearLayout)findViewById(R.id.llFileNameMain);
        llFilePathMain = (LinearLayout)findViewById(R.id.llFilePathMain);
        
        tvFileName = (TextView)llFileNameMain.findViewById(R.id.tvFileName);
        etFileName = (EditText)llFileNameMain.findViewById(R.id.etFileName);
        
        tvFilePath = (TextView)llFilePathMain.findViewById(R.id.tvFilePath);
        etFilePath = (EditText)llFilePathMain.findViewById(R.id.etFilePath);
        
        spDecoderType = (Spinner)findViewById(R.id.spDecoderType);
        
        btDecode = (Button)findViewById(R.id.btDecode);
        
        tvFileName.setText(getResources().getString(R.string.simple_ffmpeg_decoder_filename));
        tvFilePath.setText(getResources().getString(R.string.simple_ffmpeg_decoder_filepath));
        etFileName.setHint(getResources().getString(R.string.simple_ffmpeg_decoder_filename_hint));
        etFilePath.setHint(getResources().getString(R.string.simple_ffmpeg_decoder_filepath_hint));
        btDecode.setText(getResources().getString(R.string.simple_ffmpeg_decoder_decode));
        
        String decoder_list[] = getResources().getStringArray(R.array.simple_decoder_list);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, decoder_list);
        spDecoderType.setAdapter(adapter);
        spDecoderType.setOnItemSelectedListener(this);
        
        btDecode.setOnClickListener(this);
    }

    private void initData() {
        decoder_type = new int[]{
                FFmpegHelper.FFMPEG_DECODER_MPEG4,
                FFmpegHelper.FFMPEG_DECODER_H264,
                FFmpegHelper.FFMPEG_DECODER_H265,
                FFmpegHelper.FFMPEG_DECODER_VP8,
                FFmpegHelper.FFMPEG_DECODER_VP9
        };
        decoder_idx = 0;
    }
    
    @Override
    public void onClick(View arg0) {
        String filename = etFileName.getText().toString();
        String filepath = etFilePath.getText().toString();
        
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_decoder_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filepath == null || filepath.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_decoder_toast_filepath), Toast.LENGTH_SHORT).show();
            return;
        }
        int decoder_value = decoder_type[decoder_idx];
        
        new MyThread(filename, filepath, decoder_value).start();
    }

    @Override
    public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
        decoder_idx = position;
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
        private int    decoder_type;
        
        public MyThread(String filename, String filepath, int decoder_type) {
            this.filename = filename;
            this.filepath = filepath;
            this.decoder_type = decoder_type;
        }
        
        @Override
        public void run() {
            super.run();
            FFmpegHelper.simple_ffmpeg_decoder(filename, filepath, decoder_type);
        }
    }
}
