#ifndef __STORE_SCENE_H__
#define __STORE_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
/*#include "exchangeInfo.h"*/

class StoreScene:public cocos2d::gui::TouchGroup
{
public:
    static cocos2d::CCScene* scene();
    CREATE_FUNC(StoreScene);
    ~StoreScene(void);
    virtual bool init();
    static unsigned int confirmPayment(int rmb);
    void touchedCallBack(CCObject*,cocos2d::gui::TouchEventType);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    cocos2d::gui::Widget * m_widget;
    static void androidPay(int rmb);
    static void decidePayWays(int rmb);
    static void updateMyGoldShow(){if (m_inst) m_inst->_updateMyGoldShow();}

protected:
    static StoreScene *m_inst;
    void _updateMyGoldShow();
};

class GiftStoreScene:public cocos2d::gui::TouchGroup
{
public:
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GiftStoreScene);
    ~GiftStoreScene(void);
    virtual bool init();
    void touchedCallBack(CCObject*,cocos2d::gui::TouchEventType);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
};

class MiniGiftStoreScene:public cocos2d::gui::TouchGroup
{
public:
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MiniGiftStoreScene);
    ~MiniGiftStoreScene(void);
    virtual bool init();
    void touchedCallBack(CCObject*,cocos2d::gui::TouchEventType);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
};

class PaymentStoreScene:public cocos2d::gui::TouchGroup
{
public:
    static cocos2d::CCNode* create(int rmb);
    ~PaymentStoreScene(void){}
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void touchedCallBack(CCObject*,cocos2d::gui::TouchEventType);
    virtual bool init(int rmb);
    int m_rmb;

};
#endif //__STORE_SCENE_H__

