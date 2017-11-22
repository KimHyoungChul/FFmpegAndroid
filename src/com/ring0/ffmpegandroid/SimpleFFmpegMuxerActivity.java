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

public class SimpleFFmpegMuxerActivity extends Activity implements OnClickListener {
    
    private LinearLayout llVideofile;
    private LinearLayout llAudiofile;
    private LinearLayout llMuxerfile;
    
    private TextView tvVideofile;
    private EditText etVideofile;
    private TextView tvAudiofile;
    private EditText etAudiofile;
    private TextView tvMuxerfile;
    private EditText etMuxerfile;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_ffmpeg_muxer);
        initView();
        initData();
    }
    
    private void initView() {
        llVideofile = (LinearLayout)findViewById(R.id.llVideofile);
        llAudiofile = (LinearLayout)findViewById(R.id.llAudiofile);
        llMuxerfile = (LinearLayout)findViewById(R.id.llMuxerfile);
        
        tvVideofile = (TextView)llVideofile.findViewById(R.id.tvVideofile);
        tvVideofile.setText(getResources().getString(R.string.simple_ffmpeg_muxer_videofile));
        
        etVideofile = (EditText)llVideofile.findViewById(R.id.etVideofile);
        etVideofile.setHint(getResources().getString(R.string.simple_ffmpeg_muxer_videofile_hint));
        
        tvAudiofile = (TextView)llAudiofile.findViewById(R.id.tvAudiofile);
        tvAudiofile.setText(getResources().getString(R.string.simple_ffmpeg_muxer_audiofile));
        
        etAudiofile = (EditText)llAudiofile.findViewById(R.id.etAudiofile);
        etAudiofile.setHint(getResources().getString(R.string.simple_ffmpeg_muxer_audiofile_hint));
        
        tvMuxerfile = (TextView)llMuxerfile.findViewById(R.id.tvMuxerfile);
        tvMuxerfile.setText(getResources().getString(R.string.simple_ffmpeg_muxer_muxerfile));
        
        etMuxerfile = (EditText)llMuxerfile.findViewById(R.id.etMuxerfile);
        etMuxerfile.setHint(getResources().getString(R.string.simple_ffmpeg_muxer_muxerfile_hint));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_ffmpeg_muxer_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String videofile = etVideofile.getText().toString();
        String audiofile = etAudiofile.getText().toString();
        String muxerfile = etMuxerfile.getText().toString();
        
        if (videofile == null || videofile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_muxer_toast_videofile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (audiofile == null || audiofile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_muxer_toast_audiofile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (muxerfile == null || muxerfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_ffmpeg_muxer_toast_muxerfile), Toast.LENGTH_SHORT).show();
            return;
        }
        FFmpegHelper.simple_ffmpeg_muxer(videofile, audiofile, muxerfile);
    }
}
