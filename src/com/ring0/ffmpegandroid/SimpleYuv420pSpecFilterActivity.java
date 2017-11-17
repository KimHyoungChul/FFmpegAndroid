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

public class SimpleYuv420pSpecFilterActivity extends Activity implements OnClickListener {
    
    private LinearLayout llSrcfile;
    private LinearLayout llFilterfile;
    private LinearLayout llSize;
    private LinearLayout llFilter;
    
    private TextView tvSrcfile;
    private EditText etSrcfile;
    private TextView tvFilterfile;
    private EditText etFilterfile;
    private EditText etWidth;
    private EditText etHeight;
    private TextView tvFilter;
    private EditText etFilter;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_yuv420p_spec_filter);
        initView();
        initData();
    }
    
    private void initView() {
        llSrcfile = (LinearLayout)findViewById(R.id.llSrcfile);
        llFilterfile = (LinearLayout)findViewById(R.id.llFilterfile);
        llSize = (LinearLayout)findViewById(R.id.llSize);
        llFilter = (LinearLayout)findViewById(R.id.llFilter);
        
        tvSrcfile = (TextView)llSrcfile.findViewById(R.id.tvSrcfile);
        tvSrcfile.setText(getResources().getString(R.string.simple_yuv420p_spec_filter_srcfile));
        
        etSrcfile = (EditText)llSrcfile.findViewById(R.id.etSrcfile);
        etSrcfile.setHint(getResources().getString(R.string.simple_yuv420p_spec_filter_srcfile_hint));
        
        tvFilterfile = (TextView)llFilterfile.findViewById(R.id.tvFilterfile);
        tvFilterfile.setText(getResources().getString(R.string.simple_yuv420p_spec_filter_filterfile));
        
        etFilterfile = (EditText)llFilterfile.findViewById(R.id.etFilterfile);
        etFilterfile.setHint(getResources().getString(R.string.simple_yuv420p_spec_filter_filterfile_hint));
        
        etWidth = (EditText)llSize.findViewById(R.id.etWidth);
        etWidth.setHint(getResources().getString(R.string.simple_yuv420p_spec_filter_width_hint));
        
        etHeight = (EditText)llSize.findViewById(R.id.etHeight);
        etHeight.setHint(getResources().getString(R.string.simple_yuv420p_spec_filter_height_hint));
        
        tvFilter = (TextView)llFilter.findViewById(R.id.tvFilter);
        tvFilter.setText(
                "lutyuv='u=128:v=128'\n" +
        		"boxblur\n" +
        		"hflip\n" +
        		"hue='h=60:s=-3'\n" +
        		"crop=2/3*in_w:2/3*in_h\n" +
        		"drawbox=x=100:y=100:w=100:h=100:color=pink@0.5\n" +
        		"drawtext=fontfile=arial.ttf:fontcolor=green:fontsize=30:text='ring0'\n");
        
        etFilter = (EditText)llFilter.findViewById(R.id.etFilter);
        etFilter.setHint(getResources().getString(R.string.simple_yuv420p_spec_filter_filter_hint));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_yuv420p_spec_filter_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String srcfile = etSrcfile.getText().toString();
        String filterfile = etFilterfile.getText().toString();
        String filter = etFilter.getText().toString();
        String widthstr = etWidth.getText().toString();
        String heightstr = etHeight.getText().toString();
        
        if (srcfile == null || srcfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_spec_filter_toast_srcfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filterfile == null || filterfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_spec_filter_toast_filterfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (filter == null || filter.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_spec_filter_toast_filter), Toast.LENGTH_SHORT).show();
            return;
        }
        if (widthstr == null || widthstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_spec_filter_toast_width_hint), Toast.LENGTH_SHORT).show();
            return;
        }
        if (heightstr == null || heightstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_spec_filter_toast_height_hint), Toast.LENGTH_SHORT).show();
            return;
        }
        int width = Integer.parseInt(widthstr);
        int height = Integer.parseInt(heightstr);
        FFmpegHelper.simple_yuv420p_spec_filter(srcfile, filterfile, filter, width, height);
    }
}
