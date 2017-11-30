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

public class SimpleFFmpegPusherActivity extends Activity implements OnClickListener {
    
    private LinearLayout llUrl;
    private LinearLayout llFile;
    
    private TextView tvUrl;
    private EditText etUrl;
    private TextView tvFile;
    private EditText etFile;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_ffmpeg_pusher);
        initView();
        initData();
    }
    
    private void initView() {
        llUrl = (LinearLayout)findViewById(R.id.llUrl);
        llFile = (LinearLayout)findViewById(R.id.llFile);
        
        tvUrl = (TextView)llUrl.findViewById(R.id.tvUrl);
        tvUrl.setText(getResources().getString(R.string.simple_ffmpeg_pusher_filename));
        
        etUrl = (EditText)llUrl.findViewById(R.id.etUrl);
        etUrl.setHint(getResources().getString(R.string.simple_ffmpeg_pusher_filename_hint));
        etUrl.setText("rtmp://192.168.31.84/live");
        
        tvFile = (TextView)llFile.findViewById(R.id.tvFile);
        tvFile.setText(getResources().getString(R.string.simple_ffmpeg_pusher_filepath));
        
        etFile = (EditText)llFile.findViewById(R.id.etFile);
        etFile.setHint(getResources().getString(R.string.simple_ffmpeg_pusher_filepath_hint));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_ffmpeg_pusher_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String filename = etUrl.getText().toString();
        String filepath = etFile.getText().toString();
        
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_pusher_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filepath == null || filepath.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_pusher_toast_filepath), Toast.LENGTH_SHORT).show();
            return;
        }
        FFmpegHelper.simple_ffmpeg_pusher(filename, filepath);
    }
}
