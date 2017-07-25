/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package skt.board.fish;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.Toast;
import channel.Channel;
import channel.Testin;
import channel.Umeng;
import channel.impl.ChannelFactory;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import skt.tools.PhoneUtils;
import skt.tools.PhoneUtils.OperatorName;
import skt.tools.StatisticsLog;

public class Fish extends Cocos2dxActivity{

     //static final String RECIVE_LOG_URL = "http://192.168.254.76:8080/gm/newLog";
    static final String RECIVE_LOG_URL = "http://42.96.170.133:10017/fishLog/newLog";
    public static Umeng umeng;
    public static Fish myMainActivity = null; // Ö÷activity
    public Channel channel;

    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
        myMainActivity = this;
        OperatorName curOperator = PhoneUtils.getOperatorName(this);
        String channelId = Channel.getSonChannelID(this);
		StatisticsLog.init(this, RECIVE_LOG_URL, channelId);
		//StatisticsLog.getInstance().sendSelfLog("begin");
        Umeng.onCreate(this, channelId);
        Testin.setChannelId(channelId);
        //sendLog("afUM");
        channel = ChannelFactory.getChannel(curOperator, this);
        //sendLog("created");
    	
		String umChannelId = "";
		try {
			 ApplicationInfo appInfo = this.getPackageManager()
			        .getApplicationInfo(getPackageName(),PackageManager.GET_META_DATA);
			umChannelId = appInfo.metaData.getString("com.snowfish.channel");
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		umeng = new Umeng();
		umeng.onCreate(myMainActivity, umChannelId);
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// TexasPoker should create stencil buffer
    	//gLSurfaceView.setEGLConfigChooser(8 , 8, 8, 8, 16, 0);
    	//holder.setFormat(PixelFormat.RGB_8888);
    	//holder.setFormat(PixelFormat.RGB_565);
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
  
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        channel.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onResume(){
        super.onResume();
        channel.onResume();
        Umeng.onResume(this);
    } 
    
    @Override
    public void onPause() { 
        super.onPause();
        channel.onPause();
        Umeng.onPause(this);
    }
    
    @Override
    protected void onDestroy(){
        super.onDestroy();
        channel.onDestroy();
    }
    
    /*public static void exitGame(){
        myMainActivity.channel.exitGame();
    }*/
            
    public static int getStoreId(){
        return myMainActivity.channel.getChannelIndex();
    }

    public static void moreGame(){
        myMainActivity.channel.moreGame();
    }
    
    public static void adMoreGame(){
        Uri moreGame = Uri.parse("http://42.96.170.133:10017/fishLog/xmnzq_0109_r_yz_1mm_v1.0_zc_zqm_300008758053_2200136427.apk");
        myMainActivity.startActivity(new Intent(Intent.ACTION_VIEW, moreGame));
    }
    
    public static String getMoreGameNormalPng(){
        return myMainActivity.channel.getMoreGameNormalPng();
    }
    
    public static String getMoreGameClickedPng(){
        return myMainActivity.channel.getMoreGameClickedPng();
    }
    
//    public static String getTJNeedInfo() {
//        String tjNeedInfo = "";
//        String id = CommonFunc.getDeviceId(myMainActivity);
//        String operatorSonChannelId = Channel.getSonChannelID(myMainActivity);
//        long time = System.currentTimeMillis();
//        tjNeedInfo += "&uId=" + id +"&appId=" + myMainActivity.channel.getAppId() + "&pId=" + PhoneUtils.getNetworkOperator(myMainActivity)
//        + "&sonChannelId="+operatorSonChannelId + "&IMEI=" +PhoneUtils.getIMEI(myMainActivity) + "&time="+time
//        + "&key="+EncryptUtils.MD5Encrypt(myMainActivity.channel.getKey()+time) + "&version=" + GlobalDict.getInstance().version;
//        return tjNeedInfo;
//    }
    
    

	
}
