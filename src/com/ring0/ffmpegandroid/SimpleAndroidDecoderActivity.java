package com.ring0.ffmpegandroid;

import com.ring0.ffmpeg.FFmpegHelper;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class SimpleAndroidDecoderActivity extends Activity implements OnClickListener {
    
    private LinearLayout llFilename;
    private LinearLayout llPath;
    
    private TextView tvFilename;
    private EditText etFilename;
    private TextView tvPath;
    private EditText etPath;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_android_decoder);
        initView();
        initData();
    }
    
    private void initView() {
        llFilename = (LinearLayout)findViewById(R.id.llFilename);
        llPath = (LinearLayout)findViewById(R.id.llPath);
        
        tvFilename = (TextView)llFilename.findViewById(R.id.tvFilename);
        tvFilename.setText(getResources().getString(R.string.simple_android_decoder_filename));
        
        etFilename = (EditText)llFilename.findViewById(R.id.etFilename);
        etFilename.setHint(getResources().getString(R.string.simple_android_decoder_filename_hint));
        
        tvPath = (TextView)llPath.findViewById(R.id.tvPath);
        tvPath.setText(getResources().getString(R.string.simple_android_decoder_filepath));
        
        etPath = (EditText)llPath.findViewById(R.id.etPath);
        etPath.setHint(getResources().getString(R.string.simple_android_decoder_filepath_hint));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_android_decoder_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String filename = etFilename.getText().toString();
        String filepath = etPath.getText().toString();
        
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_android_decoder_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filepath == null || filepath.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_android_decoder_toast_filepath), Toast.LENGTH_SHORT).show();
            return;
        }
        FFmpegHelper.simple_android_decoder(filename, filepath);
    }
}
