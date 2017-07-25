package skt.board.fish;

import skt.board.fish.lightning.R;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

public class Welcome extends Activity {
	
	public static Welcome context;
	boolean isStop=true;
	private Handler handler;
	private Runnable runnable;
	private ImageView imageView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);// 去掉标题栏
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);// 设置全屏
        setContentView(R.layout.zcwelcome);  
        context = this;
        new Handler().postDelayed(new Runnable() {
			public void run() {
				
				Intent intent = new Intent();
				intent.setClass(Welcome.this, Welcome1.class);
				startActivity(intent);
				finish();
			}
		}, 2000);
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		super.onDestroy();
	}
}
