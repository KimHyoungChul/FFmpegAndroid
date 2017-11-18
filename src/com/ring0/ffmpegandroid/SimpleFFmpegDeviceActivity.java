package com.ring0.ffmpegandroid;

import com.ring0.ffmpeg.FFmpegHelper;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class SimpleFFmpegDeviceActivity extends Activity implements OnClickListener{
    private Button btPlay = null;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.simple_ffmpeg_device);
        
        btPlay = (Button)findViewById(R.id.btPlay);
        btPlay.setText(getResources().getString(R.string.simple_ffmpeg_device_start));
        btPlay.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        FFmpegHelper.simple_ffmpeg_avdevice();
    }
}
