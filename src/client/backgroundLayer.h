#ifndef __BACKGROUND_LAYER_H__
#define __BACKGROUND_LAYER_H__

#include "cocos2d.h"

class BackgroundLayer:public cocos2d::CCLayer
{
public:
    ~BackgroundLayer(void);
    CREATE_FUNC(BackgroundLayer);
    virtual bool init();
    void changeBg(float sumTime, float step);
protected:
    static void reloadBg(cocos2d::CCSprite *sprite, int bgIndex);
    void myLoop(float duration);
    void togetherBg(float togetherX);
    cocos2d::CCSprite *m_bg;
    cocos2d::CCSprite *m_nextBg;
    int m_bgIndex;
    float m_remainTime;
    float m_step;
};
#endif

