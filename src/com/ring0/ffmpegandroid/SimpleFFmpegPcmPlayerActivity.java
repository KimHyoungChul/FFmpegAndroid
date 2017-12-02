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

public class SimpleFFmpegPcmPlayerActivity extends Activity implements OnClickListener {
    
    private LinearLayout llFilename;
    
    private TextView tvFilename;
    private EditText etFilename;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_ffmpeg_pcm_player);
        initView();
        initData();
    }
    
    private void initView() {
        llFilename = (LinearLayout)findViewById(R.id.llFilename);
        
        tvFilename = (TextView)llFilename.findViewById(R.id.tvFilename);
        tvFilename.setText(getResources().getString(R.string.simple_ffmpeg_pcm_player_filename));
        
        etFilename = (EditText)llFilename.findViewById(R.id.etFilename);
        etFilename.setHint(getResources().getString(R.string.simple_ffmpeg_pcm_player_filename_hint));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_ffmpeg_pcm_player_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String filename = etFilename.getText().toString();
        
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_pcm_player_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        FFmpegHelper.simple_ffmpeg_audio_pcm_player(filename);
    }
}
