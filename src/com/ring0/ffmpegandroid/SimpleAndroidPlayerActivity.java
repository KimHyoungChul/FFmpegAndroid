package com.ring0.ffmpegandroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.VideoView;

public class SimpleAndroidPlayerActivity extends Activity implements OnClickListener {
    private VideoView vVideo;
    private LinearLayout llFilenameMain;
    private TextView tvFilename;
    private EditText etFilename;
    private Button btSet;
    private Button btStart;
    private Button btPause;
    private Button btStop;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_android_player);
        
        vVideo = (VideoView)findViewById(R.id.vVideo);
        llFilenameMain = (LinearLayout)findViewById(R.id.llFilenameMain);
        
        tvFilename = (TextView)llFilenameMain.findViewById(R.id.tvFilename);
        tvFilename.setText(getResources().getString(R.string.simple_android_player_filename));
        
        etFilename = (EditText)llFilenameMain.findViewById(R.id.etFilename);
        etFilename.setHint(getResources().getString(R.string.simple_android_player_filehint));
        
        btSet = (Button)findViewById(R.id.btSet);
        btSet.setText(getResources().getString(R.string.simple_android_player_set));
        
        btStart = (Button)findViewById(R.id.btStart);
        btStart.setText(getResources().getString(R.string.simple_android_player_start));
        
        btPause = (Button)findViewById(R.id.btPause);
        btPause.setText(getResources().getString(R.string.simple_android_player_pause));
        
        btStop = (Button)findViewById(R.id.btStop);
        btStop.setText(getResources().getString(R.string.simple_android_player_stop));
        
        btSet.setOnClickListener(this);
        btStart.setOnClickListener(this);
        btPause.setOnClickListener(this);
        btStop.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.btSet) {
            String filename = etFilename.getText().toString();
            if (filename == null || filename.equals("")) {
                Toast.makeText(this, getResources().getString(R.string.simple_android_player_toast_filename), Toast.LENGTH_SHORT).show();
                return;
            }
            vVideo.setVideoPath(filename);
        }
        else if (id == R.id.btStart) {
            vVideo.start();
        }
        else if (id == R.id.btPause) {
            vVideo.pause();
        }
        else if (id == R.id.btStop) {
            vVideo.stopPlayback();
        }
    }
    
    
}
