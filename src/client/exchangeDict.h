#ifndef __EXCHANGE_DICT_H__
#define __EXCHANGE_DICT_H__
#include <vector>
#include <map>
#include "myGame.h"
#include "support/tinyxml2/tinyxml2.h"

class ExchangeInfo
{
public:
    ExchangeInfo():gold(0),presentGold(0),rmb(0),showInStore(false),miniGift(false),gift(false){
    }
    int gold;
    int presentGold;
    int rmb;
    bool showInStore;
    bool miniGift;
    bool gift;
};

class StoreInfo{
public:
    void getExchangeInfos(std::vector<ExchangeInfo*>& infos) const;
    const ExchangeInfo* getMiniGiftInfo() const;
    const ExchangeInfo* getGiftInfo() const;
    const ExchangeInfo* getExchangeInfo(int rmb) const;
public:
    std::vector<ExchangeInfo*> exchangeInfos ;
};

class ExchangeDict
{
public:
    static ExchangeDict* getInstance()
    {
        if (!m_dict)
        {
            ExchangeDict *dict = new ExchangeDict();
            dict->loadInfos();
            m_dict = dict;
        }
        return m_dict;
    }
    int init();
    ExchangeDict(void);
    ~ExchangeDict(void);
    int loadInfos();
    const StoreInfo* getStoreInfo(int storeId) const;
protected:
    static ExchangeDict* m_dict;
    std::vector<StoreInfo*> storeInfos ;
};
#endif //__EXCHANGE_DICT_H__

