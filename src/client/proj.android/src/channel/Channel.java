package channel;

import java.io.ByteArrayInputStream;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import skt.board.fish.Fish;
import skt.tools.StatisticsLog;
import tools.CommonFunc;
import tools.ExchangeDict;
import tools.ExchangeDict.ExchangeInfo;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.widget.Toast;

public abstract class Channel{

    private static final String MM_SON_CHANNEL_FILE = "mmiap.xml";
    private static String channelID = null;

    public static String getSonChannelID(Context activity){
        if(null == channelID){
            try{
                String channleStr = CommonFunc.getResFileContent(MM_SON_CHANNEL_FILE, activity);
                XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
                XmlPullParser parser = factory.newPullParser();
                ByteArrayInputStream bais = new ByteArrayInputStream(channleStr.getBytes());
                parser.setInput(bais, "utf-8");
                int event = parser.getEventType();
                while(event != XmlPullParser.END_DOCUMENT){
                    switch(event){
                    case XmlPullParser.START_DOCUMENT:
                        break;
                    case XmlPullParser.START_TAG:
                        String tag = parser.getName();
                        if("channel".equals(tag)){
                            channelID = parser.nextText();
                        }
                        break;
                    case XmlPullParser.END_TAG:
                        break;
                    }
                    event = parser.next();
                }
            }catch(Exception e){
                channelID = "";
            }
        }
        return channelID;
    }

    public Channel(Activity activity, int channelIndex){
        this.context = activity;
        this.channelIndex = channelIndex;
    }

    public abstract void init();

    public abstract void pay(int rmb);

    public abstract void onResume();

    public abstract void onDestroy();

    public abstract void onPause();

    public void exitGame(){
        try{
            StatisticsLog.sendLog("exit", "", "");
            context.finish();
        }catch(Exception e){
            e.printStackTrace();
        }finally{
            Umeng.onKillProcess(context);
            android.os.Process.killProcess(android.os.Process.myPid());
            // System.exit(0);
        }
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data){
    }

    public String getAppId(){
        return "";
    }

    public String getKey(){
        return "";
    }

    public int getChannelIndex(){
        return channelIndex;
    }

    public void setMoreGameInfo(String path, String normalPng, String clickedPng){
        haveMoreGame = true;
        moreGameUrlPath = path;
        moreGameNormalPng = normalPng;
        moreGameClieckedPng = clickedPng;
    }

    public void moreGame(){
        if(haveMoreGame){
            Uri moreGame = Uri.parse(moreGameUrlPath);
            this.context.startActivity(new Intent(Intent.ACTION_VIEW, moreGame));
        }
    }

    public String getMoreGameNormalPng(){
        return haveMoreGame ? moreGameNormalPng : "";
    }

    public String getMoreGameClickedPng(){
        return haveMoreGame ? moreGameClieckedPng : "";
    }

    protected Activity context;
    protected int channelIndex;
    public boolean haveMoreGame = false;
    public String moreGameUrlPath;
    public String moreGameNormalPng;
    public String moreGameClieckedPng;

    public static class EmptyChannel extends Channel{

        public EmptyChannel(Activity activity){
            super(activity, 0);
        }

        @Override
        public void init(){
        }

        @Override
        public void pay(int rmb){
        }

        @Override
        public void onResume(){
        }

        @Override
        public void onDestroy(){
        }

        @Override
        public void onPause(){
        }
    }

    public static class TestChannel extends Channel{

        public TestChannel(Activity activity){
            super(activity, 0);
        }

        @Override
        public void init(){
        }

        @Override
        public void pay(final int rmb){
            context.runOnUiThread(new Runnable(){
                @Override
                public void run(){
                    ExchangeInfo exchangeInfo = ExchangeDict.getInstance().getStoreInfo(channelIndex).getByRmb(rmb);
                    Toast.makeText(context, "道具" + (exchangeInfo.presentGold + exchangeInfo.gold) + "金币" + "购买成功", Toast.LENGTH_LONG).show();
                    Pay.paySuccess(rmb, "");
                }
            });
        }

        @Override
        public void onResume(){
        }

        @Override
        public void onDestroy(){
        }

        @Override
        public void onPause(){
        }
    }

}
