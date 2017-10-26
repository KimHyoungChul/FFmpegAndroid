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

public class SimpleYuv420pBoardActivity extends Activity implements OnClickListener {
    
    private LinearLayout llSrcfile;
    private LinearLayout llDstfile;
    private LinearLayout llBoard;
    private LinearLayout llSize;
    
    private TextView tvSrcfile;
    private EditText etSrcfile;
    private TextView tvDstfile;
    private EditText etDstfile;
    private TextView tvBoard;
    private EditText etBoard;
    private EditText etWidth;
    private EditText etHeight;
    private Button btnStart;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.simple_yuv420p_board);
        initView();
        initData();
    }
    
    private void initView() {
        llSrcfile = (LinearLayout)findViewById(R.id.llSrcfile);
        llDstfile = (LinearLayout)findViewById(R.id.llDstfile);
        llBoard = (LinearLayout)findViewById(R.id.llBoard);
        llSize = (LinearLayout)findViewById(R.id.llSize);
        
        tvSrcfile = (TextView)llSrcfile.findViewById(R.id.tvSrcfile);
        tvSrcfile.setText(getResources().getString(R.string.simple_yuv420p_board_srcfile));
        
        etSrcfile = (EditText)llSrcfile.findViewById(R.id.etSrcfile);
        etSrcfile.setHint(getResources().getString(R.string.simple_yuv420p_board_srcfile_hint));
        
        tvDstfile = (TextView)llDstfile.findViewById(R.id.tvDstfile);
        tvDstfile.setText(getResources().getString(R.string.simple_yuv420p_board_dstfile));
        
        etDstfile = (EditText)llDstfile.findViewById(R.id.etDstfile);
        etDstfile.setHint(getResources().getString(R.string.simple_yuv420p_board_dstfile_hint));
        
        tvBoard = (TextView)llBoard.findViewById(R.id.tvBoard);
        tvBoard.setText(getResources().getString(R.string.simple_yuv420p_board_board));
        
        etBoard = (EditText)llBoard.findViewById(R.id.etBoard);
        etBoard.setHint(getResources().getString(R.string.simple_yuv420p_board_board_hint));
        
        etWidth = (EditText)llSize.findViewById(R.id.etWidth);
        etWidth.setHint(getResources().getString(R.string.simple_yuv420p_board_width));
        
        etHeight = (EditText)llSize.findViewById(R.id.etHeight);
        etHeight.setHint(getResources().getString(R.string.simple_yuv420p_board_height));
        
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStart.setText(getResources().getString(R.string.simple_yuv420p_board_start));
        btnStart.setOnClickListener(this);
    }
    
    private void initData() {
        
    }

    @Override
    public void onClick(View v) {
        String srcfile = etSrcfile.getText().toString();
        String dstfile = etDstfile.getText().toString();
        String boardstr = etBoard.getText().toString();
        String widthstr = etWidth.getText().toString();
        String heightstr = etHeight.getText().toString();
        
        if (srcfile == null || srcfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_board_toast_srcfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (dstfile == null || dstfile.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_board_toast_dstfile), Toast.LENGTH_SHORT).show();
            return;
        }
        if (boardstr == null || boardstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_board_toast_board), Toast.LENGTH_SHORT).show();
            return;
        }
        if (widthstr == null || widthstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_board_toast_width), Toast.LENGTH_SHORT).show();
            return;
        }
        if (heightstr == null || heightstr.equals("")) {
            Toast.makeText(this, getResources().getString(R.string.simple_yuv420p_board_toast_height), Toast.LENGTH_SHORT).show();
            return;
        }
        int board = Integer.parseInt(boardstr);
        int width = Integer.parseInt(widthstr);
        int height = Integer.parseInt(heightstr);
        FFmpegHelper.simple_yuv420p_board(srcfile, dstfile, board, width, height);
    }
}
