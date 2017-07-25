package channel;

import java.io.UnsupportedEncodingException;

import skt.board.fish.Fish;

public class Pay {
    public static int curPayRmb;
    
    public static native void paySuccess(int rmb, String tradeID);
    public static native void payFail(int rmb, String reason);
    
    public static void pay(final int rmb) throws UnsupportedEncodingException{
        curPayRmb = rmb;
        Fish.myMainActivity.channel.pay(rmb);
    }
	public static void sendThirdPartyTjLog(String tag, String str1, String str2, String str3){
        Fish.myMainActivity.umeng.onEvent(Fish.myMainActivity, tag, str1, str2, str3);
     }
}
