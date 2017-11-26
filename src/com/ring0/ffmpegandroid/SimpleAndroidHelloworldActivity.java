package com.ring0.ffmpegandroid;

import com.ring0.ffmpeg.FFmpegHelper;

import android.app.Activity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.widget.TextView;

public class SimpleAndroidHelloworldActivity extends Activity {
    
    private TextView tvInfo = null;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.simple_android_helloworld);
        
        tvInfo = (TextView)findViewById(R.id.tvInfo);
        tvInfo.setMovementMethod(ScrollingMovementMethod.getInstance());
        tvInfo.setText(FFmpegHelper.simple_android_helloworld());
    }
}
