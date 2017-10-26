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

public class SimpleRgb24Yuv420pActivity extends Activity implements OnClickListener {
    
    private LinearLayout llSrcfile;
    private LinearLayout llDstfile;
    private LinearLayout llSize;
    
    private TextView tvSrcfile;
    private EditText etSrcfile;
    private TextView tvDstfile;
    private EditText etDstfile;
    private EditText etWidth;
    private EditText etHeight;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_rgb24_yuv420p);
        initView();
        initData();
    }
    
    private void initView() {
        llSrcfile = (LinearLayout)findViewById(R.id.llSrcfile);
        llDstfile = (LinearLayout)findViewById(R.id.llDstfile);
        llSize = (LinearLayout)findViewById(R.id.llSize);
        
        tvSrcfile = (TextView)llSrcfile.findViewById(R.id.tvSrcfile);
        tvSrcfile.setText(getResources().getString(R.string.simple_rgb24_yuv420p_srcfile));
        
        etSrcfile = (EditText)llSrcfile.findViewById(R.id.etSrcfile);
        etSrcfile.setHint(getResources().getString(R.string.simple_rgb24_yuv420p_srcfile_hint));
        
        tvDstfile = (TextView)llDstfile.findViewById(R.id.tvDstfile);
        tvDstfile.setText(getResources().getString(R.string.simple_rgb24_yuv420p_dstfile));
        
        etDstfile = (EditText)llDstfile.findViewById(R.id.etDstfile);
        etDstfile.setHint(getResources().getString(R.string.simple_rgb24_yuv420p_dstfile_hint));
        
        etWidth = (EditText)llSize.findViewById(R.id.etWidth);
        etWidth.setHint(getResources().getString(R.string.simple_rgb24_yuv420p_width));
        
        etHeight = (EditText)llSize.findViewById(R.id.etHeight);
        etHeight.setHint(getResources().getString(R.string.simple_rgb24_yuv420p_height));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_rgb24_yuv420p_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String srcfile = etSrcfile.getText().toString();
        String dstfile = etDstfile.getText().toString();
        String widthstr = etWidth.getText().toString();
        String heightstr = etHeight.getText().toString();
        
        if (srcfile == null || srcfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_rgb24_yuv420p_toast_srcfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (dstfile == null || dstfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_rgb24_yuv420p_toast_dstfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (widthstr == null || widthstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_rgb24_yuv420p_toast_width), Toast.LENGTH_SHORT).show();
            return;
        }
        if (heightstr == null || heightstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_rgb24_yuv420p_toast_height), Toast.LENGTH_SHORT).show();
            return;
        }
        int width = Integer.parseInt(widthstr);
        int height = Integer.parseInt(heightstr);
        FFmpegHelper.simple_rgb24_yuv420p(srcfile, dstfile, width, height);
    }
}
