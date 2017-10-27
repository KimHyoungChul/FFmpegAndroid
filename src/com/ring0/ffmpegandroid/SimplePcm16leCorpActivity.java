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

public class SimplePcm16leCorpActivity extends Activity implements OnClickListener {
    
    private LinearLayout llSrcfile;
    private LinearLayout llDstfile;
    private LinearLayout llCorp;
    
    private TextView tvSrcfile;
    private EditText etSrcfile;
    private TextView tvDstfile;
    private EditText etDstfile;
    private EditText etStartPoint;
    private EditText etCount;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_pcm16le_corp);
        initView();
        initData();
    }
    
    private void initView() {
        llSrcfile = (LinearLayout)findViewById(R.id.llSrcfile);
        llDstfile = (LinearLayout)findViewById(R.id.llDstfile);
        llCorp = (LinearLayout)findViewById(R.id.llCorp);
        
        tvSrcfile = (TextView)llSrcfile.findViewById(R.id.tvSrcfile);
        tvSrcfile.setText(getResources().getString(R.string.simple_pcm16le_corp_srcfile));
        
        etSrcfile = (EditText)llSrcfile.findViewById(R.id.etSrcfile);
        etSrcfile.setHint(getResources().getString(R.string.simple_pcm16le_corp_srcfile_hint));
        
        tvDstfile = (TextView)llDstfile.findViewById(R.id.tvDstfile);
        tvDstfile.setText(getResources().getString(R.string.simple_pcm16le_corp_dstfile));
        
        etDstfile = (EditText)llDstfile.findViewById(R.id.etDstfile);
        etDstfile.setHint(getResources().getString(R.string.simple_pcm16le_corp_dstfile_hint));
        
        etStartPoint = (EditText)llCorp.findViewById(R.id.etStartPoint);
        etStartPoint.setHint(getResources().getString(R.string.simple_pcm16le_corp_startpoint_hint));
        
        etCount = (EditText)llCorp.findViewById(R.id.etCount);
        etCount.setHint(getResources().getString(R.string.simple_pcm16le_corp_count_hint));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_pcm16le_corp_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String srcfile = etSrcfile.getText().toString();
        String dstfile = etDstfile.getText().toString();
        String startstr = etStartPoint.getText().toString();
        String countstr = etCount.getText().toString();
        
        if (srcfile == null || srcfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_pcm16le_corp_toast_srcfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (dstfile == null || dstfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_pcm16le_corp_toast_dstfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (startstr == null || startstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_pcm16le_corp_toast_startpoint), Toast.LENGTH_SHORT).show();
            return;
        }
        if (countstr == null || countstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_pcm16le_corp_toast_count), Toast.LENGTH_SHORT).show();
            return;
        }
        int start = Integer.parseInt(startstr);
        int count = Integer.parseInt(countstr);
        FFmpegHelper.simple_pcm16le_corp(srcfile, dstfile, start, count);
    }
}
