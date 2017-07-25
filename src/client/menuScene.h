#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"

class MenuScene:public cocos2d::gui::TouchGroup
{
public:
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);
    virtual bool init();
    virtual void update(float delta);
    void bubbleMoveActionEnd(cocos2d::CCNode* sender);
    void touchedCallBack(CCObject*,cocos2d::gui::TouchEventType);
    ~MenuScene(void);

protected:
    float m_addBubbleRemainTime;
    void keyBackClicked();
    bool m_exit;
};

class SettingLayer:public cocos2d::gui::TouchGroup
{
public:
    CREATE_FUNC(SettingLayer);
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void refreshSoundButton(bool isOn);
    void touchedCallBack(CCObject*,cocos2d::gui::TouchEventType);
    ~SettingLayer(void);
    bool m_showHelp;
};

#endif //__MENU_SCENE_H__

