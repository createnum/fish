package channel.impl;

import channel.Channel;
import channel.Pay;
import skt.board.fish.Fish;
import skt.board.fish.lightning.R;
import tools.ExchangeDict;
import tools.ExchangeDict.ExchangeInfo;
import zcweb.ZCPayIcallback;
import zcweb.ZCWeb;
import android.app.Activity;
import android.widget.Toast;
import android.view.View;
import android.view.View.OnClickListener;

public class ZCWebChannel extends Channel {
	protected String productid;
	protected String appid;
	protected long lastPayTime;
    public ZCWebChannel(Activity activity, String productid, String appid){
         super(activity, 0);
         this.productid = productid;
         this.appid = appid;
    }
    
    @Override
     public void init(){
    }


      @Override
    public void pay(int rmb){
    	
    	long curTime = System.currentTimeMillis();
    	/*if(curTime - lastPayTime < 60000){
    		context.runOnUiThread(new Runnable(){
                @Override
                public void run(){
                	Toast.makeText(context, "支付太频繁，请一分钟后再支付！", Toast.LENGTH_LONG).show();
                }
    		});
    		return;
    	}*/
    	lastPayTime = curTime;
    	final ExchangeInfo exchangeInfo = ExchangeDict.getInstance().getStoreInfo(channelIndex).getByrmb(rmb);
        Pay.curPayRmb = exchangeInfo.rmb;
        context.runOnUiThread(new Runnable(){
        	public void run() {
        		/*GameInterface.doBilling(context, 4, exchangeInfo.payCode,
						"0123456789012345", ZCWebChannel.this);*/
        		 Pay.paySuccess(Pay.curPayRmb, "");
         		 Pay.sendThirdPartyTjLog("paySuccess", ""+Pay.curPayRmb, "0", "24");//24 :赞成web联通  友盟统计中的支付渠道
         		 Toast.makeText(context, "支付成功", Toast.LENGTH_SHORT).show();
        	}
        });
        /*if(!Fish.myMainActivity.getString(R.string.visibleCommodityInfo).equals("true")){
			Pay.sendThirdPartyTjLog("pay", ""+exchangeInfo.rmb, ""+(exchangeInfo.gold+exchangeInfo.presentGold), "23");//23 :电信爱动漫   友盟统计中的支付渠道
			continuePay(exchangeInfo.rmb);
			Toast.makeText(context, "发起请求计费", Toast.LENGTH_SHORT).show();
			return;
		}
        context.runOnUiThread(new Runnable(){
            @Override
            public void run(){
				final PayConfirmDialog dialog = new PayConfirmDialog(Fish.myMainActivity);
			   		dialog.init(exchangeInfo.payname, "" + exchangeInfo.rmb*0.01, "010-84543920");
			   		dialog.setCloseListener(new OnClickListener() {
						@Override
						public void onClick(View v) {
							Pay.payFail(Pay.curPayRmb, "取消支付");
							Pay.sendThirdPartyTjLog("payFail", ""+exchangeInfo.rmb, "1", "23");//23 :电信爱动漫   友盟统计中的支付渠道
							dialog.dismiss();
						}
				    });
				    dialog.setConfirmListener(new OnClickListener() {
				        @Override
				        public void onClick(View v) {
				        	continuePay(exchangeInfo.rmb);
							Pay.sendThirdPartyTjLog("pay", ""+exchangeInfo.rmb, ""+(exchangeInfo.gold+exchangeInfo.presentGold), "23");//23 :电信爱动漫   友盟统计中的支付渠道							
				        	dialog.dismiss();
				        }
				    });
				   dialog.show();
				
            }
        });*/
        
    }
    
    public void continuePay(int rmb)
    {
    	final ExchangeInfo exchangeInfo = ExchangeDict.getInstance().getStoreInfo(channelIndex).getByrmb(rmb);
    	 context.runOnUiThread(new Runnable(){
             @Override
             public void run(){
 				try {
 					Pay.sendThirdPartyTjLog("pay", ""+exchangeInfo.rmb, ""+(exchangeInfo.gold+exchangeInfo.presentGold), "24");//24 :赞成web联通  友盟统计中的支付渠道
 					ZCWeb.getInst().pay(Pay.curPayRmb, new ZCPayIcallback(){  
 			            public void payResult(String resultCode, String orderId) {  
 			                if(resultCode.equals("200000")){
 				            		 Pay.paySuccess(Pay.curPayRmb, orderId);
 				            		 Pay.sendThirdPartyTjLog("paySuccess", ""+Pay.curPayRmb, "0", "24");//24 :赞成web联通  友盟统计中的支付渠道
 				            		 Toast.makeText(context, "支付成功", Toast.LENGTH_SHORT).show();
 			            		}else{
 			            			 Pay.payFail(Pay.curPayRmb, resultCode);
 			            			 Pay.sendThirdPartyTjLog("payFail", ""+Pay.curPayRmb, resultCode, "24");//24 :赞成web联通   友盟统计中的支付渠道
 			            			 Toast.makeText(context, "支付失败" + resultCode, Toast.LENGTH_SHORT).show();
 			            		}
 								Pay.curPayRmb = 0;
 			            }  
 			        });
 				} catch (Exception e) {
 					// TODO Auto-generated catch block
 					e.printStackTrace();
 				}
             }
         });
    	
    }


    @Override
    public void onResume(){
    }

    @Override
    public void onDestroy(){
        // TODO Auto-generated method stub

    }

    @Override
    public void onPause(){
    }

    

}
