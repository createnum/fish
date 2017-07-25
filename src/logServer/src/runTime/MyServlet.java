package runTime;

import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class MyServlet extends HttpServlet{
    private static final long serialVersionUID = 1L;

    /**
     * Default constructor.
     */
    public MyServlet(){
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
        Map map = request.getParameterMap();
        // 获得通知签名
        long st = System.currentTimeMillis();
        String ct = (String) ((Object[])map.get("ct"))[0];
        String fbtime = (String) ((Object[])map.get("fbt"))[0];
        String btime = (String) ((Object[])map.get("bt"))[0];
        String version = (String) ((Object[])map.get("ver"))[0];
        String channel = (String) ((Object[])map.get("ch"))[0];
        String phoneName = (String) ((Object[])map.get("pt"))[0];
        String aVersion = (String) ((Object[])map.get("aVer"))[0];
        String key = (String) ((Object[])map.get("key"))[0];
        String imsi = (String) ((Object[])map.get("imsi"))[0];
        String tag = (String) ((Object[])map.get("tag"))[0];
        Object on = map.get("operatorName");
        String operatorName = "";
        if(null != on){
            operatorName = (String) ((Object[])on)[0];
        }

        String sql = "insert into tlog(serverTime, clientTime, firstTime, beginTime, `version`, channel, phoneType, androidVersion, `key`, imsi, tag, operatorName)" 
            + " VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
        Connection conn = null;
        PreparedStatement ps = null;
        try{
            conn = DBServiceImpl.getInstance().getConnection();
            ps = conn.prepareStatement(sql);
            ps.setTimestamp(1, new Timestamp(st));
            ps.setTimestamp(2, new Timestamp(Long.parseLong(ct)));
            ps.setTimestamp(3, new Timestamp(Long.parseLong(fbtime)));
            ps.setTimestamp(4, new Timestamp(Long.parseLong(btime)));
            ps.setString(5, version);
            ps.setString(6, channel);
            ps.setString(7, phoneName);
            ps.setString(8, aVersion);
            ps.setString(9, key);
            ps.setString(10, imsi);
            ps.setString(11, tag);
            ps.setString(12, operatorName);
            ps.execute();
        }catch(SQLException e){
            // TODO Auto-generated catch block
            e.printStackTrace();
        }finally{
            if(null != ps){
                try{
                    ps.close();
                }catch(SQLException e){
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
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

}
