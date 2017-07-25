#ifndef __LEVEL_UPGRADE_LAYER_H__
#define __LEVEL_UPGRADE_LAYER_H__
#include "cocos2d.h"

class BezierTest:public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* scene();
    static BezierTest* create();
    ~BezierTest(void);
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void refresh();
    int m_id;
};

#endif //__HELP_SCENE_H__

