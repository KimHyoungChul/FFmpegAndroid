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

public class SimpleYuv420pFilterActivity extends Activity implements OnClickListener {
    
    private LinearLayout llSrcfile;
    private LinearLayout llFilterfile;
    private LinearLayout llSize;
    
    private TextView tvSrcfile;
    private EditText etSrcfile;
    private TextView tvFilterfile;
    private EditText etFilterfile;
    private TextView tvFrame;
    private EditText etFrame;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_yuv420p_filter);
        initView();
        initData();
    }
    
    private void initView() {
        llSrcfile = (LinearLayout)findViewById(R.id.llSrcfile);
        llFilterfile = (LinearLayout)findViewById(R.id.llFilterfile);
        llSize = (LinearLayout)findViewById(R.id.llSize);
        
        tvSrcfile = (TextView)llSrcfile.findViewById(R.id.tvSrcfile);
        tvSrcfile.setText(getResources().getString(R.string.simple_yuv420p_filter_srcfile));
        
        etSrcfile = (EditText)llSrcfile.findViewById(R.id.etSrcfile);
        etSrcfile.setHint(getResources().getString(R.string.simple_yuv420p_filter_srcfile_hint));
        
        tvFilterfile = (TextView)llFilterfile.findViewById(R.id.tvFilterfile);
        tvFilterfile.setText(getResources().getString(R.string.simple_yuv420p_filter_filterfile));
        
        etFilterfile = (EditText)llFilterfile.findViewById(R.id.etFilterfile);
        etFilterfile.setHint(getResources().getString(R.string.simple_yuv420p_filter_filterfile_hint));
        
        tvFrame = (TextView)llSize.findViewById(R.id.tvFrame);
        tvFrame.setText(getResources().getString(R.string.simple_yuv420p_filter_frame));
        
        etFrame = (EditText)llSize.findViewById(R.id.etFrame);
        etFrame.setHint(getResources().getString(R.string.simple_yuv420p_filter_frame_hint));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_yuv420p_filter_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String srcfile = etSrcfile.getText().toString();
        String filterfile = etFilterfile.getText().toString();
        String framestr = etFrame.getText().toString();
        
        if (srcfile == null || srcfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_filter_toast_srcfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filterfile == null || filterfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_filter_toast_filterfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (framestr == null || framestr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_filter_toast_frame), Toast.LENGTH_SHORT).show();
            return;
        }
        int frame = Integer.parseInt(framestr);
        FFmpegHelper.simple_yuv420p_to_filter(srcfile, filterfile, frame);
    }
}
