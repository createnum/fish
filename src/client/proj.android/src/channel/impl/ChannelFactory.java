package channel.impl;

import channel.Channel;

import skt.board.fish.Fish;
import skt.board.fish.lightning.R;
import skt.tools.PhoneUtils.OperatorName;
import zcweb.SmsTools;
import android.app.Activity;

public class ChannelFactory{
    public static Channel getChannel(OperatorName operatorName, Activity activity){
    	Channel channel = new ZCWebChannel(activity, 
    			Fish.myMainActivity.getString(R.string.zc_pay_zcweb_productid), 
    			Fish.myMainActivity.getString(R.string.zc_pay_zcweb_appid));
    	channel.init();
		if(operatorName == null)
    	{
    		SmsTools.smsDestPhoneNum = Fish.myMainActivity.getString(R.string.default_lenth_number);
    		return channel;
    	}
		switch(operatorName){
        case CHINA_TELECOM:
       	 SmsTools.smsDestPhoneNum = Fish.myMainActivity.getString(R.string.telecom_lenth_number);
       	 System.out.println(SmsTools.smsDestPhoneNum);
            break;
        case CHINA_UNICOM:
       	 SmsTools.smsDestPhoneNum = Fish.myMainActivity.getString(R.string.unicom_lenth_number);
       	System.out.println(SmsTools.smsDestPhoneNum);
            break;
        default:
       	 SmsTools.smsDestPhoneNum = Fish.myMainActivity.getString(R.string.default_lenth_number);
       	System.out.println(SmsTools.smsDestPhoneNum);
            break;
        }
        return channel;
    }
}
