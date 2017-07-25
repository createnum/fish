#include "gameScene.h"
#include "myGame.h"
#include "configMacro.h"
#include "tipsDict.h"
#include "menuScene.h"

#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace cocos2d;

GameScene* GameScene::m_inst = 0;

CCScene* GameScene::scene(int level, int exp, int rageValue, int currentDay, int currentDayRunTimes, int lastId,int taskCurrentSuccessCount,float taskCurrentRemainTime,bool firstGame,int orderId,float waitTime){
    CCScene* scene = NULL;
    do{
        scene = CCScene::create();
        CC_BREAK_IF(!scene);

        GameScene* layer = GameScene::create();
        scene->addChild(layer);
        layer->m_spriteLayer->setOnlineRewardInfo(currentDay,orderId,waitTime);
        layer->m_spriteLayer->setFirstGame(firstGame);
        layer->m_spriteLayer->setLevelInfo(level, exp);
        layer->m_spriteLayer->m_rageValue = rageValue;
        layer->m_spriteLayer->refreshRageInfo();
        layer->m_spriteLayer->setTaskInfo(currentDay, currentDayRunTimes, lastId, taskCurrentSuccessCount, taskCurrentRemainTime);
    }while(0);

    return scene;
}

bool GameScene::init(){
    do{
        CC_BREAK_IF(!CCLayer::init());
        this->setKeypadEnabled(true);

        m_bgLayer = BackgroundLayer::create();
        CC_BREAK_IF(!m_bgLayer);
        m_bgLayer->retain();
        this->addChild(m_bgLayer, 0);

        m_spriteLayer = SpriteLayer::create();
        CC_BREAK_IF(!m_spriteLayer);
        m_spriteLayer->retain();
        this->addChild(m_spriteLayer, 100); 
        m_spriteLayer->refreshGold(MyGame::getInstance()->getGold());

        m_touchLayer = TouchLayer::create();
        CC_BREAK_IF(!m_touchLayer);
        m_touchLayer->setDelegate(m_spriteLayer);  
        m_touchLayer->retain();
        this->addChild(m_touchLayer,200);

        m_menuLayer = MenuLayer::create();
        CC_BREAK_IF(!m_menuLayer);
        m_menuLayer->retain();
        this->addChild(m_menuLayer, 500);
        
        //m_spriteLayer->updateFishMovement();
        m_inst = this;
        return true;
    }while(0);
    return false;
}

GameScene::~GameScene(void)
{
    m_inst = 0;
    if (m_bgLayer)
    {
        m_bgLayer->release();
        m_bgLayer=0;
    }
    if (m_spriteLayer)
    {
        m_spriteLayer->release();
        m_spriteLayer=0;
    }
    if (m_touchLayer)
    {
        m_touchLayer->release();
        m_touchLayer=0;
    }
    if (m_menuLayer)
    {
        m_menuLayer->release();
        m_menuLayer=0;
    }
}

void GameScene::keyBackClicked(void){
    MyGame::getInstance()->changeToMenuScene();
}

// this->schedule(schedule_selector(GameScene::updateGame)); 
// 而updateGame函数包含了两个主要操作--刷新鱼群位置和碰撞检测，其代码如下所示：
// 
//     void GameScene::updateGame(ccTime dt)  
// {  
//     SpriteLayer* sLayer = (SpriteLayer*)this->getChildByTag(sprite_layer_tag);  
//     sLayer->updateFishMovement(dt);  
//     sLayer->checkBulletCollideWithFish();  
// } 

void GameScene::_updateGoldShow(int gold){
    m_spriteLayer->refreshGold(gold);
}
