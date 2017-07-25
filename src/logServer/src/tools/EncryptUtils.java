package tools;

import java.math.BigInteger;
import java.security.MessageDigest;

/**
 * 加密工具类
 * @description 
 * @author Fang Yucun
 * @created 2013年10月31日
 */
public class EncryptUtils {

    private EncryptUtils() {};
    
    public static String SHA1Encrypt(String input) {
        MessageDigest md = null;
        try {
            md = MessageDigest.getInstance("sha1");
            md.update(input.getBytes());
            String pwd = new BigInteger(1, md.digest()).toString(16);
            return pwd;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return input;
    }

    public static String MD5Encrypt(String input) {
        MessageDigest md = null;
        try {
            md = MessageDigest.getInstance("MD5");
            md.update(new String(input.getBytes("gbk"), "UTF-8").getBytes("UTF-8"));
            byte[] bytes = md.digest();
            int i;
            StringBuffer buffer = new StringBuffer("");
            for (int offset = 0; offset < bytes.length; offset++) {
                i = bytes[offset];
                if (i < 0) {
                    i += 256;
                }
                if (i < 16) {
                    buffer.append("0");
                }
                buffer.append(Integer.toHexString(i));
            }
            return buffer.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return input;
    }
    
    
}
