package com.ring0.ffmpegandroid;

import java.io.File;
import java.io.FileInputStream;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.ring0.ffmpeg.FFmpegHelper;
import com.ring0.ffmpegandroid.ui.PcmView;
import com.ring0.opensl.OpenSLHelper;

public class PlayerPcmActivity extends Activity implements OnClickListener, OnItemSelectedListener {
    
    private LinearLayout llPathMain;
    private LinearLayout llPcmMain;
    private LinearLayout llChannelMain;
    private LinearLayout llSampleMain;
    private PcmView vPcm;
    
    private TextView tvFileName;
    private EditText etFileName;
    private TextView tvPcm;
    private Spinner  spPcmType;
    private TextView tvChannel;
    private EditText etChannel;
    private TextView tvSample;
    private EditText etSample;
    private Button   btPlay;
    
    private int pcm_type[];
    private int pcm_idx;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.player_pcm);
        initView();
        initData();
    }
    
    private void initView() {
        llPathMain    = (LinearLayout)findViewById(R.id.llPathMain);
        llPcmMain     = (LinearLayout)findViewById(R.id.llPcmMain);
        llChannelMain = (LinearLayout)findViewById(R.id.llChannelMain);
        llSampleMain  = (LinearLayout)findViewById(R.id.llSampleMain);
        vPcm          = (PcmView)findViewById(R.id.vPcm);
        
        tvFileName    = (TextView)llPathMain.findViewById(R.id.tvFileName);
        etFileName    = (EditText)llPathMain.findViewById(R.id.etFileName);
        tvPcm         = (TextView)llPcmMain.findViewById(R.id.tvPcm);
        spPcmType     = (Spinner)llPcmMain.findViewById(R.id.spPcmType);
        tvChannel     = (TextView)llChannelMain.findViewById(R.id.tvChannel);
        etChannel     = (EditText)llChannelMain.findViewById(R.id.etChannel);
        tvSample      = (TextView)llSampleMain.findViewById(R.id.tvSample);
        etSample      = (EditText)llSampleMain.findViewById(R.id.etSample);
        btPlay        = (Button)findViewById(R.id.btPlay);
        
        tvFileName.setText(getResources().getString(R.string.player_pcm_filename));
        etFileName.setHint(getResources().getString(R.string.player_pcm_filename_hint));
        tvPcm.setText(getResources().getString(R.string.player_pcm_type));
        tvChannel.setText(getResources().getString(R.string.player_pcm_channel));
        etChannel.setHint(getResources().getString(R.string.player_pcm_channel_hint));
        tvSample.setText(getResources().getString(R.string.player_pcm_sample));
        etSample.setHint(getResources().getString(R.string.player_pcm_sample_hint));
        btPlay.setText(getResources().getString(R.string.player_pcm_play));
        btPlay.setOnClickListener(this);
        // 初始化 Spinner
        String pcm_list[] = getResources().getStringArray(R.array.pcm_list);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, pcm_list);
        spPcmType.setAdapter(adapter);
        spPcmType.setOnItemSelectedListener(this);
    }
    
    private void initData() {
        pcm_type = new int[] {
                FFmpegHelper.FFMPEG_SAMPLE_FMT_U8,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S16,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S32,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_FLT,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_DBL,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S64,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_U8P,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S16P,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S32P,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_FLTP,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_DBLP,
                FFmpegHelper.FFMPEG_SAMPLE_FMT_S64P
        };
        pcm_idx = 0;
    }

    @Override
    public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
        pcm_idx = position;
    }

    @Override
    public void onNothingSelected(AdapterView<?> arg0) {
    }

    @Override
    public void onClick(View v) {
        String tempFile    = etFileName.getText().toString();
        String tempChannel = etChannel.getText().toString();
        String tempSample  = etSample.getText().toString();
        if (tempFile == null || tempFile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.player_pcm_toast_filename), Toast.LENGTH_SHORT).show();
            return;
        }
        if (tempChannel == null || tempChannel.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.player_pcm_toast_channel), Toast.LENGTH_SHORT).show();
            return;
        }
        if (tempSample == null || tempSample.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.player_pcm_toast_sample), Toast.LENGTH_SHORT).show();
            return;
        }
        int channel   = Integer.parseInt(tempChannel);
        int sample    = Integer.parseInt(tempSample);
        int pcm_value = pcm_type[pcm_idx];
        
        FileInputStream fis = null;
        try {
            fis = new FileInputStream(new File(tempFile));
            int size = fis.available();
            byte[] buffer = new byte[size];
            fis.read(buffer, 0, size);
            vPcm.update(buffer);
            
            OpenSLHelper.opensl_play(buffer, size, channel, sample, pcm_value);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        finally {
            if (fis != null) {
                try {
                    fis.close();
                }catch (Exception e) {}
            }
        }
    }
    
}
