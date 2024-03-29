package com.ring0.ffmpegandroid;


import java.util.Vector;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class SimplePcmActivity extends Activity implements View.OnClickListener {
    private ListView lvList = null;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.simple_pcm);
        
        lvList = (ListView)findViewById(R.id.lvList);
        lvList.setAdapter(new MyListAdapter(this.getApplicationContext()));
    }

    private class MyListAdapter extends BaseAdapter {
        private Context context = null;
        private Vector<String> listVector = null;
        private LayoutInflater inflater = null;
        
        public MyListAdapter(Context context) {
            this.context = context;
            this.inflater = (LayoutInflater)this.context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            this.listVector = new Vector<String>();
            
            String arrays[] = this.context.getResources().getStringArray(R.array.simple_pcm_list);
            if (arrays != null && arrays.length > 0) {
                int size = arrays.length;
                for (int i = 0; i < size; i++) {
                    this.listVector.add(arrays[i]);
                }
            }
        }
        
        @Override
        public int getCount() {
            int count = 0;
            if (this.listVector != null && this.listVector.size() > 0) {
                count = this.listVector.size();
            }
            return count;
        }

        @Override
        public Object getItem(int arg0) {
            return null;
        }

        @Override
        public long getItemId(int arg0) {
            return 0;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup group) {
            ViewHolder holder = null;
            if (convertView != null && convertView.getTag() != null && convertView.getTag().getClass() == ViewHolder.class) {
                holder = (ViewHolder)convertView.getTag();
            }
            else {
                holder = new ViewHolder();
                convertView = inflater.inflate(R.layout.list_item, null, false);
                convertView.setTag(holder);
                holder.llItem = convertView;
                holder.tvItem = (TextView)holder.llItem.findViewById(R.id.tvItem);
                holder.tvItem.setOnClickListener(SimplePcmActivity.this);
            }
            
            prepareItem(position, this.listVector, holder);
            return convertView;
        }
        
        public void prepareItem(int position, Vector<String> list, ViewHolder holder) {
            holder.tvItem.setText(list.get(position));
            holder.tvItem.setTag(R.id.tvItem, Integer.valueOf(position));
        }
    }
    
    private class ViewHolder {
        public View llItem;
        public TextView tvItem;
    }

    @Override
    public void onClick(View item) {
        int tag = (Integer)item.getTag(R.id.tvItem);
        Intent intent = null;
        switch (tag) {
        case 0: // pcm16le 分离为左右声道
            intent = new Intent(this, SimplePcm16leSplitActivity.class);
            startActivity(intent);
            break;
        case 1: // pcm16le 左声道音量降低一半
            intent = new Intent(this, SimplePcm16leLefthalfActivity.class);
            startActivity(intent);
            break;
        case 2: // pcm16le 声音速度提高一倍
            intent = new Intent(this, SimplePcm16leDoublespeedActivity.class);
            startActivity(intent);
            break;
        case 3: // pcm16le 转换为 pcm8le
            intent = new Intent(this, SimplePcm16lePcm8leActivity.class);
            startActivity(intent);
            break;
        case 4: // pcm16le 截取部分采样
            intent = new Intent(this, SimplePcm16leCorpActivity.class);
            startActivity(intent);
            break;
        case 5: // pcm16le 保存为 wav
            intent = new Intent(this, SimplePcm16leWavActivity.class);
            startActivity(intent);
            break;
        }
    }
}
