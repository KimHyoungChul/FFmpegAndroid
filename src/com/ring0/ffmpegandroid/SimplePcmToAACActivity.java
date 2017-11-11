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

public class SimplePcmToAACActivity extends Activity implements OnClickListener {
    
    private LinearLayout llSrcfile;
    private LinearLayout llDstfile;
    private LinearLayout llParams;
    
    private TextView tvSrcfile;
    private EditText etSrcfile;
    private TextView tvDstfile;
    private EditText etDstfile;
    private EditText etSampleRate;
    private EditText etChannel;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_pcm_to_aac);
        initView();
        initData();
    }
    
    private void initView() {
        llSrcfile = (LinearLayout)findViewById(R.id.llSrcfile);
        llDstfile = (LinearLayout)findViewById(R.id.llDstfile);
        llParams  = (LinearLayout)findViewById(R.id.llParams);
        
        tvSrcfile = (TextView)llSrcfile.findViewById(R.id.tvSrcfile);
        tvSrcfile.setText(getResources().getString(R.string.simple_pcm_to_aac_srcfile));
        
        etSrcfile = (EditText)llSrcfile.findViewById(R.id.etSrcfile);
        etSrcfile.setHint(getResources().getString(R.string.simple_pcm_to_aac_srcfile_hint));
        
        tvDstfile = (TextView)llDstfile.findViewById(R.id.tvDstfile);
        tvDstfile.setText(getResources().getString(R.string.simple_pcm_to_aac_dstfile));
        
        etDstfile = (EditText)llDstfile.findViewById(R.id.etDstfile);
        etDstfile.setHint(getResources().getString(R.string.simple_pcm_to_aac_dstfile_hint));
        
        etSampleRate = (EditText)llParams.findViewById(R.id.etSampleRate);
        etSampleRate.setHint(getResources().getString(R.string.simple_pcm_to_aac_sample_rate));
        
        etChannel = (EditText)llParams.findViewById(R.id.etChannel);
        etChannel.setHint(getResources().getString(R.string.simple_pcm_to_aac_channel));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_pcm_to_aac_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String srcfile       = etSrcfile.getText().toString();
        String dstfile       = etDstfile.getText().toString();
        String sampleratestr = etSampleRate.getText().toString();
        String channelstr    = etChannel.getText().toString();
        
        if (srcfile == null || srcfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_pcm_to_aac_toast_srcfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (dstfile == null || dstfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_pcm_to_aac_toast_dstfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (sampleratestr == null || sampleratestr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_pcm_to_aac_toast_sample_rate), Toast.LENGTH_SHORT).show();
            return;
        }
        if (channelstr == null || channelstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_pcm_to_aac_toast_channel), Toast.LENGTH_SHORT).show();
            return;
        }
        int sample_rate = Integer.parseInt(sampleratestr);
        int channel = Integer.parseInt(channelstr);
        FFmpegHelper.simple_pcm_to_aac(srcfile, dstfile, sample_rate, channel);
    }
}
