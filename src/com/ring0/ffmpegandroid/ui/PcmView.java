package com.ring0.ffmpegandroid.ui;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

public class PcmView extends View {
    private Paint mPaint;
    private byte[] pcm;
    
    public PcmView(Context context) {
        super(context);
        init();
    }
    public PcmView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }
    public PcmView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }
    
    public void update(byte[] pcm) {
        this.pcm = pcm;
        invalidate();
    }
    
    private void init() {
        mPaint = new Paint();
        mPaint.setAntiAlias(true);
        mPaint.setColor(Color.BLUE);
        mPaint.setStrokeWidth(2);
    }
    
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawColor(Color.BLACK);
        
        if (pcm != null && pcm.length > 0) {
            int width  = getWidth();
            int height = getHeight();
            
            int size = pcm.length;
            float points[] = new float[size * 4];
            
            for (int i = 0; i < size; i++) {
                int value = pcm[i] + 128;
                if (value == 0) value = 128;
                points[i * 4 + 0] = width * i / size;
                points[i * 4 + 2] = width * i / size;
                points[i * 4 + 1] = ((float)value / 256) * height;
                points[i * 4 + 3] = height - points[i * 4 + 1];
            }
            canvas.drawLines(points, mPaint);
        }
    }
}
