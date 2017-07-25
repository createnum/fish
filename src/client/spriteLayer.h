#ifndef __SPRITE_LAYER_H__
#define __SPRITE_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "fishDelegates.h"
#include "b2Sprite.h"

class Box2dHandler;
class B2Sprite;
class FormationConfig;
class RewardConfig;
class TaskInfo;
class TaskUI;

class SpriteLayer:public cocos2d::CCLayer, public TouchLayerDelegate, public Box2dHandlerDelegate
{
public:
    cocos2d::CCSprite * loadFishSpriteFrame(const char *frameName, int frameCount, int frameBeginIndex, bool repeatForever = true, int times = 1);
    static bool isFish(const int& tag);
    static bool isBall(const int& tag);
    static bool isNet(const int& tag);

    cocos2d::CCSprite* getLightningRing(int diameter);
    cocos2d::CCSprite* getLightning(const cocos2d::CCPoint &targetPosition);
    void lightningRepeat(CCNode *node);
public:
    ~SpriteLayer(void);
    CREATE_FUNC(SpriteLayer);
    virtual bool init();
    void saveGameData(bool force);
    void pauseGame();
    void resumeGame();
    void turnControlBar(bool show);
    void setLevelInfo(int level, int exp);
    void refreshLevelInfo(bool allRefresh);
    void refreshRageInfo();
    void refreshGold(int gold);
    void setTaskInfo(int currentDay, int currentDayRunTimes, int lastId, int taskCurrentSuccessCount,float taskCurrentRemainTime);
    void refreshSoundButton();
    void refreshCannon();
    virtual void update(float delta);
    void showSpritePrompt(cocos2d::CCSprite *sprite, bool showAmn);
    void showPromptOfAboveCenter(const char* prompt);
    void showPromptCenter(const char* prompt, int y);
    void refreshAddGoldRemainTime(int second);
    void createChangeSceneFormation();
    void newFormation(const FormationConfig *config, bool randomBeginPoint);
    B2Sprite* createFish(int fishId = -1, bool randomRoute = true);
    void updateFishMovement();  //更新所有鱼的游动路径  
    void setFishRoute(bool isBigFish, B2Sprite* fish, float speed, float lastSpeed);    //随机设定一条鱼的路线  
    void linerRoute(B2Sprite* fish,cocos2d::CCPoint from,cocos2d::CCPoint moveBy, float speed, float lastSpeed);//设置一条鱼沿直线游动  
    void bezierRoute(B2Sprite* fish,cocos2d::CCPoint from, cocos2d::ccBezierConfig &config, float speed, float lastSpeed);//设置一条鱼沿曲线游动  
    void bezierRoute2(B2Sprite* fish,cocos2d::CCPoint from, cocos2d::ccBezierConfig &config, cocos2d::ccBezierConfig &config2, float speed, float lastSpeed);//设置一条鱼沿曲线游动  
    void moveActionEnd(cocos2d::CCNode* sender);  //回调函数，用于在游动动作结束后清理精灵 
    void bombMoveActionEnd(cocos2d::CCNode *sender);
    void setBombRoute(bool begin, cocos2d::CCNode *sprite);
    
    void fishDeadActionEnd(cocos2d::CCNode *sender);
    void goldMoveActionEnd(cocos2d::CCNode *sender);
    void spritePromptMoveActionEnd(cocos2d::CCNode *sender);
    void changeSceneActionEnd(cocos2d::CCNode *sender);

    void fire(cocos2d::CCPoint);
    bool canCatchFish(bool isThunderFish, int fishGold, int bombGold);
    void catchThunderFish(B2Sprite *fish, int multiple);
    void showThunderFishAmn(float duration);
    void catchFish(B2Sprite *fish, int multiple);
    void showFishDeadSound(const string &deadSound);
    void showFishDeadTips(int flag, int rewardGold);
    void showCatchFishGoldEffect(cocos2d::CCPoint fishPos, int rewardGold);
    virtual void singleTouchEndsIn(cocos2d::CCPoint point);  
    virtual bool singleTouchDirecting(cocos2d::CCPoint point);  
    virtual void collisionEvent(B2Sprite *bullet, B2Sprite *fish);
    void touchedCallBack(cocos2d::CCObject* pSender, cocos2d::gui::TouchEventType pType);
    void showGoldAmn();
    void showGoldAmnEnd(cocos2d::extension::CCArmature * armature, cocos2d::extension::MovementEventType eventType, const char *name);
    void laserAmnEnd(cocos2d::extension::CCArmature * armature, cocos2d::extension::MovementEventType eventType, const char *name);
    void netMoveActionEnd(cocos2d::extension::CCArmature * armature, cocos2d::extension::MovementEventType eventType, const char *name);
    void removeReward();
    void setFirstGame(bool firstGame);
    void helpInterfaceinit();  
    void setOnlineRewardInfo(int currentDay,int orderId,int waitTime);
    void refreshOnlineRewardInfo();
    void onlineRewardEnd(bool success);
    void rewardGoldActionEnd(cocos2d::CCNode *sender);

    bool m_gameDataDirty;
    int m_orderId;
    float m_waitTime;
    bool todayRewardEnd;
    bool m_getReward;
    const RewardConfig *m_currentReward;
    
    cocos2d::gui::TouchGroup *m_mainUI;
    cocos2d::gui::Widget *m_bgWidget;

    bool m_stop;
    bool m_showHelp;
    bool m_firstGame;
    cocos2d::CCArray *m_fishes;
    cocos2d::CCArray *m_bombs;
    cocos2d::CCSprite *m_cannon;
    cocos2d::CCActionInterval *m_cannonAction;
    cocos2d::CCSprite *m_currentBomb;
    int m_curCannonIndex;
    int m_level;
    int m_exp;
    int m_rageValue;
    bool m_showLaserCannon;
    float m_taskBeginRemainTime;//任务开始剩余时间
    int m_taskCurrentDay;
    int m_taskCurrentDayRunTimes;//任务今天已经执行次数
    int m_taskLastId;
    TaskUI *m_taskUI;
    float m_fireRemainTime;
    float m_addGoldRemainTime;
    bool m_waitChangeScene;//等待散鱼游完轨迹
    int m_changeSceneBgRemainCount;//切换场景n次，更换一次背景
    float m_changeSceneRemainTime;//切换场景等待时间
    bool m_waitChangeBgEnd;
    bool m_showChangeSceneFormation;
    cocos2d::CCArray *m_waitFormations;

    cocos2d::CCArray *m_waitThunderFishes;
    int m_thunderFishAmnIndex;
    int m_thunderMultiple;
    int m_thunderSumRewardGold;

    float m_addFishRemainTime;//添加散鱼的最小间隔时间
    Box2dHandler *m_box2dHandler;
};

class FormationRuntimeInfo:public cocos2d::CCObject{
public:
    FormationRuntimeInfo(){
        m_showFormationConfig = 0;
        m_beginPointAdd = ccp(0,0);
        m_formationDelayTime = 0;
        m_formationFishIndex = 0;
    }
    const FormationConfig *m_showFormationConfig;//当前固定阵形配置信息
    cocos2d::CCPoint m_beginPointAdd;
    float m_formationDelayTime;//当前阵形从开始到当前时间已经消耗的时间，用来控制显示什么鱼
    unsigned int m_formationFishIndex;//当前轮到显示的鱼的位置索引
};
#endif

