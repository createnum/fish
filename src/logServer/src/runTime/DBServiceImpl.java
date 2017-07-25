package runTime;

import java.sql.*;
import java.util.Properties;

import org.logicalcobwebs.proxool.ProxoolFacade;

public class DBServiceImpl {
    private static DBServiceImpl instance = null;
    String dbPoolName;
    String proxpoolName;
    private DBServiceImpl()
    {
    }

    public static DBServiceImpl getInstance()
    {
        if (instance == null)
        {
            instance = new DBServiceImpl();
            instance.init();
        }
        return instance;
    }
    
    void init(){
        try{
            String prox_driver = "org.logicalcobwebs.proxool.ProxoolDriver";
            String proxool_name="proxool";
            String pool_name="HERO";
            String db_driver="com.mysql.jdbc.Driver";
            
            String url, user_name, password;
            boolean testServer = true;
            if(!testServer){
                url = "jdbc:mysql://127.0.0.1:10002/fishlog?connectTimeout=0&amp;autoReconnect=true&amp;failOverReadOnly=false&amp;noAccessToProcedureBodies=true&amp;useUnicode=true&amp;characterEncoding=UTF-8";
                user_name = "db_user_zhanxia1";
                password = "1S(o~A(a*Ip5";
            }else{
                url = "jdbc:mysql://127.0.0.1:3306/fishlog?connectTimeout=0&amp;autoReconnect=true&amp;failOverReadOnly=false&amp;noAccessToProcedureBodies=true&amp;useUnicode=true&amp;characterEncoding=UTF-8";
                user_name = "zhanxia_gm";
                password = "skt_123";
            }
            
            String maximum_connection_count="80";
            String minimum_connection_count="10";
            String simultaneous_build_throttle="5";
            String maximum_active_time="300000";
            String house_keeping_test_sql="select CURRENT_DATE";
            String house_keeping_sleep_time="90000";
            String prototype_count="5";
            
            String proxDriverName = prox_driver;
            String dbDriverName = db_driver;

                Class.forName(proxDriverName);
            Class.forName(dbDriverName);

            dbPoolName = pool_name;
            proxpoolName = proxool_name + "." + dbPoolName;

            String URL = proxpoolName + ":" + proxDriverName + ":"+ url;
            System.out.println("URL:"+URL);
            Properties info = new Properties();

            info.setProperty("user", user_name);
            info.setProperty("password", password);
            info.setProperty("proxool.maximum-connection-count", maximum_connection_count);
            info.setProperty("proxool.minimum-connection-count", minimum_connection_count);
            info.setProperty("proxool.maximum-active-time", maximum_active_time);
            info.setProperty("proxool.simultaneous-build-throttle", simultaneous_build_throttle);
            info.setProperty("proxool.house-keeping-test-sql", house_keeping_test_sql);
            info.setProperty("proxool.house-keeping-sleep-time", house_keeping_sleep_time);
            info.setProperty("proxool.prototype-count", prototype_count);
            ProxoolFacade.registerConnectionPool(URL, info);
        }catch(Exception e){
            e.printStackTrace();
        }    
    }

    public final Connection getConnection() {
        //YOYOStackTrace.displayStackTraceInformation("database info:");
        long getConStart = System.currentTimeMillis();
        Connection con = null;
        try{
            con = DriverManager.getConnection(proxpoolName);
            //System.out.println("获取游戏数据库连接："+DBServiceImpl.connCount);
        }catch(Exception ex){
            StringBuffer sb = new StringBuffer();
            sb.append("-连接数据库异常！");
            System.out.println(sb.toString());
            ex.printStackTrace();
        }
        
        long getConElpased = System.currentTimeMillis() - getConStart;
        if (getConElpased > 0){
            StringBuffer sb = new StringBuffer();
            sb.append("-连接数据库耗时 TimeElapsed:").append(getConElpased);
            System.out.println(sb.toString());
        }
        
        return con;
    }
}
