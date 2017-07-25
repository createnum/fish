#ifndef __TOUCH_LAYER_H__
#define __TOUCH_LAYER_H__

#include "cocos2d.h"
#include "fishDelegates.h"

class TouchLayer:public cocos2d::CCLayer
{
public:  
    CREATE_FUNC(TouchLayer);
    CC_SYNTHESIZE(TouchLayerDelegate*,m_pDelegate,Delegate);  
public:  
    bool init();  
    ~TouchLayer();
    virtual void update(float delta);
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
    void onExit();
    bool m_singleTouchDirecting;
    cocos2d::CCPoint m_touchedPoint;
};
#endif

