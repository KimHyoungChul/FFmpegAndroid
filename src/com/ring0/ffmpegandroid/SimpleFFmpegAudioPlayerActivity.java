package com.ring0.ffmpegandroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Toast;

import com.ring0.ffmpeg.FFmpegHelper;

public class SimpleFFmpegAudioPlayerActivity extends Activity implements OnClickListener {
    private LinearLayout llFileNameMain; // 文件名根布局
    
    private TextView tvFileName;
    private EditText etFileName;
    private Button   btDecode;
    private SeekBar  sbStereo;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.simple_ffmpeg_audio_player);
        
        initView();
    }
    
    private void initView() {
        llFileNameMain = (LinearLayout)findViewById(R.id.llFileNameMain);
        
        tvFileName = (TextView)llFileNameMain.findViewById(R.id.tvFileName);
        etFileName = (EditText)llFileNameMain.findViewById(R.id.etFileName);
        btDecode = (Button)findViewById(R.id.btDecode);
        sbStereo = (SeekBar)findViewById(R.id.sbStereo);
        sbStereo.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                System.out.println("zd-progress: " + seekBar.getProgress());
                FFmpegHelper.simple_ffmpeg_audio_player_stereo(seekBar.getProgress());
            }
            
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress,
                    boolean fromUser) {
            }
        });
        
        tvFileName.setText(getResources().getString(R.string.simple_ffmpeg_audio_player_filename));
        etFileName.setHint(getResources().getString(R.string.simple_ffmpeg_audio_player_filename_hint));
        btDecode.setText(getResources().getString(R.string.simple_ffmpeg_audio_player_decode));
        
        btDecode.setOnClickListener(this);
    }
    
    @Override
    public void onClick(View arg0) {
        String filename = etFileName.getText().toString();
        if (filename == null || filename.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_audio_player_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        
        //new MyThread(filename).start();
        FFmpegHelper.simple_ffmpeg_audio_player(filename);
    }
    /**
     * 解码线程
     */
    private class MyThread extends Thread {
        private String filename;
        
        public MyThread(String filename) {
            this.filename = filename;
        }
        
        @Override
        public void run() {
            super.run();
            FFmpegHelper.simple_ffmpeg_audio_player(filename);
        }
    }
}
