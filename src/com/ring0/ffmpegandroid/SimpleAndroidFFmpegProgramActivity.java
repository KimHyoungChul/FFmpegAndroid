package com.ring0.ffmpegandroid;

import com.ring0.ffmpeg.FFmpegHelper;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class SimpleAndroidFFmpegProgramActivity extends Activity implements OnClickListener {
    private EditText etCommand;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.simple_ffmpeg_program);
        
        etCommand = (EditText)findViewById(R.id.etCommand);
        etCommand.setHint(getResources().getString(R.string.simple_android_ffmpeg_program_command));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_android_ffmpeg_program_start));
        btnStart.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        String command = etCommand.getText().toString();
        if (command == null || command.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_android_ffmpeg_program_toast_command), Toast.LENGTH_SHORT).show();
            return;
        }
        String args[] = command.split(" ");
        FFmpegHelper.simple_ffmpeg(args.length, args);
    }
}
