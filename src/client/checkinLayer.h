#ifndef __CHECKIN_LAYER_H__
#define __CHECKIN_LAYER_H__
#include "cocos2d.h"
#include "cocos-ext.h"

class CheckinLayer:public cocos2d::gui::TouchGroup
{
public:
    static CheckinLayer* create(int dayCount, int maxDayCount, int gold);
    ~CheckinLayer(void);
    virtual bool init(int dayCount, int maxDayCount, int gold);
    void touchedCallBack(cocos2d::CCObject* pSender, cocos2d::gui::TouchEventType pType);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
};

#endif //__CHECKIN_LAYER_H__

