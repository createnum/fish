package runTime;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import tools.EncryptUtils;

public class LogServlet extends HttpServlet{
    private static final long serialVersionUID = 1L;

    public static final int RESPONSE_FLAG_SUCCESS = 0;// 成功
    public static final int RESPONSE_FLAG_FAILED_VERIFY = 1001;// 合法性验证失败
    public static final int RESPONSE_FLAG_FAILED_PARAMETER = 1002;// 参数异常

    /**
     * Default constructor.
     */
    public LogServlet(){
        // TODO Auto-generated constructor stub
    }

    /**
     * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
     *      response)
     */
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException{
        // TODO Auto-generated method stub
        this.doPost(request, response);
    }

    /**
     * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
     *      response)
     */
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException{
        DataOutputStream dos = new DataOutputStream(response.getOutputStream());
        Map map = request.getParameterMap();
        // 获得通知签名
        long currentTime = System.currentTimeMillis();
        String appId;
        String channelId;
        String version;
        String phoneBrand;
        String phoneType;
        String systemVersion;
        String imsi;
        String key;
        int operatorId;
        String verifyKey;
        List<ActionValue> actionValues;
        try{
            appId = (String) ((Object[]) map.get("appId"))[0];
            channelId = (String) ((Object[]) map.get("channelId"))[0];
            version = (String) ((Object[]) map.get("version"))[0];
            phoneBrand = (String) ((Object[]) map.get("phoneBrand"))[0];
            phoneType = (String) ((Object[]) map.get("phoneType"))[0];
            systemVersion = (String) ((Object[]) map.get("systemVersion"))[0];
            imsi = (String) ((Object[]) map.get("imsi"))[0];
            key = (String) ((Object[]) map.get("key"))[0];
            operatorId = Integer.parseInt((String) ((Object[]) map.get("operatorId"))[0]);
            verifyKey = (String) ((Object[]) map.get("verifyKey"))[0];
            String action = (String) ((Object[]) map.get("action"))[0];// time,action,value,value;time,action,value,value;
            String[] actions = action.split(";");
            actionValues = new ArrayList<LogServlet.ActionValue>();
            for(String a : actions){
                if(a.length() > 0){
                    String[] values = a.split(",");
                    ActionValue actionValue = new ActionValue();
                    actionValue.clientTime = Long.parseLong(values[0]);
                    actionValue.action = values[1];
                    if(values.length > 2){
                        actionValue.value1 = values[2];
                        if(values.length > 3){
                            actionValue.value2 = values[3];
                        }
                    }
                    actionValues.add(actionValue);
                }
            }
        }catch(Exception e){
            dos.writeInt(RESPONSE_FLAG_FAILED_PARAMETER);
            dos.flush();
            return;
        }
        String md5 = EncryptUtils.MD5Encrypt(appId + key + actionValues.get(0).clientTime);
        if(!verifyKey.equals(md5)){// check verifyKey
            dos.writeInt(RESPONSE_FLAG_FAILED_VERIFY);
            dos.flush();
            return;
        }

        String sql = "insert into tnlog(serverTime, appId, channelId, `version`, phoneBrand, phoneType, systemVersion, imsi, `key`, operatorId, clientTime, action, value1, value2)" + " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

        Connection conn = null;
        try{
            conn = DBServiceImpl.getInstance().getConnection();
            for(ActionValue actionValue : actionValues){
                PreparedStatement ps = null;
                try{
                    ps = conn.prepareStatement(sql);
                    int index = 0;
                    ps.setTimestamp(++index, new Timestamp(currentTime));
                    ps.setString(++index, appId);
                    ps.setString(++index, channelId);
                    ps.setString(++index, version);
                    ps.setString(++index, phoneBrand);
                    ps.setString(++index, phoneType);
                    ps.setString(++index, systemVersion);
                    ps.setString(++index, imsi);
                    ps.setString(++index, key);
                    ps.setInt(++index, operatorId);
                    ps.setTimestamp(++index, new Timestamp(actionValue.clientTime * 1000));
                    ps.setString(++index, actionValue.action);
                    ps.setString(++index, actionValue.value1);
                    ps.setString(++index, actionValue.value2);
                    ps.execute();
                }finally{
                    if(null != ps){
                        try{
                            ps.close();
                        }catch(SQLException e){
                            e.printStackTrace();
                        }
                    }
                }
            }
        }catch(SQLException e){
            e.printStackTrace();
        }finally{
            dos.writeInt(RESPONSE_FLAG_SUCCESS);
            dos.flush();
            if(null != conn){
                try{
                    conn.close();
                }catch(SQLException e){
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }
    }

    static class ActionValue{
        long clientTime;
        String action;
        String value1 = "";
        String value2 = "";
    }
}
