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

public class SimpleYuv444pSplitActivity extends Activity implements OnClickListener {
    
    private LinearLayout llFilename;
    private LinearLayout llPath;
    private LinearLayout llSize;
    
    private TextView tvFilename;
    private EditText etFilename;
    private TextView tvPath;
    private EditText etPath;
    private EditText etWidth;
    private EditText etHeight;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_yuv444p_split);
        initView();
        initData();
    }
    
    private void initView() {
        llFilename = (LinearLayout)findViewById(R.id.llFilename);
        llPath = (LinearLayout)findViewById(R.id.llPath);
        llSize = (LinearLayout)findViewById(R.id.llSize);
        
        tvFilename = (TextView)llFilename.findViewById(R.id.tvFilename);
        tvFilename.setText(getResources().getString(R.string.simple_yuv444p_split_filename));
        
        etFilename = (EditText)llFilename.findViewById(R.id.etFilename);
        etFilename.setHint(getResources().getString(R.string.simple_yuv444p_split_filename_hint));
        
        tvPath = (TextView)llPath.findViewById(R.id.tvPath);
        tvPath.setText(getResources().getString(R.string.simple_yuv444p_split_filepath));
        
        etPath = (EditText)llPath.findViewById(R.id.etPath);
        etPath.setHint(getResources().getString(R.string.simple_yuv444p_split_filepath_hint));
        
        etWidth = (EditText)llSize.findViewById(R.id.etWidth);
        etWidth.setHint(getResources().getString(R.string.simple_yuv444p_split_width));
        
        etHeight = (EditText)llSize.findViewById(R.id.etHeight);
        etHeight.setHint(getResources().getString(R.string.simple_yuv444p_split_height));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_yuv444p_split_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String filename = etFilename.getText().toString();
        String filepath = etPath.getText().toString();
        String widthstr = etWidth.getText().toString();
        String heightstr = etHeight.getText().toString();
        
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv444p_split_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filepath == null || filepath.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv444p_split_toast_filepath), Toast.LENGTH_SHORT).show();
            return;
        }
        if (widthstr == null || widthstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv444p_split_toast_width), Toast.LENGTH_SHORT).show();
            return;
        }
        if (heightstr == null || heightstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv444p_split_toast_height), Toast.LENGTH_SHORT).show();
            return;
        }
        int width = Integer.parseInt(widthstr);
        int height = Integer.parseInt(heightstr);
        FFmpegHelper.simple_yuv444p_split(filename, filepath, width, height);
    }
}
