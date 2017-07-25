#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
#include "backgroundLayer.h"
#include "spriteLayer.h"
#include "touchLayer.h"
#include "menuLayer.h"
#include <string>

class GameScene:public cocos2d::CCLayer
{
public:
    static GameScene *m_inst;
    static cocos2d::CCScene* scene(int level, int exp, int rageValue, int currentDay, int currentDayRunTimes, int lastId,int m_currentTaskFinishCount,float m_currentTaskRunTime,bool firstGame,int orderId,float waitTime);
    CREATE_FUNC(GameScene);
    
    virtual bool init();
    ~GameScene(void);
    virtual void keyBackClicked(void);
    static void updateGoldShow(int gold){
        if (m_inst)
        {
            m_inst->_updateGoldShow(gold);
        }
    }
    void _updateGoldShow(int gold);

    BackgroundLayer *m_bgLayer;
    SpriteLayer *m_spriteLayer;
    TouchLayer *m_touchLayer;
    MenuLayer *m_menuLayer;
};

#endif //__LOGIN_SCENE_H__

