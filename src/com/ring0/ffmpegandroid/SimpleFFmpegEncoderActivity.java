package com.ring0.ffmpegandroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;

import com.ring0.ffmpeg.FFmpegHelper;

public class SimpleFFmpegEncoderActivity extends Activity implements OnClickListener, OnItemSelectedListener {
    
    private LinearLayout llSrcfile;
    private LinearLayout llPath;
    private LinearLayout llSize;
    private LinearLayout llYuvMain;
    
    private TextView tvSrcfile;
    private EditText etSrcfile;
    private TextView tvPath;
    private EditText etPath;
    private EditText etWidth;
    private EditText etHeight;
    private TextView tvYuv;
    private Spinner  spYuvType;
    private Button btnStart;
    
    private int yuv_type[];
    private int yuv_idx;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_ffmpeg_encoder);
        initView();
        initData();
    }
    
    private void initView() {
        llSrcfile = (LinearLayout)findViewById(R.id.llSrcfile);
        llPath    = (LinearLayout)findViewById(R.id.llPath);
        llSize    = (LinearLayout)findViewById(R.id.llSize);
        llYuvMain = (LinearLayout)findViewById(R.id.llYuvMain);
        
        tvSrcfile = (TextView)llSrcfile.findViewById(R.id.tvSrcfile);
        tvSrcfile.setText(getResources().getString(R.string.simple_ffmpeg_encoder_srcfile));
        
        etSrcfile = (EditText)llSrcfile.findViewById(R.id.etSrcfile);
        etSrcfile.setHint(getResources().getString(R.string.simple_ffmpeg_encoder_srcfile_hint));
        
        tvPath = (TextView)llPath.findViewById(R.id.tvPath);
        tvPath.setText(getResources().getString(R.string.simple_ffmpeg_encoder_path));
        
        etPath = (EditText)llPath.findViewById(R.id.etPath);
        etPath.setHint(getResources().getString(R.string.simple_ffmpeg_encoder_path_hint));
        
        etWidth = (EditText)llSize.findViewById(R.id.etWidth);
        etWidth.setHint(getResources().getString(R.string.simple_ffmpeg_encoder_width));
        
        etHeight = (EditText)llSize.findViewById(R.id.etHeight);
        etHeight.setHint(getResources().getString(R.string.simple_ffmpeg_encoder_height));
        
        tvYuv     = (TextView)llYuvMain.findViewById(R.id.tvYuv);
        tvYuv.setText(getResources().getString(R.string.simple_ffmpeg_encoder_yuv));
        spYuvType = (Spinner)llYuvMain.findViewById(R.id.spYuvType);
        
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(
                this, 
                android.R.layout.simple_spinner_item, 
                getResources().getStringArray(R.array.simple_encoder_list));
        spYuvType.setAdapter(adapter);
        spYuvType.setOnItemSelectedListener(this);
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_ffmpeg_encoder_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        yuv_type = new int[]{
                FFmpegHelper.FFMPEG_ENCODER_MPEG4,
                FFmpegHelper.FFMPEG_ENCODER_H264,
                FFmpegHelper.FFMPEG_ENCODER_H265,
                FFmpegHelper.FFMPEG_ENCODER_VP8,
                FFmpegHelper.FFMPEG_ENCODER_VP9
        };
        yuv_idx = 0;
    }

    @Override
    public void onClick(View v) {
        String srcfile   = etSrcfile.getText().toString();
        String path      = etPath.getText().toString();
        String widthstr  = etWidth.getText().toString();
        String heightstr = etHeight.getText().toString();
        
        if (srcfile == null || srcfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_encoder_toast_srcfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (path == null || path.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_encoder_toast_path), Toast.LENGTH_SHORT).show();
            return;
        }
        if (widthstr == null || widthstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_encoder_toast_width), Toast.LENGTH_SHORT).show();
            return;
        }
        if (heightstr == null || heightstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_encoder_toast_height), Toast.LENGTH_SHORT).show();
            return;
        }
        int width     = Integer.parseInt(widthstr);
        int height    = Integer.parseInt(heightstr);
        int yuv_value = yuv_type[yuv_idx];
        FFmpegHelper.simple_ffmpeg_encoder(srcfile, path, width, height, yuv_value);
    }

    @Override
    public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
        yuv_idx = position;
    }

    @Override
    public void onNothingSelected(AdapterView<?> arg0) {
        
    }
}
