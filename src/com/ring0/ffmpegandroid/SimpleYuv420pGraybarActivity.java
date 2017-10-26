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

public class SimpleYuv420pGraybarActivity extends Activity implements OnClickListener {
    
    private LinearLayout llDstfile;
    private LinearLayout llLevel;
    private LinearLayout llSize;
    
    private TextView tvDstfile;
    private EditText etDstfile;
    private TextView tvLevel;
    private EditText etLevel;
    private EditText etWidth;
    private EditText etHeight;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_yuv420p_graybar);
        initView();
        initData();
    }
    
    private void initView() {
        llDstfile = (LinearLayout)findViewById(R.id.llDstfile);
        llLevel = (LinearLayout)findViewById(R.id.llLevel);
        llSize = (LinearLayout)findViewById(R.id.llSize);
        
        tvDstfile = (TextView)llDstfile.findViewById(R.id.tvDstfile);
        tvDstfile.setText(getResources().getString(R.string.simple_yuv420p_graybar_dstfile));
        
        etDstfile = (EditText)llDstfile.findViewById(R.id.etDstfile);
        etDstfile.setHint(getResources().getString(R.string.simple_yuv420p_graybar_dstfile_hint));
        
        tvLevel = (TextView)llLevel.findViewById(R.id.tvLevel);
        tvLevel.setText(getResources().getString(R.string.simple_yuv420p_graybar_level));
        
        etLevel = (EditText)llLevel.findViewById(R.id.etLevel);
        etLevel.setHint(getResources().getString(R.string.simple_yuv420p_graybar_level_hint));
        
        etWidth = (EditText)llSize.findViewById(R.id.etWidth);
        etWidth.setHint(getResources().getString(R.string.simple_yuv420p_graybar_width));
        
        etHeight = (EditText)llSize.findViewById(R.id.etHeight);
        etHeight.setHint(getResources().getString(R.string.simple_yuv420p_graybar_height));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_yuv420p_graybar_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String dstfile = etDstfile.getText().toString();
        String levelstr = etLevel.getText().toString();
        String widthstr = etWidth.getText().toString();
        String heightstr = etHeight.getText().toString();
        
        if (dstfile == null || dstfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_graybar_toast_dstfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (levelstr == null || levelstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_graybar_toast_level), Toast.LENGTH_SHORT).show();
            return;
        }
        if (widthstr == null || widthstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_graybar_toast_width), Toast.LENGTH_SHORT).show();
            return;
        }
        if (heightstr == null || heightstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_graybar_toast_height), Toast.LENGTH_SHORT).show();
            return;
        }
        int level = Integer.parseInt(levelstr);
        int width = Integer.parseInt(widthstr);
        int height = Integer.parseInt(heightstr);
        FFmpegHelper.simple_yuv420p_graybar(dstfile, level, width, height);
    }
}
