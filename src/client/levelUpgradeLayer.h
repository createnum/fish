#ifndef __LEVEL_UPGRADE_LAYER_H__
#define __LEVEL_UPGRADE_LAYER_H__
#include "cocos2d.h"

class LevelUpgradeLayer:public cocos2d::CCLayer
{
public:
    static LevelUpgradeLayer* create(int level, int gold, const std::string &bombAdd);
    ~LevelUpgradeLayer(void);
    virtual bool init(int level, int gold, const std::string &bombAdd);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void showSelf(float duration);
    void removeSelf(float duration);
    virtual void onEnter();
    virtual void onExit();
    int m_level;
    int m_gold;
    std::string m_bombAdd;
};

#endif //__HELP_SCENE_H__

