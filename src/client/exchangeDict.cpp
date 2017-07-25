#include "exchangeDict.h"
#include "CommonFunc.h"
#include "support/tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"

#define EXCHANGE_XML_PATH      "res/exchange.xml"

using namespace std;

ExchangeDict* ExchangeDict::m_dict = NULL;

int ExchangeDict::init(void)
{
    m_dict = this;
    return 0;
}
ExchangeDict::ExchangeDict(void)
{
}

ExchangeDict::~ExchangeDict(void)
{
}

int ExchangeDict::loadInfos()
{
    tinyxml2::XMLDocument doc;
    if(!CommonFunc::loadInnerDictFile(doc, EXCHANGE_XML_PATH)){
        return 1;
    }
    tinyxml2::XMLElement * storeNode = doc.RootElement()->FirstChildElement("store");
    while (storeNode)
    {
        StoreInfo *storeInfo = new StoreInfo();
        tinyxml2::XMLElement *curNode = storeNode->FirstChildElement("data");
        while (curNode)
        {
            ExchangeInfo *info = new ExchangeInfo();
            curNode->QueryIntAttribute("gold",&info->gold);
            curNode->QueryIntAttribute("rmb",&info->rmb);
            curNode->QueryIntAttribute("presentGold",&info->presentGold);
            curNode->QueryBoolAttribute("showInStore", &info->showInStore);
            curNode->QueryBoolAttribute("gift",&info->gift);
            curNode->QueryBoolAttribute("miniGift",&info->miniGift);
            curNode = curNode->NextSiblingElement("data");
            storeInfo->exchangeInfos.push_back(info);
        }
        storeNode = storeNode->NextSiblingElement("store");
        this->storeInfos.push_back(storeInfo);
    }
    return 0;
}

const StoreInfo* ExchangeDict::getStoreInfo(int storeId) const{
    return this->storeInfos[storeId];
}

void StoreInfo::getExchangeInfos(std::vector<ExchangeInfo*>& infos) const
{
    for (vector<ExchangeInfo*>::const_iterator iter = exchangeInfos.begin(); iter != exchangeInfos.end(); iter++)
    {
        ExchangeInfo *info = *iter;
        if (info->showInStore)
        {
            infos.push_back(info);
        }
    }
}

const ExchangeInfo*  StoreInfo::getMiniGiftInfo() const
{
    for (vector<ExchangeInfo*>::const_iterator iter = exchangeInfos.begin(); iter != exchangeInfos.end(); iter++)
    {
        ExchangeInfo *info = *iter;
        if (info->miniGift)
        {
            return info;
        }
    }
    return NULL;
}

const ExchangeInfo* StoreInfo::getGiftInfo() const{
    for (vector<ExchangeInfo*>::const_iterator iter = exchangeInfos.begin(); iter != exchangeInfos.end(); iter++)
    {
        ExchangeInfo *info = *iter;
        if (info->gift)
        {
            return info;
        }
    }
    return NULL;
}

const ExchangeInfo* StoreInfo::getExchangeInfo(int rmb) const
{
    for(unsigned int i=0;i<exchangeInfos.size();i++){
        if (rmb == exchangeInfos.at(i)->rmb)
        {
            return exchangeInfos.at(i);
        }
    }
    return NULL;
}