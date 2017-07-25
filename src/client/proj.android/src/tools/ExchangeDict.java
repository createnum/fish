package tools;

import java.util.*;

import org.dom4j.*;

import tools.ExchangeDict.ExchangeInfo;

public class ExchangeDict{
    public static final String EXCHANGE_CONFIG_XML_PATH = "res/exchange.xml";

    protected static ExchangeDict instance;
    protected List<StoreInfo> storeInfos = new ArrayList<StoreInfo>();

    protected ExchangeDict(){
    }

    public static ExchangeDict getInstance(){
        if(null == instance){
            Element rootNode = CommonFunc.loadDictFile(EXCHANGE_CONFIG_XML_PATH);
            ExchangeDict inst = new ExchangeDict();
            inst.loadInfos(rootNode);
            instance = inst;
        }
        return instance;
    }

    protected void loadInfos(Element rootNode){
        storeInfos.clear();
        try{
            Iterator<Element> iter = rootNode.elementIterator();
            while(iter.hasNext()){
                Element storeNode = iter.next();
                StoreInfo storeInfo = new StoreInfo();
                Iterator<Element> storeIter = storeNode.elementIterator();
                while(storeIter.hasNext()){
                    Element currentNode = storeIter.next();
                    ExchangeInfo info = new ExchangeInfo();
                    info.gold = Integer.parseInt(currentNode.attributeValue("gold"));
                    info.presentGold = Integer.parseInt(currentNode.attributeValue("presentGold"));
                    info.rmb = Integer.parseInt(currentNode.attributeValue("rmb"));
                    info.payCode = currentNode.attributeValue("payCode");
                    info.productid = currentNode.attributeValue("productid");
					info.payname = currentNode.attributeValue("payname");
                    storeInfo.exchangeInfos.add(info);
                }
                this.storeInfos.add(storeInfo);
            }
        }catch(Exception e){
            e.printStackTrace();
            System.exit(-1);
        }
    }

    public StoreInfo getStoreInfo(int channelIndex){
        return this.storeInfos.get(channelIndex);
    }

    public static class ExchangeInfo{
        public int gold;
        public int presentGold;
        public int rmb;
        public String payCode;
        public String productid;
		public String payname;
        public String getShowName(){
            return "" + (gold + presentGold) + "½ð±Ò";
        }
    }

    public static class StoreInfo{

        public ExchangeInfo getByRmb(int rmb){
            for(ExchangeInfo info : this.exchangeInfos){
                if(rmb == info.rmb){
                    return info;
                }
            }
            return null;
        }

        public ExchangeInfo getByPayCode(String payCode){
            for(ExchangeInfo info : this.exchangeInfos){
                if(payCode.equals(info.payCode)){
                    return info;
                }
            }
            return null;
        }

        public ExchangeInfo getByrmb(int rmb){
            for(ExchangeInfo info : this.exchangeInfos){
                if(rmb == info.rmb){
                    return info;
                }
            }
            return null;
        }

        protected List<ExchangeInfo> exchangeInfos = new ArrayList<ExchangeInfo>();
    }
}
