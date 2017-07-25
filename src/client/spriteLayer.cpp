#include "spriteLayer.h"
#include "cocos-ext.h"
#include <string>
#include <ctime>
#include <cmath>
#include "fishDict.h"
#include "tipsDict.h"
#include "levelDict.h"
#include "formationDict.h"
#include "b2Sprite.h"
#include "box2dHandler.h"
#include "levelUpgradeLayer.h"
#include "fishRoute.h"
#include "myGame.h"
#include "CommonFunc.h"
#include "menuScene.h"
#include "storeScene.h"
#include "configMacro.h"
#include "gameScene.h"
#include "SimpleAudioEngine.h"
#include "taskDict.h"
#include "rewardDict.h"
#include "taskUI.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif

using namespace std;
using namespace cocos2d;
using namespace cocos2d::gui;
using namespace cocos2d::extension;

#define  Reward_Image_View  "ImageView_22"
#define  Reward_Label_Atlas "LabelAtlas_24"

static const string GAME_SCENE_ROOT_PATH = string("ui/game/");
static const string CANNON_AND_NET_PLIST_NAME = "yupaoyuwangyuzidan0.plist";
static const string CANNON_AND_NET_PNG_NAME = "yupaoyuwangyuzidan0.png";
static const char *CANNON_PIC_FILE_NAME = "pao_%01d_%01d.png";
static const char *NET_PIC_FILE_NAME = "yuwang_%01d.png";
static const string LIGHTNING_PLIST_NAME = "Lightning0.plist";
static const string LIGHTNING_PNG_NAME = "Lightning0.png";
static const int BULLET_LEVEL[] = {1,2,3,4,5,6,7,8,9,10,20,30,40,50,100};
static const int NET_LEVEL_RECT[] = {87,107,127,147,167,187,197,217,237,257};

static const int FISH_AMN_BEGIN_INDEX = 0;
static const int ADD_GOLD_INTERVAL_SECOND=60;
static const int CHANGE_SCENE_INTERVAL_SECOND =3*60;
static const int CHANGE_SCENE_BG_TIMES = 3;
static const float CHANGE_BG_NEED_SUM_TIME = 2.0f;
static const int CAN_ADD_GOLD_MAX_GOLD=200;
static const int PER_TIME_ADD_GOLD_COUNT=8;

static const int MAX_RAGE_VALUE = 2500;
static const float LASER_INTERVAL_SECOND = 1.0f;
static const float FIRE_INTERVAL_SECOND = 0.20f;
static const int TASK_INTERVAL_SECOND = 5 * 60;
static const int TASK_PER_DAY_RUN_TIMES = 3;

static const int FISH_ID_MERMAID_1 = 10;
static const int FISH_ID_MERMAID_2 = 11;
static const int FISH_ID_IPHONE = 17;

enum gameTag{
    FISH_TAG = 10000,
    BALL_TAG,
    NET_TAG,
    BULLET_TAG,
    LASER_AMN_TAG,
    LASER_WAIT_AMN_TAG,
    DEAD_FISH_TIPS_TAG,
    ADD_CANNON_TIPS_TAG,
    ABOVE_CENTER_PROMPT_TAG,
    Reward_Image_Tag,
    Help_Layer_Tag,
    REWARD_ROTATE_AMN_TAG,

    THUNDER_AMN_BEGIN_TAG = 10000000,
};
 
static const int CONTROLER_BUTTON_Z_ORDER = 5;

static const int FISH_MIN_COUNT = 50;
static const int ADD_FISH_MIN_COUNT = 2;
static const int ADD_FISH_MAX_COUNT = 7;
static const int ADD_FISH_INTERVAL_TIME = 1;
static const int MINI_GEOUP_POSSIBILITY = 5;
// static const int FISH_MIN_COUNT = 1;
// static const int ADD_FISH_MIN_COUNT = 1;
// static const int ADD_FISH_MAX_COUNT = 1;
// static const int ADD_FISH_INTERVAL_TIME = 1;
// static const int  MINI_GEOUP_POSSIBILITY = 1;

static const int FISH_BASE_SPEED = 60;
static const int BOMB_BASE_SPEED = 450;

static const char *GOLD_PARENT_NAME = "ImageView_43";
static const char *GOLD_TIME_LABEL_NAME = "LabelAtlas_48";
static const char *GOLD_ADD_BUTTON_NAME = "Button_45";
static const char *GOLD_LABEL_NAME = "LabelAtlas_47";
static const char *CANNON_CONTROL_PRAENT_NAME = "ImageView_49";
static const char *CANNON_DECREASE_BUTTON_NAME ="Button_53";
static const char *CANNON_ADD_BUTTON_NAME ="Button_53_0";

static const char *CONTROL_BAR_BUTTON_NAME = "Button_27_0";
static const char *CONTROL_BAR_BUTTON_IMAGE_NAME = "ImageView_28_0";
static const char *CONTROL_BAR_PARENT_NAME = "ImageView_38";
static const char *CONTROL_MAIN_PAGE_BUTTON_NAME = "Button_39";
static const char *CONTROL_MALL_BUTTON_NAME = "Button_39_0";
static const char *MAIN_CONTROL_MALL_BUTTON_NAME = "Button_39_0_01";
static const char *CONTROL_GIFT_BUTTON_NAME = "Button_58";
static const char *CONTROL_SOUND_BUTTON_NAME = "Button_39_1";
static const char *CONTROL_HELP_BUTTON_NAME = "Button_39_2";
static const char *ONLINE_REWARD_BUTTON = "Button_23";
static const char *ONLINE_TIME_LABEL = "Label_25";
static const char* TEST_AD_BUTTON ="Button_Ad_Test";

static const string BOMB_EFFECT_AMN_PATH = "zhongjipao.ExportJson";
static const char *BOMB_EFFECT_AMN_NAME = "zhongjipao";
// static const string NET_EFFECT_AMN_PATH = "wangtexiao.ExportJson";
// static const char *NET_EFFECT_AMN_NAME = "wangtexiao";
static const string GOLD_EFFECT_AMN_PATH = "jinbi.ExportJson";
static const char *GOLD_EFFECT_AMN_NAME = "jinbi";
static const string FISH_DEAD_AMN_PATH = "buyutexiao.ExportJson";
static const char *FISH_DEAD_AMN_NAME = "buyutexiao";
static const string LASER_USE_AMN_PATH = "gaojipao1.ExportJson";
static const char *LASER_USE_AMN_NAME = "gaojipao1";

static const string NET_EFFECT_AMN_PATH = "yuwang01-10.ExportJson";
static const string REWARD_ROTATE_AMN_PATH = "xuanzhuandonghua.ExportJson";
static const char *REWARD_ROTATE_AMN_NAME = "xuanzhuandonghua";

static const string GOLD_REWARD_AMN_PATH = "jinbidiaoluo.ExportJson";


CCSprite* SpriteLayer::loadFishSpriteFrame(const char *frameName, int frameCount, int frameBeginIndex, bool repeatForever, int times){
    //创建动画每一帧，从cache里面读取  
    CCArray *animFrames = CCArray::create();
    CCSprite *sprite = 0;
    char str[64] = {0};
    for(int i = 0; i < frameCount; i++)   
    {
        sprintf(str, frameName, frameBeginIndex + i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        if (!sprite)
        {
            sprite = CCSprite::createWithSpriteFrame(frame);
        }
        animFrames->addObject(frame);  
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f); 
    CCAnimate *splitAnimate= CCAnimate::create(animation);
    if (repeatForever)
    {
        sprite->runAction(CCRepeatForever::create(splitAnimate)); 
    }else{
        CCRepeat *repet = CCRepeat::create(splitAnimate, times);
        CCAction* action = CCSequence::create(repet, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::fishDeadActionEnd)), NULL);  
        sprite->runAction(action);
    }
    return sprite;
}

bool SpriteLayer::isFish(const int& tag)
{  
    return tag == FISH_TAG;  
}  

bool SpriteLayer::isBall(const int& tag){
    return tag == BALL_TAG;
}

bool SpriteLayer::isNet(const int& tag){
    return tag == NET_TAG;
}

CCSprite* SpriteLayer::getLightningRing(int diameter)
{
    float scaleValue = diameter/155.0f;
    CCArray* animFrames = CCArray::create();
    char str[64] = {0};
    CCSprite *lightningRing = 0;    
    for(int i = 0; i < 8; i++)   
    {
        sprintf(str, "LightningRing_%d.png",  i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        if (!lightningRing)
        {
            lightningRing = CCSprite::createWithSpriteFrame(frame);
        }
         animFrames->addObject(frame);  
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f); 
    CCAnimate *splitAnimate= CCAnimate::create(animation);
    CCRepeatForever *repeatForever = CCRepeatForever::create(splitAnimate);
    lightningRing->runAction(repeatForever);
    lightningRing->setScale(scaleValue);
    return lightningRing;
}

CCSprite* SpriteLayer::getLightning(const cocos2d::CCPoint &targetPosition)
{
    CCArray* animFrames = CCArray::create();
    char str[64] = {0};
    CCSprite *lightning = 0;
    for(int i = 0; i < 2; i++)   
    {
        sprintf(str, "Lightning%d.png",  i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        if (!lightning)
        {
            lightning = CCSprite::createWithSpriteFrame(frame);
        }
        animFrames->addObject(frame);  
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f); 
    CCAnimate *splitAnimate= CCAnimate::create(animation);

    CCCallFuncN *lightCallBack = CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::lightningRepeat));
    CCAction *action = CCSequence::create(splitAnimate,lightCallBack,NULL);
    lightning->runAction(action);

    float x = -targetPosition.x;
    float y = -targetPosition.y;
    float lightningLength = sqrt(x*x + y*y);
    float scaleValue = lightningLength/253;
    float lightningAngle;
    lightningAngle = CC_RADIANS_TO_DEGREES(atan2((x),(y)))+90;

    lightning->setScaleX(scaleValue);
    lightning->setRotation(lightningAngle);
    lightning->setAnchorPoint(ccp(0,0.5));
    return lightning; 
}
void SpriteLayer::lightningRepeat(CCNode *node)
{
    CCArray* animFrames = CCArray::create();
    char str[64] = {0};
    for (int j=0; j < 2; j++)
    {
        sprintf(str, "Lightning_%d.png",  j);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        animFrames->addObject(frame);  
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f); 
    CCAnimate* splitAnimate= CCAnimate::create(animation);
    CCRepeatForever *repeatForever = CCRepeatForever::create(splitAnimate);
    node->runAction(repeatForever);
}

SpriteLayer::~SpriteLayer(void)
{
    this->unscheduleUpdate();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpriteLayer::showThunderFishAmn), this);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(LASER_WAIT_MUSIC_PATH);
    saveGameData(true);

    m_box2dHandler->unscheduleUpdate();
    CC_SAFE_RELEASE_NULL(m_cannon);
    CC_SAFE_RELEASE_NULL(m_cannonAction);
    CC_SAFE_RELEASE_NULL(m_currentBomb);
    CC_SAFE_RELEASE_NULL(m_box2dHandler);
    CC_SAFE_RELEASE_NULL(m_fishes);
    CC_SAFE_RELEASE_NULL(m_waitFormations);
    CC_SAFE_RELEASE_NULL(m_bombs);
    CC_SAFE_RELEASE_NULL(m_mainUI);
    CC_SAFE_RELEASE_NULL(m_bgWidget);
    CC_SAFE_RELEASE_NULL(m_waitThunderFishes);
    CC_SAFE_RELEASE_NULL(m_taskUI);

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    string plistPath = GAME_SCENE_ROOT_PATH + "birds_0.plist";
    cache->removeSpriteFramesFromFile(plistPath.c_str());
    plistPath = GAME_SCENE_ROOT_PATH + "birds_1.plist";
    cache->removeSpriteFramesFromFile(plistPath.c_str());
    plistPath = GAME_SCENE_ROOT_PATH + "birds_2.plist";
    cache->removeSpriteFramesFromFile(plistPath.c_str());
    plistPath = GAME_SCENE_ROOT_PATH + LIGHTNING_PLIST_NAME;
    cache->removeSpriteFramesFromFile(plistPath.c_str());
    
    string path = GAME_SCENE_ROOT_PATH + BOMB_EFFECT_AMN_PATH;
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(path.c_str());
//     path = GAME_SCENE_ROOT_PATH + NET_EFFECT_AMN_PATH;
//     CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(path.c_str());
    path = GAME_SCENE_ROOT_PATH + GOLD_EFFECT_AMN_PATH;
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(path.c_str());

    string NET_EFFECT_AMN_NAME = GAME_SCENE_ROOT_PATH + NET_EFFECT_AMN_PATH;
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(NET_EFFECT_AMN_NAME.c_str());

    path = GAME_SCENE_ROOT_PATH + REWARD_ROTATE_AMN_PATH;
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(path.c_str());

    path = GAME_SCENE_ROOT_PATH + GOLD_REWARD_AMN_PATH;
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(path.c_str());
}

bool SpriteLayer::init()  
{  
    bool bRet = false;  
    do {
        CC_BREAK_IF(!CCLayer::init());  

        m_showHelp = false;
        m_gameDataDirty = false;
        srand((unsigned)time(0));
        m_showChangeSceneFormation = false;
        m_fireRemainTime = 0;

        m_addGoldRemainTime = ADD_GOLD_INTERVAL_SECOND;
        m_waitChangeScene = false;
        m_changeSceneBgRemainCount = CHANGE_SCENE_BG_TIMES;
        m_changeSceneRemainTime = CHANGE_SCENE_INTERVAL_SECOND;
        m_waitChangeBgEnd = false;
        m_addFishRemainTime = 0;
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();  

        m_fishes = CCArray::create();  
        m_fishes->retain();  

        m_bombs = CCArray::create();
        m_bombs->retain();

        m_waitFormations = CCArray::create();
        m_waitFormations->retain();

        m_waitThunderFishes = CCArray::create();
        m_waitThunderFishes->retain();
        m_thunderFishAmnIndex = 0;
        m_thunderMultiple = 0;
        m_thunderSumRewardGold=0;

        m_mainUI = TouchGroup::create();
        CC_BREAK_IF(!m_mainUI);
        m_mainUI->retain();
        this->addChild(m_mainUI, CONTROLER_BUTTON_Z_ORDER);
        string uiPath = "ui/game/buyuzhujiem_1.ExportJson";
        m_bgWidget = extension::GUIReader::shareReader()->widgetFromJsonFile(uiPath.c_str());

        CC_BREAK_IF(!m_bgWidget);
        m_bgWidget->retain();
        m_mainUI->addWidget(m_bgWidget);

        Button *mallButton = Button::create();
        mallButton->loadTextureNormal("ui/game/shangchenganniu.png");
        mallButton->setAnchorPoint(ccp(0.5f, 0.5f));
        mallButton->setPosition(ccp(749,430));
        mallButton->setTouchEnabled(true);
        mallButton->setName(MAIN_CONTROL_MALL_BUTTON_NAME);
        mallButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));
        m_bgWidget->addChild(mallButton);

        Widget* cannonWidget = m_bgWidget->getChildByName(CANNON_CONTROL_PRAENT_NAME);
        Button* decreseButton = dynamic_cast<Button*>(cannonWidget->getChildByName(CANNON_DECREASE_BUTTON_NAME));
        CC_BREAK_IF(!decreseButton);
        decreseButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));

        Button* addButton = dynamic_cast<Button*>(cannonWidget->getChildByName(CANNON_ADD_BUTTON_NAME));
        CC_BREAK_IF(!addButton);
        addButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));

        Button* addGoldButton = dynamic_cast<Button*>(m_bgWidget->getChildByName(GOLD_PARENT_NAME)->getChildByName(GOLD_ADD_BUTTON_NAME));
        CC_BREAK_IF(!addGoldButton);
        addGoldButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));

        m_stop = false;
        Widget* controlBar= m_bgWidget->getChildByName(CONTROL_BAR_PARENT_NAME);
        CC_BREAK_IF(!controlBar);
        controlBar->setPosition(ccp(controlBar->getPosition().x, 0 - controlBar->getContentSize().height));

        Button* controlButton = 0;
        controlButton = dynamic_cast<Button*>(controlBar->getChildByName(CONTROL_MAIN_PAGE_BUTTON_NAME));
        CC_BREAK_IF(!controlButton);
        controlButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));
        controlButton = dynamic_cast<Button*>(controlBar->getChildByName(CONTROL_MALL_BUTTON_NAME));
        CC_BREAK_IF(!controlButton);
        controlButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));
        controlButton = dynamic_cast<Button*>(controlBar->getChildByName(CONTROL_GIFT_BUTTON_NAME));
        CC_BREAK_IF(!controlButton);
        controlButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));   
        controlButton = dynamic_cast<Button*>(controlBar->getChildByName(CONTROL_SOUND_BUTTON_NAME));
        CC_BREAK_IF(!controlButton);
        controlButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));
        controlButton = dynamic_cast<Button*>(controlBar->getChildByName(CONTROL_HELP_BUTTON_NAME));
        CC_BREAK_IF(!controlButton);
        controlButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));
        refreshSoundButton();

        Button *stopButton = dynamic_cast<Button*>(m_bgWidget->getChildByName(CONTROL_BAR_BUTTON_NAME));
        CC_BREAK_IF(!stopButton);
        stopButton->addTouchEventListener(this, toucheventselector(SpriteLayer::touchedCallBack));

        Button *onlineRewardButton = dynamic_cast<Button*>(m_bgWidget->getChildByName(ONLINE_REWARD_BUTTON));
        CC_BREAK_IF(!onlineRewardButton);
        onlineRewardButton->addTouchEventListener(this,toucheventselector(SpriteLayer::touchedCallBack));
        onlineRewardButton->setVisible(false);

        Label *countDownTime = dynamic_cast<Label *>(m_bgWidget->getChildByName(ONLINE_TIME_LABEL));
        countDownTime->setVisible(false);

        /*炮台的初始化*/  
        m_showLaserCannon = false;
        m_curCannonIndex = 9;
        m_cannonAction = 0;
        m_currentBomb = 0;
        m_cannon = CCSprite::create();
        m_cannon->retain();
        this->addChild(m_cannon, CONTROLER_BUTTON_Z_ORDER + 1);  
        this->refreshCannon();

        setLevelInfo(1,0);

        m_rageValue = 0;
        this->refreshRageInfo();

        m_orderId = 0;
        m_waitTime = 0;
        m_getReward = false;
        todayRewardEnd = false;
        m_taskBeginRemainTime = TASK_INTERVAL_SECOND;
        m_taskCurrentDay = 0;
        m_taskCurrentDayRunTimes = 0;
        m_taskLastId = 0;
        m_taskUI = 0;

        m_box2dHandler = Box2dHandler::handler();
        CC_BREAK_IF(!m_box2dHandler);
        m_box2dHandler->retain();
        m_box2dHandler->scheduleUpdate();
        m_box2dHandler->setDelegate(this);
        this->addChild(m_box2dHandler);

        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        string plistPath = GAME_SCENE_ROOT_PATH + "birds_0.plist";
        string pngPath = GAME_SCENE_ROOT_PATH + "birds_0.png";
        cache->addSpriteFramesWithFile(plistPath.c_str(), pngPath.c_str());
        plistPath = GAME_SCENE_ROOT_PATH + "birds_1.plist";
        pngPath = GAME_SCENE_ROOT_PATH + "birds_1.png";
        cache->addSpriteFramesWithFile(plistPath.c_str(), pngPath.c_str());
        plistPath = GAME_SCENE_ROOT_PATH + "birds_2.plist";
        pngPath = GAME_SCENE_ROOT_PATH + "birds_2.png";
        cache->addSpriteFramesWithFile(plistPath.c_str(), pngPath.c_str());
        plistPath = GAME_SCENE_ROOT_PATH + LIGHTNING_PLIST_NAME;
        pngPath = GAME_SCENE_ROOT_PATH + LIGHTNING_PNG_NAME;
        cache->addSpriteFramesWithFile(plistPath.c_str(),pngPath.c_str());

        string path = GAME_SCENE_ROOT_PATH + BOMB_EFFECT_AMN_PATH;
        CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());

        path = GAME_SCENE_ROOT_PATH + GOLD_EFFECT_AMN_PATH;
        CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());

        string NET_EFFECT_AMN_NAME = GAME_SCENE_ROOT_PATH + NET_EFFECT_AMN_PATH;
        CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(NET_EFFECT_AMN_NAME.c_str());

        path = GAME_SCENE_ROOT_PATH + REWARD_ROTATE_AMN_PATH;
        CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());

        path = GAME_SCENE_ROOT_PATH + GOLD_REWARD_AMN_PATH;
        CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());
       
        this->scheduleUpdate();
        bRet = true;  
    } while (0);  
    return bRet;  
}

void SpriteLayer::saveGameData(bool force){
    if (!force && !m_gameDataDirty)
    {
        return;
    }
    int taskCurrentSuccessCount = 0;
    float taskCurrentRemainTime = 0;
    if (m_taskUI)
    {
        taskCurrentSuccessCount = m_taskUI->m_taskCurrentSuccessCount;
        taskCurrentRemainTime = m_taskUI->m_taskCurrentRemainTime;
    }
    MyGame::getInstance()->saveGameData(m_level, m_exp, m_rageValue, m_taskCurrentDay, m_taskCurrentDayRunTimes, m_taskLastId,taskCurrentSuccessCount,taskCurrentRemainTime,m_firstGame,m_orderId,m_waitTime);
    m_gameDataDirty = false;
}

void SpriteLayer::helpInterfaceinit()
{
    CCLayerColor * helpLayer = CCLayerColor::create(ccc4(128,128,128,125),800,480);
    this->addChild(helpLayer,500,Help_Layer_Tag);
    CCSprite *help = CCSprite::create("ui/game/help.jpg");
    help->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/2,MyGame::DEFAULT_STANDARD_HEIGHT/2));
    helpLayer->addChild(help);
}

void SpriteLayer::setFirstGame(bool firstGame)
{
     this->m_firstGame = firstGame;
     if (m_firstGame)
     {
         this->helpInterfaceinit();
         this->pauseGame();
         m_firstGame = false;
         m_showHelp = true;
     }
     else
         MyGame::getInstance()->tryShowCheckinLayer(this); 
}

void SpriteLayer::pauseGame(){
    if (m_stop)
    {
        return;
    }
    m_stop = true;
    for (unsigned int i=0;i<m_fishes->count();i++)
    {
        B2Sprite* f = dynamic_cast<B2Sprite*>(m_fishes->objectAtIndex(i));
        f->pauseSchedulerAndActions();
    }
    for (unsigned int i=0;i<m_bombs->count();i++)
    {
        B2Sprite* f = dynamic_cast<B2Sprite*>(m_bombs->objectAtIndex(i));
        f->pauseSchedulerAndActions();
    }
    ImageView* image = dynamic_cast<ImageView*>(m_bgWidget->getChildByName(CONTROL_BAR_BUTTON_NAME)->getChildByName(CONTROL_BAR_BUTTON_IMAGE_NAME));
    image->loadTexture("ui/game/zangtingann01.png");
    if (MyGame::SHOW_TEST_AD_BUTTON)
    {
        Button* moreButton = Button::create();
        moreButton->loadTextureNormal("ui/menu/ad.jpg");
        moreButton->loadTexturePressed("ui/menu/ad.jpg");
        moreButton->setName(TEST_AD_BUTTON);
        moreButton->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/2-30, 350));
        moreButton->setTouchEnabled(true);
        moreButton->addTouchEventListener((CCObject*)this, toucheventselector(MenuScene::touchedCallBack));
        m_bgWidget->addChild(moreButton,100); 
    }
}

void SpriteLayer::resumeGame(){
    if (!m_stop)
    {
        return;
    }
    m_stop = false;
    for (unsigned int i=0;i<m_fishes->count();i++)
    {
        B2Sprite* f = dynamic_cast<B2Sprite*>(m_fishes->objectAtIndex(i));
        f->resumeSchedulerAndActions();
    }
    for (unsigned int i=0;i<m_bombs->count();i++)
    {
        B2Sprite* f = dynamic_cast<B2Sprite*>(m_bombs->objectAtIndex(i));
        f->resumeSchedulerAndActions();
    }
    ImageView* image = dynamic_cast<ImageView*>(m_bgWidget->getChildByName(CONTROL_BAR_BUTTON_NAME)->getChildByName(CONTROL_BAR_BUTTON_IMAGE_NAME));
    image->loadTexture("ui/game/zangtingann00.png");
    if (MyGame::SHOW_TEST_AD_BUTTON)
    {
        CCNode* node = m_bgWidget->getChildByName(TEST_AD_BUTTON);
        m_bgWidget->removeChild(node); 
    }
}

void SpriteLayer::turnControlBar(bool show){
    Widget* controlBar= m_bgWidget->getChildByName(CONTROL_BAR_PARENT_NAME);
    CCPoint targetPoint;
    targetPoint.x = controlBar->getPosition().x;
    if (show)
    {
        targetPoint.y = 6;
    }else{
        targetPoint.y = 0 - controlBar->getContentSize().height;
    }
    CCMoveTo* move = CCMoveTo::create(0.2f, targetPoint);
    controlBar->runAction(move);
}

void SpriteLayer::setLevelInfo(int level, int exp){
    this->m_level = level;
    this->m_exp = exp;
    this->refreshLevelInfo(true);
}

void SpriteLayer::refreshLevelInfo(bool allRefresh){
    const LevelConfig *config = LevelDict::getInstance()->getLevelConfig(m_level);
    Widget* parentWidget = m_bgWidget->getChildByName("ImageView_23");
    while (m_exp >= config->upgradeExp)
    {
        m_level++;
        MyStatisticsLog::sendLevelChangeLog(m_level);
        MyGame::getInstance()->playerSoundEffect(LEVEL_UP_MUSIC_PATH,false,true);
        m_exp -= config->upgradeExp;
        int lastCannonCount = config->bombCount;
        config = LevelDict::getInstance()->getLevelConfig(m_level);
        MyGame::getInstance()->addGold(config->rewardGold);
        this->refreshGold(MyGame::getInstance()->getGold());
        showGoldAmn();
        LevelUpgradeLayer *layer = LevelUpgradeLayer::create(m_level, config->rewardGold, config->cannonAdd);
        this->addChild(layer, 100);
        if (config->bombCount != lastCannonCount)
        {
            CCNode* tips = this->getChildByTag(ADD_CANNON_TIPS_TAG);
            if (!tips)
            {
                tips = CCSprite::create("ui/game/unlockCannon.png");
                this->addChild(tips, 100, ADD_CANNON_TIPS_TAG);
            }
            tips->setScale(0);
            tips->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/2,120));
            CCEaseSineOut* sineOut = CCEaseSineOut::create(CCScaleTo::create(0.4f, 0.7f));
            CCEaseSineIn* sineIn = CCEaseSineIn::create(CCScaleTo::create(0.4f, 1.2f));
            CCEaseSineOut* sineOut2 = CCEaseSineOut::create(CCScaleTo::create(0.4f, 0.7f));
            CCEaseSineIn* sineIn2 = CCEaseSineIn::create(CCScaleTo::create(0.4f, 1.2f));
            CCEaseSineOut* sineOut3 = CCEaseSineOut::create(CCScaleTo::create(0.4f, 0));
            CCSequence *action = CCSequence::create(CCDelayTime::create(2.0f), CCScaleTo::create(0.1f, 1.0f), sineOut, sineIn, sineOut2, sineIn2, sineOut3, NULL);
            tips->runAction(action);

            m_curCannonIndex = config->bombCount - 1;
            if (!m_showLaserCannon)
            {
                this->refreshCannon();
            }
        }
        Label* showName = dynamic_cast<Label*>(parentWidget->getChildByName("Label_26"));
        showName->setText(config->showName);
    }
    char num[80];  
    sprintf(num, "%d", m_level);
    LabelAtlas* levelLabel = dynamic_cast<LabelAtlas*>(parentWidget->getChildByName("LabelAtlas_25"));
    levelLabel->setStringValue(num);
    if (allRefresh)
    {
        Label* showName = dynamic_cast<Label*>(parentWidget->getChildByName("Label_26"));
        showName->setText(config->showName);
    }
    LoadingBar* expBar = dynamic_cast<LoadingBar*>(parentWidget->getChildByName("LoadingBar_24"));
    expBar->setPercent(100*m_exp/config->upgradeExp);
}

void SpriteLayer::refreshRageInfo(){
    Widget* widget = m_bgWidget->getChildByName("ImageView_49")->getChildByName("LoadingBar_50");
    LoadingBar* bar = dynamic_cast<LoadingBar*>(widget);
    int percent = 100 * m_rageValue / MAX_RAGE_VALUE;
    if (percent > 100)
    {
        percent = 100;
    }
    bar->setPercent(percent);
    if (!m_showLaserCannon && percent>=100)
    {
        CCLog("refreshRageInfo 100");
        m_showLaserCannon = true;
        m_fireRemainTime = LASER_INTERVAL_SECOND;;
        {
            string path = GAME_SCENE_ROOT_PATH + LASER_USE_AMN_PATH;
            CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());
        }
        refreshCannon();
    }
}

void SpriteLayer::refreshGold(int gold){
    char num[80];  
    sprintf(num, "%d", gold);
    LabelAtlas* goldLabel = dynamic_cast<LabelAtlas*>(m_bgWidget->getChildByName(GOLD_PARENT_NAME)->getChildByName(GOLD_LABEL_NAME));
    goldLabel->setStringValue(num);
}

void SpriteLayer::setOnlineRewardInfo(int currentDay,int orderId,int waitTime)
{
    int c = CommonFunc::getCurrentDay();
    if (c != currentDay)
    {
        currentDay = c;
        orderId = 0;
        waitTime = 0;
    }
    if (orderId == 0)
    {
        m_currentReward = RewardDict::getInstance()->nextRewardConfig(-1);
        m_orderId = m_currentReward->orderId;
        m_waitTime = m_currentReward->waitTime;
    }
    else
    { 
        if (RewardDict::getInstance()->nextRewardConfig(orderId)==NULL)
        { 
            todayRewardEnd = true;
        }
        else if (RewardDict::getInstance()->nextRewardConfig(orderId)!=NULL)
        {

            todayRewardEnd = false;
        }
        m_orderId = orderId;
        m_waitTime = waitTime;
    }
}

void SpriteLayer::refreshOnlineRewardInfo()
{
    Label *getCountDownTime = (Label*)m_bgWidget->getChildByName(ONLINE_TIME_LABEL);
    if (!todayRewardEnd)
    {
        m_bgWidget->getChildByName(ONLINE_REWARD_BUTTON)->setVisible(true);
        if (m_waitTime > 0)
        {
            char str[128] = {0};
            int second = ((int) m_waitTime);
            int min = second / 60;
            second %= 60;
            sprintf(str, "%02d:%02d", min, second);
            getCountDownTime->setText(str);
        }
    }
    else
    {
        m_bgWidget->removeChild(m_bgWidget->getChildByName(ONLINE_REWARD_BUTTON),true);
        getCountDownTime->setVisible(false);
    }
           
    if (m_waitTime==0&&!todayRewardEnd)
    {
        getCountDownTime->setVisible(false);
        CCArmature *armature = CCArmature::create(REWARD_ROTATE_AMN_NAME);
        armature->getAnimation()->playWithIndex(0);
        armature->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/4+56,MyGame::DEFAULT_STANDARD_HEIGHT-37));
        armature->setTag(REWARD_ROTATE_AMN_TAG);
        if (NULL==this->getChildByTag(REWARD_ROTATE_AMN_TAG))
        {
            this->addChild(armature,CONTROLER_BUTTON_Z_ORDER);

        } 
    }
    else if (m_waitTime>0)
    {
       getCountDownTime->setVisible(true);
    }
}

void SpriteLayer::onlineRewardEnd(bool success)
{
    if (NULL!=this->getChildByTag(REWARD_ROTATE_AMN_TAG))
    {
        this->removeChildByTag(REWARD_ROTATE_AMN_TAG);
    }
    if (success)
    {
        MyGame::getInstance()->addGold(RewardDict::getInstance()->nextRewardConfig(m_orderId-1)->rewardGold);
        this->refreshGold(MyGame::getInstance()->getGold());
        showGoldAmn();
        MyGame::getInstance()->playerSoundEffect(REWARD_GOLD_DOWN_MUSIC_PATH,false,true);
        m_getReward = true;

        char str[10] = {0};
        sprintf(str, "%d",RewardDict::getInstance()->nextRewardConfig(m_orderId-1)->rewardGold);
        string digitPath = GAME_SCENE_ROOT_PATH+"gold_digit.png";
        CCLabelAtlas *goldLabel = CCLabelAtlas::create(str, digitPath.c_str(), 27, 35, '0');
        goldLabel->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/4+80,MyGame::DEFAULT_STANDARD_HEIGHT-90));
        this->addChild(goldLabel,CONTROLER_BUTTON_Z_ORDER);

        CCMoveTo *moveTo = CCMoveTo::create(0.5f,ccp(MyGame::DEFAULT_STANDARD_WIDTH/4+80,MyGame::DEFAULT_STANDARD_HEIGHT-55));
        CCDelayTime *delay = CCDelayTime::create(1.0f);
        CCFadeOut *fadeOut=CCFadeOut::create(0.5f);
        CCCallFuncN *callFunc = CCCallFuncN::create(this,callfuncN_selector(SpriteLayer::rewardGoldActionEnd));
        CCSequence *action = CCSequence::create(moveTo,delay,fadeOut,callFunc,NULL);
        goldLabel->runAction(action);
    }
}

void SpriteLayer::rewardGoldActionEnd(cocos2d::CCNode *sender)
{
    this->removeChild(sender,true);
}
void SpriteLayer::setTaskInfo(int currentDay, int currentDayRunTimes, int lastId, int taskCurrentSuccessCount,float taskCurrentRemainTime){
    int c = CommonFunc::getCurrentDay();
    if (c != currentDay)
    {
        currentDay = c;
        currentDayRunTimes = 0;
    }
    m_taskBeginRemainTime = TASK_INTERVAL_SECOND;
    m_taskCurrentDay = currentDay;
    m_taskCurrentDayRunTimes = currentDayRunTimes;
    m_taskLastId = lastId;

    if (0 != taskCurrentSuccessCount || 0 != taskCurrentRemainTime)
    {
        CC_SAFE_RELEASE_NULL(m_taskUI);
        m_taskUI = TaskUI::create(m_taskLastId);
        m_taskUI->retain();
        m_taskUI->setSuccessCount(taskCurrentSuccessCount);
        m_taskUI->setRemainTime(taskCurrentRemainTime);
        m_mainUI->addChild(m_taskUI);
    }
}
void SpriteLayer::refreshSoundButton(){
    Button *soundButton = dynamic_cast<Button*>(m_bgWidget->getChildByName(CONTROL_BAR_PARENT_NAME)->getChildByName(CONTROL_SOUND_BUTTON_NAME));
    string picName;
    if (MyGame::getInstance()->getSoundIsOn())
    {
        picName = "ui/game/soundOff.png";
    }else{
        picName = "ui/game/soundOn.png";
    }
    soundButton->loadTextures(picName.c_str(), picName.c_str(), picName.c_str());
}

void SpriteLayer::refreshCannon(){
    m_cannon->stopAction(m_cannonAction);
    int level;
    if (m_showLaserCannon)
    {
        level = 0;
    }else{
        level = BULLET_LEVEL[m_curCannonIndex];
    }
    CCLog("refreshCannon %d", level);
    string plistPath = GAME_SCENE_ROOT_PATH + CANNON_AND_NET_PLIST_NAME;
    string pngPath = GAME_SCENE_ROOT_PATH + CANNON_AND_NET_PNG_NAME;
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile(plistPath.c_str(), pngPath.c_str());

    //创建动画每一帧，从cache里面读取 
    int frameBeginIndex = 1;
    CCArray* animFrames = CCArray::create();
    char str[64] = {0};
    for(int i = 0; i < 2; i++)   
    {
        sprintf(str, CANNON_PIC_FILE_NAME, level, frameBeginIndex + i);
        CCSpriteFrame* frame = cache->spriteFrameByName(str);
        animFrames->addObject(frame);  
    }
    sprintf(str, CANNON_PIC_FILE_NAME, level, frameBeginIndex + 0);
    {
        CCSpriteFrame *pFrame = cache->spriteFrameByName(str);
        m_cannon->setTexture(pFrame->getTexture());
        m_cannon->setTextureRect(pFrame->getRect());
        m_cannon->setDisplayFrame(pFrame);
    }
    m_cannon->setAnchorPoint(ccp(0.5f, 0.25f));
    m_cannon->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/2, 26));
    CCSpriteFrame* frame = cache->spriteFrameByName(str);
    animFrames->addObject(frame);  

    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f); 
    CCAnimate *splitAnimate= CCAnimate::create(animation);
    CC_SAFE_RELEASE_NULL(m_cannonAction);
    m_cannonAction = CCRepeat::create(splitAnimate,1); 
    m_cannonAction->retain();

    CC_SAFE_RELEASE_NULL(m_currentBomb);
    const char* bombStr = 0;
    if (m_showLaserCannon)
    {
        bombStr = "paodan_0.png";
    }else if (level <= 2)
    {
        bombStr = "paodan_1-2.png";
    }else if (level <= 4)
    {
        bombStr = "paodan_3-4.png";
    }else if (level <= 6)
    {
        bombStr = "paodan_5-6.png";
    }else if (level <= 8)
    {
        bombStr = "paodan_7-8.png";
    }else if (level <= 9)
    {
        bombStr = "paodan_9.png";
    }else if (level <= 10)
    {
        bombStr = "paodan_10.png";
    }else if (level <= 30)
    {
        bombStr = "paodan_20-30.png";
    }else if (level <= 50)
    {
        bombStr = "paodan_40-50.png";
    }else{
        bombStr = "paodan_100.png";
    }
    m_currentBomb=CCSprite::createWithSpriteFrameName(bombStr);
    m_currentBomb->retain();

    cache->removeSpriteFramesFromFile(plistPath.c_str()); 
}

void SpriteLayer::update(float duration){
    if (m_stop)
    {
        return;
    }
    {//update add gold
        int lastAddGoldRemainSecond = static_cast<int>(m_addGoldRemainTime);
        int gold = MyGame::getInstance()->getGold();
        if (gold < CAN_ADD_GOLD_MAX_GOLD)
        {

            m_addGoldRemainTime-=duration;
            if (m_addGoldRemainTime<=0)
            {
                m_addGoldRemainTime=ADD_GOLD_INTERVAL_SECOND;
                if (gold<CAN_ADD_GOLD_MAX_GOLD)
                {
                    MyGame::getInstance()->addGold(PER_TIME_ADD_GOLD_COUNT);
                    this->refreshGold(MyGame::getInstance()->getGold());
                }
            }
        }else{
            m_addGoldRemainTime = ADD_GOLD_INTERVAL_SECOND;
        }
        int currentAddGoldRemainSecond = static_cast<int>(m_addGoldRemainTime);
        if (currentAddGoldRemainSecond != lastAddGoldRemainSecond)
        {
            this->refreshAddGoldRemainTime(currentAddGoldRemainSecond);
        }
    }

    if (m_taskUI)
    {
        m_taskUI->update(duration);
        if (m_taskUI->m_end)
        {
            if (m_taskUI->m_success)
            {
                this->m_exp += m_taskUI->m_currentTask->rewardExp;
                this->refreshLevelInfo(false);
                MyGame::getInstance()->addGold(m_taskUI->m_currentTask->rewardGold);
                this->refreshGold(MyGame::getInstance()->getGold());
                showGoldAmn();
                m_taskCurrentDayRunTimes++;
                CCSprite *sprite = CCSprite::create("ui/game/taskEndPrompt.png");
                this->showSpritePrompt(sprite, false);
            }else{
                char str[128] = {0};
                sprintf(str, TipsDict::getInstance()->getTips(TipsDict::KEY_TASK_FAILED).c_str(), m_taskUI->m_currentTask->taskName.c_str());
                this->showPromptOfAboveCenter(str);
            }
            m_taskBeginRemainTime = TASK_INTERVAL_SECOND;
            m_gameDataDirty = true;
            m_mainUI->removeChild(m_taskUI);
            CC_SAFE_RELEASE_NULL(m_taskUI);
        }
    }else if(m_taskCurrentDayRunTimes < TASK_PER_DAY_RUN_TIMES){
        m_taskBeginRemainTime -= duration;
        if (m_taskBeginRemainTime <= 0)
        {
            const TaskInfo *taskInfo = 0;
            if (m_taskLastId < 0)
            {
                taskInfo = TaskDict::getInstance()->firstTaskInfo();
            }else{
                taskInfo = TaskDict::getInstance()->nextTaskInfo(m_taskLastId);
            }
            m_taskLastId = taskInfo->id;
            m_taskUI = TaskUI::create(m_taskLastId);
            m_taskUI->retain();
            m_mainUI->addChild(m_taskUI);
        }
    }

    m_waitTime-=duration;
    if (m_waitTime<0)
    {
        m_waitTime = 0;
    }
    if (m_getReward)
    {   
        m_getReward =false;
        m_currentReward = RewardDict::getInstance()->nextRewardConfig(m_orderId);
        if (m_currentReward!=NULL)
        {
            m_orderId = m_currentReward->orderId;
            m_waitTime = m_currentReward->waitTime; 
        }
        else
        {   
            todayRewardEnd = true;
            m_bgWidget->removeChild(m_bgWidget->getChildByName(ONLINE_REWARD_BUTTON),true);
        } 
    }
        refreshOnlineRewardInfo();

    m_fireRemainTime-=duration;
    if (m_fireRemainTime<0)
    {
        m_fireRemainTime=0;
    }
    if (!m_waitChangeScene)
    {
        if (!m_showChangeSceneFormation)
        {
            m_changeSceneRemainTime-=duration;
            if (m_changeSceneRemainTime<=0)
            {
                m_waitChangeScene =true;
                m_waitChangeBgEnd = false;
                m_changeSceneRemainTime = CHANGE_SCENE_INTERVAL_SECOND;
                for (unsigned int i=0;i<m_fishes->count();i++)
                {
                    B2Sprite* f = dynamic_cast<B2Sprite*>(m_fishes->objectAtIndex(i));
                    f->m_isAccelerated = true;
                }
                m_waitFormations->removeAllObjects();
                CCSprite *sprite = CCSprite::create("ui/game/changeFormationPrompt.png");
                showSpritePrompt(sprite, true);
            }
        }
    }else if (m_fishes->count()<=0 && m_waitFormations->count() <= 0)
    {
        if (!m_waitChangeBgEnd)
        {
            m_changeSceneBgRemainCount--;
            if (m_changeSceneBgRemainCount<=0)
            {
                m_waitChangeBgEnd = true;
                //切换背景
                m_changeSceneBgRemainCount = CHANGE_SCENE_BG_TIMES;
                string path = GAME_SCENE_ROOT_PATH + "langhua.ExportJson";
                CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());
                CCArmature *armature = CCArmature::create("langhua");
                armature->getAnimation()->playWithIndex(0);
                armature->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH, MyGame::DEFAULT_STANDARD_HEIGHT/2));
                int moveLen = MyGame::DEFAULT_STANDARD_WIDTH + 60;
                CCAction* action = CCSequence::create(CCMoveBy::create(CHANGE_BG_NEED_SUM_TIME, ccp(-moveLen, 0)), CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::changeSceneActionEnd)), NULL); 
                armature->runAction(action);
                this->addChild(armature);
                GameScene::m_inst->m_bgLayer->changeBg(CHANGE_BG_NEED_SUM_TIME, moveLen / CHANGE_BG_NEED_SUM_TIME);
                CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(path.c_str());
            }else{
                m_waitChangeScene=false;
                this->createChangeSceneFormation();
            }
        }
    }
    for (unsigned int i=0;i<m_fishes->count();i++)
    {
        B2Sprite* f = dynamic_cast<B2Sprite*>(m_fishes->objectAtIndex(i));
        if (f->getIsDead())
        {
            f->setIsDead(false);
            m_box2dHandler->removeSprite(f);
        }
        if (!f->getIsAlive())
        {
            f->setIsAlive(true);
            m_box2dHandler->removeSprite(f);
            m_fishes->removeObjectAtIndex(i);
            i--;
            this->removeChild(f, true);
        }
    }
    for (unsigned int i=0;i<m_bombs->count();i++)
    {
        B2Sprite* f = dynamic_cast<B2Sprite*>(m_bombs->objectAtIndex(i));
        if (f->getIsDead())
        {
            f->setIsDead(false);
            m_box2dHandler->removeSprite(f);
        }
        if (!f->getIsAlive())
        {
            f->setIsAlive(true);
            m_box2dHandler->removeSprite(f);
            m_bombs->removeObjectAtIndex(i);
            i--;
            this->removeChild(f, true);
        }
    }
    int formationWaitFishCount = 0;
    for (unsigned int i=0;i<m_waitFormations->count();i++)
    {
        FormationRuntimeInfo* formationInfo = dynamic_cast<FormationRuntimeInfo*>(m_waitFormations->objectAtIndex(i));
        formationInfo->m_formationDelayTime += duration;
        vector<FormationFish*> fishes = formationInfo->m_showFormationConfig->m_fishes;
        while (formationInfo->m_formationFishIndex < fishes.size() && formationInfo->m_formationDelayTime >= fishes[formationInfo->m_formationFishIndex]->delay)
        {
            FormationFish* fish = fishes[formationInfo->m_formationFishIndex];
            B2Sprite* fishSprite = this->createFish(fish->fishId, false);
            float speed = FISH_BASE_SPEED;
            CCPoint beginPoint = ccp(fish->beginPoint.x + formationInfo->m_beginPointAdd.x, fish->beginPoint.y + formationInfo->m_beginPointAdd.y);
            if(0==fish->type) {  
                linerRoute(fishSprite, beginPoint, fish->line->endPoint, speed, speed);
            }  
            else if (1==fish->type)
            {
                ccBezierConfig config;
                config.controlPoint_1 = fish->line->bControlPoint;
                config.controlPoint_2 = fish->line->eControlPoint;
                config.endPosition = fish->line->endPoint;
                bezierRoute(fishSprite, beginPoint, config, speed, speed);
            }else
            {  
                ccBezierConfig config;
                config.controlPoint_1 = fish->line->bControlPoint;
                config.controlPoint_2 = fish->line->eControlPoint;
                config.endPosition = fish->line->endPoint;
                ccBezierConfig config2;
                config2.controlPoint_1 = fish->line->bControlPoint2;
                config2.controlPoint_2 = fish->line->eControlPoint2;
                config2.endPosition = fish->line->endPoint2;
                bezierRoute2(fishSprite, beginPoint, config, config2, speed, speed);
            } 
            formationInfo->m_formationFishIndex++;
        }
        int remainCount = formationInfo->m_showFormationConfig->m_fishes.size() - formationInfo->m_formationFishIndex;
        if (remainCount <= 0)
        {
            m_waitFormations->removeObjectAtIndex(i);
            i--;
        }else{
            formationWaitFishCount += remainCount;
        }
    }
    if (m_showChangeSceneFormation && m_waitFormations->count()<=0 && m_fishes->count() <= 0)
    {
        m_showChangeSceneFormation = false;
        MyGame::getInstance()->playBackgroundMusic(false);
    }
    m_addFishRemainTime -= duration;
    if (!m_waitChangeScene && !m_showChangeSceneFormation && (m_fishes->count() + formationWaitFishCount) < FISH_MIN_COUNT && m_addFishRemainTime <= 0)
    {
        m_addFishRemainTime = ADD_FISH_INTERVAL_TIME;
        int count;
        if (ADD_FISH_MAX_COUNT - ADD_FISH_MIN_COUNT > 0)
        {
            count = abs(rand()) % (ADD_FISH_MAX_COUNT - ADD_FISH_MIN_COUNT) + ADD_FISH_MIN_COUNT;
        }else{
            count = ADD_FISH_MIN_COUNT;
        }
        for (int i = 0; i < count; i++)
        {
            if (0 == (abs(rand()) % MINI_GEOUP_POSSIBILITY))
            {
                const FormationConfig *config = FormationDict::getInstance()->randomMiniFormationConfig();
                newFormation(config, true);
                count -= config->m_fishes.size() + 1;
            }else{
                this->createFish();
            }
        }
    }
    saveGameData(false);
}

void SpriteLayer::showSpritePrompt(cocos2d::CCSprite *sprite, bool showAmn){
    sprite->setAnchorPoint(ccp(0.5f, 0.5f));
    sprite->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/2, MyGame::DEFAULT_STANDARD_HEIGHT/2 + 50));
    CCAction *action = 0;
    if (showAmn)
    {
        sprite->setScale(2.5f);
        sprite->setOpacity(122);
        CCScaleTo *moveIn = CCScaleTo::create(0.5f, 1.0f);
        CCFadeTo *fadeIn = CCFadeTo::create(0.5f, 255);
        CCSpawn *in = CCSpawn::createWithTwoActions(moveIn, fadeIn);
        CCDelayTime* delay = CCDelayTime::create(1.0f);
        CCScaleTo *moveOut = CCScaleTo::create(0.2f, 0.7f);
        CCFadeTo *fadeOut = CCFadeTo::create(0.2f, 122);
        CCSpawn *out = CCSpawn::createWithTwoActions(moveOut, fadeOut);
        action = CCSequence::create(CCEaseSineIn::create(in), delay, CCEaseSineOut::create(out), CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::spritePromptMoveActionEnd)), NULL);
    }else{
        CCDelayTime* delay = CCDelayTime::create(2.0f);
        action = CCSequence::create(delay, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::spritePromptMoveActionEnd)), NULL);
    }
    sprite->runAction(action);
    this->addChild(sprite, 100);
}

void SpriteLayer::showPromptOfAboveCenter(const char* prompt){
    showPromptCenter(prompt, 300);
}

void SpriteLayer::showPromptCenter(const char* prompt, int y){
    CCNode *node = this->getChildByTag(ABOVE_CENTER_PROMPT_TAG);
    if (node)
    {
        node->stopAllActions();
        CCLabelTTF *label = dynamic_cast<CCLabelTTF*>(node);
        label->setString(prompt);
        label->setOpacity(0);
    }else{
        CCLabelTTF *label = CCLabelTTF::create(prompt, "ArialMT", 24);
        label->setAnchorPoint(ccp(0.5, 1));
        label->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/2, y));
        label->setTag(ABOVE_CENTER_PROMPT_TAG);
        this->addChild(label, 1000);
        label->setOpacity(0);
        node = label;
    }
    CCFadeIn *in = CCFadeIn::create(0.2f);
    CCFadeTo* to = CCFadeTo::create(0.5f, 0);
    CCEaseSineOut* sineOut = CCEaseSineOut::create(to);
    CCAction* action = CCSequence::create(in, CCDelayTime::create(0.8f), sineOut, NULL); 
    node->runAction(action);
}

void SpriteLayer::refreshAddGoldRemainTime(int second){
    char num[80];
    int min = second / 60;
    second %= 60;
    sprintf(num, "%02d:%02d", min, second);
    LabelAtlas* goldLabel = dynamic_cast<LabelAtlas*>(m_bgWidget->getChildByName(GOLD_PARENT_NAME)->getChildByName(GOLD_TIME_LABEL_NAME));
    goldLabel->setStringValue(num);
}

void SpriteLayer::createChangeSceneFormation(){
    MyGame::getInstance()->playBackgroundMusic(true);
    CCObject* fish = NULL;  
    CCARRAY_FOREACH(m_fishes, fish) {  
        B2Sprite* f = dynamic_cast<B2Sprite*>(fish);
        f->setIsAlive(false);
    }
    m_showChangeSceneFormation = true;
    newFormation(FormationDict::getInstance()->randomFormationConfig(), false);
}

void SpriteLayer::newFormation(const FormationConfig *config, bool randomBeginPoint){
    FormationRuntimeInfo *info = new FormationRuntimeInfo();
    info->autorelease();
    info->m_showFormationConfig = config;
    info->m_formationDelayTime=0;
    info->m_formationFishIndex=0;
    if (randomBeginPoint)
    {
        CCPoint beginPoint;
        FormationFish *firstFish = config->m_fishes[0];
        firstFish->line->makeRandomBeginPosition(beginPoint);
        info->m_beginPointAdd.x = beginPoint.x - firstFish->beginPoint.x;
        info->m_beginPointAdd.y = beginPoint.y - firstFish->beginPoint.y;
    }
    m_waitFormations->addObject(info);
}
B2Sprite* SpriteLayer::createFish(int fishId, bool randomRoute)
{
    if (fishId < 0)
    {
        do 
        {
            fishId = FishDict::getInstance()->getRandomFishId();
            const FishConfig *fishConfig = FishDict::getInstance()->getFishConfig(fishId);
            int haveCount = 0;

            CCObject* fish = NULL;  
            CCARRAY_FOREACH(m_fishes, fish) {  
                B2Sprite* sprite = dynamic_cast<B2Sprite*>(fish);
                if (sprite->getData() == fishId)
                {
                    haveCount++;
                    if (haveCount >= fishConfig->maxNum)
                    {
                        break;
                    }
                }
            }
            if (haveCount < fishConfig->maxNum){
                break;
            }
        } while (true);
    }
    if (FISH_ID_MERMAID_1==fishId || FISH_ID_MERMAID_2 == fishId)
    {
        CCSprite *sprite = CCSprite::create("ui/game/mermaidPrompt.png");
        showSpritePrompt(sprite, true);
    }
    const FishConfig *fishConfig = FishDict::getInstance()->getFishConfig(fishId);

    string plistPath = GAME_SCENE_ROOT_PATH + fishConfig->plist_path;
    string pngPath = GAME_SCENE_ROOT_PATH + fishConfig->png_path;
    
    B2Sprite *fishSprite = B2Sprite::create();
    fishSprite->addShowAmn(loadFishSpriteFrame(fishConfig->normal_name.c_str(), fishConfig->normal_count, FISH_AMN_BEGIN_INDEX));
    if (fishConfig->isThunder)
    {
        CCSprite* ring = getLightningRing(fishSprite->getContentSize().width);
        ring->setAnchorPoint(ccp(0.5f,0.5f));
        ring->setPosition(ccp(fishSprite->getContentSize().width/2, fishSprite->getContentSize().height/2));
        fishSprite->addChild(ring);
    }
    if (FISH_ID_IPHONE == fishId)
    {
        fishSprite->m_canRotation = false;
    }
    fishSprite->setData(fishId);
    fishSprite->setAnchorPoint(ccp(0.5f,0.5f));
    fishSprite->setTag(FISH_TAG);
    m_box2dHandler->addFish(fishSprite);
    m_fishes->addObject(fishSprite);  
    this->addChild(fishSprite);  
    if (randomRoute)
    {
        int rn = abs(rand() % 100);
        float lastSpeed;
        if (rn < 25)
        {
            lastSpeed = FISH_BASE_SPEED*fishConfig->accelerateSpeed;
        }else if (rn < 50)
        {
            lastSpeed = FISH_BASE_SPEED*fishConfig->decelerateSpeed;
        }else{
            lastSpeed = FISH_BASE_SPEED*fishConfig->speed;
        }
        this->setFishRoute(!fishConfig->isSmall, fishSprite, FISH_BASE_SPEED*fishConfig->speed, lastSpeed);
    }
    return fishSprite;
}

void SpriteLayer::updateFishMovement()  
{  
    CCObject* fish = NULL;  
    CCARRAY_FOREACH(m_fishes, fish) {  
        //this->setFishRoute(dynamic_cast<CCNode*>(fish), FISH_MOVE_FULL_PATH_NEED_TIME);  
    }  
} 

void SpriteLayer::setFishRoute(bool isBigFish, B2Sprite* fish, float speed, float lastSpeed)  
{  
    const FormationLine* line;
    if (isBigFish)
    {
        line = FormationDict::getInstance()->randomBigFishLine();
    }else{
        line = FormationDict::getInstance()->randomSmallFishLine();
    }
    CCPoint beginPoint;
    line->makeRandomBeginPosition(beginPoint);
    if (0 == line->type)
    {
        linerRoute(fish, beginPoint, line->endPoint, speed, lastSpeed);
    }else if (1 == line->type)
    {
        ccBezierConfig config;
        config.controlPoint_1 = line->bControlPoint;
        config.controlPoint_2 = line->eControlPoint;
        config.endPosition = line->endPoint;
        bezierRoute(fish, beginPoint, config, speed, lastSpeed);
    }else
    {
        ccBezierConfig config;
        config.controlPoint_1 = line->bControlPoint;
        config.controlPoint_2 = line->eControlPoint;
        config.endPosition = line->endPoint;
        ccBezierConfig config2;
        config2.controlPoint_1 = line->bControlPoint2;
        config2.controlPoint_2 = line->eControlPoint2;
        config2.endPosition = line->endPoint2;
        bezierRoute2(fish, beginPoint, config, config2, speed, lastSpeed);
    }
}  

void SpriteLayer::linerRoute(B2Sprite* fish, CCPoint from, CCPoint moveBy, float speed, float lastSpeed)  
{  
    fish->setPosition(from);  
    FishRoute* line = FishRoute::createLine(speed, lastSpeed, moveBy, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::moveActionEnd)));
    //CCDelayTime* waitAction = CCDelayTime::create(delay);
    //CCMoveTo* move = CCMoveTo::create(dt, to);  
    //CCAction* action = CCSequence::create(line, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::moveActionEnd)), NULL);  
        //waitAction, RotateWithAction::create(move), CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::moveActionEnd)), NULL);  

    fish->runAction(line);  
}  

void SpriteLayer::bezierRoute(B2Sprite* fish, CCPoint from, cocos2d::ccBezierConfig &config, float speed, float lastSpeed)  
{  
    CCSize s = CCDirector::sharedDirector()->getWinSize();  
    fish->setPosition(from);
    FishRoute *route = FishRoute::createBezier(speed,lastSpeed, config, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::moveActionEnd)));
    //CCAction* action = CCSequence::create(RotateWithAction::create(move), CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::moveActionEnd)), NULL);  
    fish->runAction(route);  
}  

void SpriteLayer::bezierRoute2(B2Sprite* fish, CCPoint from, cocos2d::ccBezierConfig &config, cocos2d::ccBezierConfig &config2, float speed, float lastSpeed)  
{  
    CCSize s = CCDirector::sharedDirector()->getWinSize();  
    fish->setPosition(from);
    FishRoute *route = FishRoute::createBezier2(speed,lastSpeed, config, config2, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::moveActionEnd)));
    //CCAction* action = CCSequence::create(RotateWithAction::create(move), CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::moveActionEnd)), NULL);  
    fish->runAction(route);  
}  
void SpriteLayer::moveActionEnd(CCNode* sender)  
{  
     B2Sprite* fish = dynamic_cast<B2Sprite*>(sender);
     fish->setIsAlive(false);
//     m_box2dHandler->removeSprite(fish);
//     m_fishes->removeObject(sender);
//     this->removeChild(sender,true);
    //this->setFishRoute(dynamic_cast<CCNode*>(sender), FISH_MOVE_FULL_PATH_NEED_TIME);  
}

void SpriteLayer::bombMoveActionEnd(CCNode *sender){
//     B2Sprite* fish = dynamic_cast<B2Sprite*>(sender);
//     fish->setIsAlive(false);
    sender->setRotation(90 - (180 - (90 - sender->getRotation())));
    setBombRoute(false, sender);
}

void SpriteLayer::setBombRoute(bool being, CCNode *sprite){
    CCPoint beginPosition = sprite->getPosition();
    float slope = CC_DEGREES_TO_RADIANS(90 - sprite->getRotation());

    float c = cos(slope);
    float s = sin(slope);
    float xDis = 0, yDis = 0;
    if (being)
    {
        if (c < 0)
        {
            xDis = 0 - beginPosition.x;
        }else{
            xDis = MyGame::DEFAULT_STANDARD_WIDTH - beginPosition.x;
        }
        yDis = MyGame::DEFAULT_STANDARD_HEIGHT - beginPosition.y;
    }else{
        if (beginPosition.x<=0)
        {
            xDis = MyGame::DEFAULT_STANDARD_WIDTH - beginPosition.x;
        }else if (beginPosition.x>=MyGame::DEFAULT_STANDARD_WIDTH)
        {
            xDis = 0 - beginPosition.x;
        }
        if (beginPosition.y<=0)
        {
            yDis = MyGame::DEFAULT_STANDARD_HEIGHT-beginPosition.y;
        }else if (beginPosition.y>=MyGame::DEFAULT_STANDARD_HEIGHT)
        {
            yDis = 0 - beginPosition.y;
        }
        CCAssert(0 != xDis || 0 != yDis, "");
        float t = tan(slope);
        if (0==xDis)
        {
            if(yDis * t > 0){
                xDis = MyGame::DEFAULT_STANDARD_WIDTH - beginPosition.x;
            }else{
                xDis = 0 - beginPosition.x;
            }
        }else{
            if (xDis * t > 0)
            {
                yDis = MyGame::DEFAULT_STANDARD_HEIGHT - beginPosition.y;
            }else{
                yDis = 0 - beginPosition.y;
            }
        }
    }
    float xLen = xDis / c;
    float yLen = yDis / s;
    float len = abs(xLen) > abs(yLen) ? yLen : xLen;
    len = len > 0 ? len + 1 : len - 1;
    CCPoint targetPoint = ccp(beginPosition.x + len * c, beginPosition.y + len * s);
    
    slope = atan2((targetPoint.y - beginPosition.y), (targetPoint.x - beginPosition.x));
    float degree = CC_RADIANS_TO_DEGREES(slope);
    sprite->setRotation(180 - degree - 90);

    float duration = abs(len) / BOMB_BASE_SPEED;
//     const int cannonLen = 70;
//     float addY = cannonLen * sin(slope);
//     float addX = cannonLen * cos(slope);
    //     float y = MyGame::DEFAULT_STANDARD_HEIGHT;
    //     float x = y * direction.x / direction.y;
    //     CCPoint moveBy = ccp(x, y);
    // 
    CCMoveTo *moveTo = CCMoveTo::create(duration,targetPoint);
    CCAction* action = CCSequence::create(moveTo, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::bombMoveActionEnd)), NULL);
    //FishRoute* line = FishRoute::createLine(BOMB_BASE_SPEED,  moveBy, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::bombMoveActionEnd)), -90);
    sprite->runAction(action);  
}

void SpriteLayer::fishDeadActionEnd(cocos2d::CCNode *sender){
    B2Sprite *fish = dynamic_cast<B2Sprite*>(sender->getParent());
    fish->setIsAlive(false);
}

void SpriteLayer::goldMoveActionEnd(cocos2d::CCNode *sender){
    MyGame::getInstance()->playerSoundEffect(GAIN_FISH_GOLD_MUSIC_PATH,false,true);
    this->removeChild(sender,true);
}

void SpriteLayer::spritePromptMoveActionEnd(cocos2d::CCNode *sender){
    this->removeChild(sender, true);
}

void SpriteLayer::changeSceneActionEnd(cocos2d::CCNode *sender){
    this->removeChild(sender, true);
    m_waitChangeBgEnd = false;
    m_waitChangeScene=false;
    this->createChangeSceneFormation();
}
// 对于linearRoute方法来说，改变方向十分简单：由于只是简单的直线运动，我们只需要在一开始计算出鱼游动的方向，然后使用setRotate函数即可。我们可以使用类似于下面的代码来计算鱼旋转的角度：
// 
// 
//     float rotate = -(exitY - enterY) / s.width;  
// fish->setRotation(rotate / 3.14159f * 180); 
// 而对于曲线运动，我们可以用CCSpawn对曲线动作与旋转动作进行包装，即可实现鱼一边沿曲线移动一边转动的效果，相关代码如下：
// 
// 
//     CCFiniteTimeAction *curveMove = CCSequence::create(  
//     CCMoveTo::create(dt / 2, ccp(s.width / 2 + r[!direction], h[direction])),  
//     CCSpawn::create(   
//     CCBezierTo::create(dt / 4, b1),  
//     CCRotateBy::create(dt / 4, -90)),  
//     CCSpawn::create(  
//     CCBezierTo::create(dt / 4, b1),  
//     CCBezierTo::create(dt / 4, b2)),  
//     CCMoveTo::create(dt / 2, to),  
//     NULL);  
// curveMove是使用CCSpawn包装后的曲线动作，我们用它替换掉前文bezierRoute方法中的move动作。它使鱼在第一段曲线上运动的同时，逆时针旋转90度，而在第二段曲线上运动的同时，顺时针旋转90度，虽然旋转不够精确，但是看起来自然多了。

// ccBezierConfig bezier;  
// bezier.controlPoint_1 = ccp(20, 150);  
// bezier.controlPoint_2 = ccp(200, 30);  
// bezier.endPosition = ccp(160, 30);  
// CFiniteTimeAction * beizerAction = CCBezierTo::create(actualDuration / 4, bezier);  
//http://book.51cto.com/art/201305/394779.htm  4.5　变速动作

void SpriteLayer::fire(cocos2d::CCPoint targetPosition){
    if (m_fireRemainTime > 0)
    {
        return;
    }
    m_fireRemainTime = FIRE_INTERVAL_SECOND;

    CCPoint beginPosition = m_cannon->getPosition();
    float slope = atan2((targetPosition.y - beginPosition.y), (targetPosition.x - beginPosition.x));
    float degree = CC_RADIANS_TO_DEGREES(slope);
    int cannonIndex = m_curCannonIndex;
    if (!m_showLaserCannon)
    {
        int needGold = BULLET_LEVEL[m_curCannonIndex];
        if (MyGame::getInstance()->getGold()>=needGold)
        {
            MyGame::getInstance()->addGold(-needGold);
            this->refreshGold(MyGame::getInstance()->getGold());
        }else{
            pauseGame();

			MyStatisticsLog::m_payPlace = "addGoldButton";
			StoreScene* mallUI = StoreScene::create();
			this->addChild(mallUI, CONTROLER_BUTTON_Z_ORDER);
			GameScene::m_inst->m_spriteLayer->pauseGame();

            //MyStatisticsLog::m_payPlace = string("goldNotEnough");
            //GiftStoreScene* mallUI = GiftStoreScene::create();
            //this->addChild(mallUI, CONTROLER_BUTTON_Z_ORDER);
            //this->showPromptCenter(TipsDict::getInstance()->getTips(TipsDict::KEY_NOT_ENGOUH_GOLD).c_str(), 120);
            return;
        }
        MyGame::getInstance()->playerSoundEffect(BOMB_MUSIC_PATH,false,true);
    }else{
        m_fireRemainTime = 2.0f;
        m_showLaserCannon = false;
        cannonIndex = -1;
        this->removeChildByTag(LASER_AMN_TAG, true);
        this->removeChildByTag(LASER_WAIT_AMN_TAG, true);        
        this->m_rageValue = 0;
        this->refreshRageInfo();
        MyGame::getInstance()->playerSoundEffect(LASER_LAUNCH_MUSIC_PATH,false,true);
    }
    m_cannon->stopAction(m_cannonAction);
    m_cannon->runAction(m_cannonAction);

    B2Sprite* fishSprite = B2Sprite::create();
    CCSprite* showSprite = CCSprite::createWithTexture(m_currentBomb->getTexture(), m_currentBomb->getTextureRect());
    fishSprite->addShowAmn(showSprite);
    fishSprite->setData(cannonIndex);
    fishSprite->setAnchorPoint(ccp(0.5f,0.5f));
    fishSprite->setRotation(180 - degree - 90);  
    CCPoint direction = ccp(targetPosition.x-beginPosition.x, targetPosition.y-beginPosition.y);
    m_box2dHandler->addBall(fishSprite, direction);
    fishSprite->setTag(BALL_TAG);

    const int cannonLen = 70;
    float addY = cannonLen * sin(slope);
    float addX = cannonLen * cos(slope);
    beginPosition.x += addX;
    beginPosition.y += addY;
    fishSprite->setPosition(beginPosition);
    this->setBombRoute(true, fishSprite);

    CCArmature *armature = CCArmature::create(BOMB_EFFECT_AMN_NAME);
    armature->getAnimation()->playWithIndex(m_curCannonIndex < 10 ? 0 : 1);
    armature->setPosition(fishSprite->getContentSize().width/2, -fishSprite->getContentSize().height/2);
    fishSprite->addChild(armature);
//    armature->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&GameScene::generalAmnEndCallBack));

    this->addChild(fishSprite);
    this->m_bombs->addObject(fishSprite);
//     char bombPath[80];  
//     sprintf(bombPath, BOMB_PIC_NAME, m_curCannonLevel);
//     CCSprite *bomb = CCSprite::create(bombPath);  
//     CCPoint beginPosition = m_cannon->getPosition();
//     bomb->setPosition(beginPosition);
//     bomb->setTag(BOMB_BEGIN_TAG + m_curCannonLevel);
//     this->addChild(bomb, 2);
// 
//     float degree = atan2((targetPosition.y - beginPosition.y), -(targetPosition.x - beginPosition.x));
//     degree = degree / 3.14159f * 180 - 90;
//     bomb->setRotation(degree);  
// 
//     CCMoveTo *move = CCMoveTo::create(0.5f, targetPosition);
//     CCAction* action = CCSequence::create(move, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::bombMoveActionEnd)), NULL);  
//     bomb->runAction(action);  
}

bool SpriteLayer::canCatchFish(bool isThunderFish, int fishGold, int bombGold){
    float prob = 0;
    if (isThunderFish)
    {
        prob = 10.0f * fishGold / bombGold;// 1/(10x分数/鱼炮等级)
    }else if (fishGold < 2000)
    {
        if (fishGold>=bombGold)
        {
            prob = 4.0f * fishGold / bombGold;//概率=1/(4x分数/鱼炮等级)
        }else{
            prob = ((3 + fishGold) * fishGold) / (2 * fishGold - 1 + (bombGold-1) * 0.1f) + (0.4f - fishGold * 0.01f);
        }
    }else{
        prob = 5000.0f / bombGold; //i.	概率=1/(5000/鱼炮等级)
    }
    const int sum = 10000;
    int probInt = (int)(1.0f / prob * sum);
    if (m_level <= 2 && fishGold < 2000)
    {
        probInt += 300;
    }
    return (abs(rand()) % sum) < probInt;
}

void SpriteLayer::catchThunderFish(B2Sprite *fish, int multiple){
    m_waitThunderFishes->removeAllObjects();
    m_fireRemainTime = 10000000;
    m_thunderFishAmnIndex = 0;
    m_thunderMultiple = multiple;
    m_thunderSumRewardGold = 0;
    for (unsigned int i=0;i<m_fishes->count();i++)
    {
        B2Sprite* otherFish = dynamic_cast<B2Sprite*>(m_fishes->objectAtIndex(i));
        int fishId = otherFish->getData();
        if (otherFish != fish && FISH_ID_IPHONE != fishId && !FishDict::getInstance()->getFishConfig(fishId)->isThunder 
            && otherFish->getIsAlive() && !otherFish->getIsDead() && MyGame::withinScreen(otherFish->getPositionX(), otherFish->getPositionY()))
        {
            CCPoint fishPos = otherFish->getPosition();
            unsigned int index = 0;
            for (; index < m_waitThunderFishes->count(); index++)//ii.	雷电按照从右向左从上向下进行连接
            {
                B2Sprite* temp = dynamic_cast<B2Sprite*>(m_waitThunderFishes->objectAtIndex(index));
                if (fishPos.x > temp->getPositionX() || (fishPos.x == temp->getPositionX() && fishPos.y > temp->getPositionY()))
                {
                    break;
                }
            }
            m_waitThunderFishes->insertObject(otherFish, index);
        }
    }

    int count = abs(rand()) % 7 + 5;
    int removeCount = m_waitThunderFishes->count() - count;
    while (removeCount > 0)
    {
        int index = abs(rand()) % m_waitThunderFishes->count();
        m_waitThunderFishes->removeObjectAtIndex(index);
        removeCount--;
    }
    m_waitThunderFishes->insertObject(fish, 0);
    for (unsigned int i = 0; i < m_waitThunderFishes->count(); i++)
    {
        B2Sprite* currentFish = dynamic_cast<B2Sprite*>(m_waitThunderFishes->objectAtIndex(i));
        currentFish->stopAllActions();
    }
    showThunderFishAmn(0);
}

void SpriteLayer::showThunderFishAmn(float duration){
    int sumCount = m_waitThunderFishes->count();
    if (m_thunderFishAmnIndex < sumCount)
    {
        B2Sprite* currentFish = dynamic_cast<B2Sprite*>(m_waitThunderFishes->objectAtIndex(m_thunderFishAmnIndex));
        if (!currentFish->getIsAlive() || currentFish->getIsDead())
        {
            m_thunderFishAmnIndex++;
            showThunderFishAmn(0);
            return;
        }
        currentFish->stopAllActions();
        if (m_thunderFishAmnIndex > 0)
        {
            B2Sprite *beforeFish = dynamic_cast<B2Sprite*>(m_waitThunderFishes->objectAtIndex(m_thunderFishAmnIndex-1));
            CCSprite *lightning = getLightning(ccp(currentFish->getPositionX()- beforeFish->getPositionX(), currentFish->getPositionY()- beforeFish->getPositionY()));
            lightning->setPosition(beforeFish->getPosition());
            lightning->setTag(THUNDER_AMN_BEGIN_TAG + m_thunderFishAmnIndex);
            this->addChild(lightning);   
            MyGame::getInstance()->playerSoundEffect(LIGHTNING_LINK_MUSIC_PATH, false,false);
        }
        if (0 != m_thunderFishAmnIndex)
        {
            CCSprite *ring = getLightningRing(currentFish->getContentSize().width);
            ring->setAnchorPoint(ccp(0.5f,0.5f));
            ring->setPosition(ccp(currentFish->getContentSize().width/2, currentFish->getContentSize().height/2));
            currentFish->addChild(ring);
        }

        m_thunderFishAmnIndex++;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SpriteLayer::showThunderFishAmn), this, 0.2f, false);
    }else{
        for (unsigned int i = 0; i < m_waitThunderFishes->count(); i++)
        {
            B2Sprite* currentFish = dynamic_cast<B2Sprite*>(m_waitThunderFishes->objectAtIndex(i));
            if (!currentFish->getIsAlive() || currentFish->getIsDead()){
                continue;
            }
            currentFish->setIsDead(true);
            const FishConfig *fishConfig = FishDict::getInstance()->getFishConfig(currentFish->getData());
            int rewardGold = 0;
            if (0 == m_thunderFishAmnIndex)
            {
                rewardGold = fishConfig->gold * m_thunderMultiple;
            }else{
                rewardGold = fishConfig->gold;
            }
            m_thunderSumRewardGold += rewardGold;
            MyGame::getInstance()->addGold(rewardGold);
            this->refreshGold(MyGame::getInstance()->getGold());
            this->m_exp += fishConfig->exp;
            this->refreshLevelInfo(false);
            this->m_rageValue += fishConfig->exp;
            refreshRageInfo();

            if (m_taskUI)
            {
                m_taskUI->catchFish(fishConfig->id);
                m_taskUI->gainGold(rewardGold);
            }
            showFishDeadSound(fishConfig->deadSound);
            showCatchFishGoldEffect(currentFish->getPosition(), rewardGold);
            currentFish->addShowAmn(loadFishSpriteFrame(fishConfig->dead_name.c_str(), fishConfig->dead_count, FISH_AMN_BEGIN_INDEX, false, 2));
            this->removeChildByTag(THUNDER_AMN_BEGIN_TAG + i);
        }
        showFishDeadTips(2, m_thunderSumRewardGold);
        m_gameDataDirty = true;

        m_waitThunderFishes->removeAllObjects();
        m_fireRemainTime = 0;
        m_thunderFishAmnIndex = 0;
        m_thunderMultiple = 0;
        m_thunderSumRewardGold = 0;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpriteLayer::showThunderFishAmn), this);
    }
}

void SpriteLayer::catchFish(B2Sprite *fish, int multiple){
    const FishConfig *fishConfig = FishDict::getInstance()->getFishConfig(fish->getData());
    if (fishConfig->id==FISH_ID_IPHONE)
    {
        MyGame::getInstance()->playerSoundEffect(IPHONE6_MUSIC_PATH,false,false);
    }
    int rewardGold = fishConfig->gold * multiple;
    MyGame::getInstance()->addGold(rewardGold);
    this->refreshGold(MyGame::getInstance()->getGold());
    this->m_exp += fishConfig->exp;
    this->refreshLevelInfo(false);
    this->m_rageValue += fishConfig->exp;
    refreshRageInfo();
    m_gameDataDirty = true;

    fish->stopAllActions();
    fish->setIsDead(true);
    fish->addShowAmn(loadFishSpriteFrame(fishConfig->dead_name.c_str(), fishConfig->dead_count, FISH_AMN_BEGIN_INDEX, false, 4));

    if (m_taskUI)
    {
        m_taskUI->catchFish(fishConfig->id);
        m_taskUI->gainGold(rewardGold);
    }
    showFishDeadSound(fishConfig->deadSound);
    showFishDeadTips(fishConfig->showDeadTips, rewardGold);
    showCatchFishGoldEffect(fish->getPosition(), rewardGold);
}

void SpriteLayer::showFishDeadSound(const string &deadSound){
    if (deadSound.length() > 0)
    {
        string path = "sound/" + deadSound;
        MyGame::getInstance()->playerSoundEffect(path.c_str(),false,false);
    }
}

void SpriteLayer::showFishDeadTips(int flag, int rewardGold){
    if (0 != flag)
    {
        string uiRewardPath="ui/game/jinbijiangli_1.json";
        Widget *widget = extension::GUIReader::shareReader()->widgetFromJsonFile(uiRewardPath.c_str());
        widget->setTag(Reward_Image_Tag);
        ImageView *rewardImage=dynamic_cast<ImageView*>(widget->getChildByName(Reward_Image_View));
        rewardImage->setScale(0);
        this->addChild(widget);
        CCScaleTo *maxScale=CCScaleTo::create(0.5f,1.0f);
        CCActionInterval * easeSineIn = CCEaseSineIn::create(maxScale);
        CCActionInterval * rewardDelay = CCDelayTime::create(1);
        CCScaleTo *minScale=CCScaleTo::create(0.5f,0);
        CCCallFunc *rewardCallFun=CCCallFunc::create(this,callfunc_selector(SpriteLayer::removeReward));
        CCSequence *rewardAction=CCSequence::create(easeSineIn,rewardDelay,minScale,rewardCallFun,NULL);
        rewardImage->runAction(rewardAction);

        char str[128] = {0};
        sprintf(str, "%d", rewardGold);
        LabelAtlas *rewardLabel=dynamic_cast<LabelAtlas*>(widget->getChildByName(Reward_Image_View)->getChildByName(Reward_Label_Atlas));
        rewardLabel->setStringValue(str);

        ImageView *rewardImageId=dynamic_cast<ImageView*>(widget->getChildByName(Reward_Image_View)->getChildByName("ImageView_23"));
        if(flag==1)
        {
            rewardImageId->loadTexture("ui/game/nizhenniu.png");  
            rewardImageId->setPosition(ccp(0,100));
            MyGame::getInstance()->playerSoundEffect(GREAT_MUSIC_PATH,false,false);
        }
        else if (flag==2)
        {
            rewardImageId->loadTexture("ui/game/nizhentainiu.png");
            rewardImageId->setPosition(ccp(0,120));
            MyGame::getInstance()->playerSoundEffect(MORE_GREAT_MUSIC_PATH,false,false);
        }
        else if (flag==3)
        {
            rewardImageId->loadTexture("ui/game/iPhone6Reward.png");
            rewardImageId->setPosition(ccp(0,50));
        }     
    }
}

void SpriteLayer::showCatchFishGoldEffect(CCPoint fishPos, int rewardGold){
    int goldNum;
    if (rewardGold<10)
    {
        goldNum=2;
    }
    else if (rewardGold<50)
    {
        goldNum=4;
    }
    else if (rewardGold<100)
    {
        goldNum=6;
    }
    else 
    {
        goldNum=8;
    }

    for(int i=0;i<goldNum;i++)
    {
        CCArmature *armature = CCArmature::create(GOLD_EFFECT_AMN_NAME);     
        armature->getAnimation()->playWithIndex(0);
        armature->setPosition(ccp(fishPos.x-30*i,fishPos.y+CCRANDOM_0_1()*70));
        this->addChild(armature);

        CCJumpBy * jumpTo = CCJumpTo::create(1.0f,ccp(armature->getPositionX(),armature->getPositionY()+50),50,1);
        CCMoveTo * moveTo = CCMoveTo::create(0.6f,ccp(armature->getPositionX(),armature->getPositionY()));
        CCMoveTo* movetTarget = CCMoveTo::create(0.6f, ccp(130,40));
        CCEaseSineOut* sineOut = CCEaseSineOut::create(movetTarget);
        CCAction* action = CCSequence::create(jumpTo, moveTo,  sineOut, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::goldMoveActionEnd)), NULL); 
        armature->runAction(action);
    }
    string xPath = GAME_SCENE_ROOT_PATH + "gold_x.png";
    CCSprite *goldSprite = CCSprite::create(xPath.c_str());
    char str[128] = {0};
    sprintf(str, "%d", rewardGold);
    string digitPath = GAME_SCENE_ROOT_PATH+"gold_digit.png";
    CCLabelAtlas *goldLabel = CCLabelAtlas::create(str, digitPath.c_str(), 27, 35, '0');
    goldLabel->setAnchorPoint(ccp(0,0.5f));
    goldLabel->setPosition(ccp(goldSprite->getContentSize().width, goldSprite->getContentSize().height / 2));
    goldSprite->setPosition(fishPos);
    goldSprite->addChild(goldLabel);
    goldSprite->setScale(0.5f);
    this->addChild(goldSprite);
    CCScaleTo *scaleTo = CCScaleTo::create(0.1f, 1.0f);
    CCEaseSineIn *sineIn = CCEaseSineIn::create(scaleTo);
    CCMoveTo *moveTo=CCMoveTo::create(0.5f,ccp(goldSprite->getPositionX(),goldSprite->getPositionY()+20));
    CCFadeOut *fadeOut=CCFadeOut::create(0.5f);
    CCAction *goldAction = CCSequence::create(sineIn,moveTo,fadeOut, CCCallFuncN::create(this, callfuncN_selector(SpriteLayer::goldMoveActionEnd)), NULL);
    goldSprite->runAction(goldAction);
}

void SpriteLayer::removeReward()
{
    this->removeChildByTag(Reward_Image_Tag);
}

void SpriteLayer::singleTouchEndsIn(CCPoint point)  
{ 
}  

bool SpriteLayer::singleTouchDirecting(CCPoint point)  
{
    if (m_showHelp)
    {
        m_showHelp = false;
        this->removeChildByTag(Help_Layer_Tag,true);
        this->resumeGame();
        MyGame::getInstance()->tryShowCheckinLayer(this);
        return false;
    }
    if (m_stop)
    {
        return false;  
    }
    CCPoint origin = m_cannon->getPosition();
    float slope = (float)atan2(point.y - origin.y, point.x - origin.x);
    float degree = CC_RADIANS_TO_DEGREES(slope);
    degree = 90 - degree;
    bool success = degree > -80 && degree < 80;
    if (success)
    {
        m_cannon->setRotation(degree);
        this->fire(point);
    }
    return success;
}  

void SpriteLayer::collisionEvent(B2Sprite *bullet, B2Sprite *fish){
    bullet->setIsAlive(false);
    if (isBall(bullet->getTag()))
    {
        B2Sprite* fishSprite = B2Sprite::create();
        int cannonIndex = bullet->getData();
        if (cannonIndex >= 0)
        {
            int connonLevel = BULLET_LEVEL[cannonIndex];
            int index = connonLevel < 10 ? connonLevel - 1 : 9;
            fishSprite->setContentSize(CCSizeMake(NET_LEVEL_RECT[index],NET_LEVEL_RECT[index]));
            fishSprite->setAnchorPoint(ccp(0.5f,0.5f));
            CCPoint beginPosition = bullet->getPosition();
            fishSprite->setPosition(beginPosition);

            m_box2dHandler->addNet(fishSprite);
            fishSprite->setTag(NET_TAG);
            fishSprite->setData(bullet->getData());
            CCArmature *armature = CCArmature::create("yuwang01-10");
            armature->getAnimation()->playWithIndex(index);
            armature->setPosition(fishSprite->getContentSize().width/2, fishSprite->getContentSize().height/2);
            armature->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&SpriteLayer::netMoveActionEnd));
            fishSprite->addChild(armature);
            this->addChild(fishSprite);
        }else{
            if (m_waitThunderFishes->count() <= 0)
            {
                refreshCannon();
                catchThunderFish(fish, 1);
                CCSprite *ring = getLightningRing(fish->getContentSize().width);
                ring->setAnchorPoint(ccp(0.5f,0.5f));
                ring->setPosition(ccp(fish->getContentSize().width/2, fish->getContentSize().height/2));
                fish->addChild(ring);
            }
        }
    }else if (fish->getIsAlive() && !fish->getIsDead() && NULL != fish->getB2Body())
    {
        const FishConfig *fishConfig = FishDict::getInstance()->getFishConfig(fish->getData());
        int bombGold = BULLET_LEVEL[bullet->getData()];
        int multiple = 1;
        if (bombGold > 10)
        {
            multiple = bombGold / 10;
            bombGold =10;
        }
        if (canCatchFish(fishConfig->isThunder, fishConfig->gold, bombGold))
        {
            if (fishConfig->isThunder)
            {
                catchThunderFish(fish, multiple);
            }else{
                catchFish(fish, multiple);
            }
        }
    }
}
void SpriteLayer::netMoveActionEnd(cocos2d::extension::CCArmature * armature, cocos2d::extension::MovementEventType eventType, const char *name){
    if (COMPLETE == eventType)
    {
        B2Sprite * fish = (B2Sprite *)armature->getParent();
        m_box2dHandler->removeSprite(dynamic_cast<B2Sprite*>(fish));
        this->removeChild(fish,true);      
    }
}

void SpriteLayer::touchedCallBack(CCObject* pSender,TouchEventType pType){
    switch(pType){
    case TOUCH_EVENT_BEGAN:
        break;
    case TOUCH_EVENT_MOVED:
        break;
    case TOUCH_EVENT_ENDED:
        {
            Widget *pNode = (Widget*)(pSender);
            cocos2d::CCLog("scheduler : SpriteLayer touchedCallBack (%s)", pNode->getName());
            if (0 == strcmp(pNode->getName(), CANNON_DECREASE_BUTTON_NAME))
            {
                if (!m_showLaserCannon&&!m_stop)
                {
                    m_curCannonIndex--;
                    if (m_curCannonIndex<0)
                    {
                        const LevelConfig *config = LevelDict::getInstance()->getLevelConfig(m_level);
                        m_curCannonIndex = config->bombCount-1;
                    }
                    MyGame::getInstance()->playerSoundEffect(SWITCH_CONNON_MUSIC_PATH,false,true);
                    this->refreshCannon();
                }
            }else if (0 == strcmp(pNode->getName(), CANNON_ADD_BUTTON_NAME))
            {
                if (!m_showLaserCannon&&!m_stop)
                {
                    m_curCannonIndex++;
                    const LevelConfig *config = LevelDict::getInstance()->getLevelConfig(m_level);
                    if (m_curCannonIndex>=config->bombCount)
                    {
                        m_curCannonIndex=0;
                    }
                    MyGame::getInstance()->playerSoundEffect(SWITCH_CONNON_MUSIC_PATH,false,true);
                    this->refreshCannon();
                }
            }else if ((0 == strcmp(pNode->getName(), GOLD_ADD_BUTTON_NAME))&&!m_stop)
            {
                MyStatisticsLog::m_payPlace="addGoldButton";
                StoreScene* mallUI = StoreScene::create();
                this->addChild(mallUI, CONTROLER_BUTTON_Z_ORDER);
                GameScene::m_inst->m_spriteLayer->pauseGame();
            }else if ((0 == strcmp(pNode->getName(), CONTROL_BAR_BUTTON_NAME))&&!m_showHelp)
            {
                if (m_stop)
                {
                    resumeGame();
                    turnControlBar(false);
                }else{
                    pauseGame();
                    turnControlBar(true);
                }
            }else if (0 == strcmp(pNode->getName(), CONTROL_MAIN_PAGE_BUTTON_NAME))
            {
                MyGame::getInstance()->changeToMenuScene();
            }else if (0 == strcmp(pNode->getName(), CONTROL_MALL_BUTTON_NAME))
            {
                MyStatisticsLog::m_payPlace="controlMallButton";
                StoreScene* mallUI = StoreScene::create();
                this->addChild(mallUI, CONTROLER_BUTTON_Z_ORDER);
                turnControlBar(false);
            }else if (0 == strcmp(pNode->getName(), MAIN_CONTROL_MALL_BUTTON_NAME))
            {
                pauseGame();
                MyStatisticsLog::m_payPlace="mainMallButton";
                StoreScene* mallUI = StoreScene::create();
                this->addChild(mallUI, CONTROLER_BUTTON_Z_ORDER);
                turnControlBar(false);
            }
            else if (0 == strcmp(pNode->getName(), CONTROL_GIFT_BUTTON_NAME))
            {
                MyStatisticsLog::m_payPlace="giftButton";
                MiniGiftStoreScene *mallUI = MiniGiftStoreScene::create();
                this->addChild(mallUI, CONTROLER_BUTTON_Z_ORDER);
                turnControlBar(false);
            }
            else if (0 == strcmp(pNode->getName(), CONTROL_SOUND_BUTTON_NAME))
            {
                MyGame::getInstance()->setSoundIsOn(!MyGame::getInstance()->getSoundIsOn());
                refreshSoundButton();
            }else if (0 == strcmp(pNode->getName(), CONTROL_HELP_BUTTON_NAME))
            {
                if (!m_showHelp)
                {
                    this->turnControlBar(false);
                    m_showHelp = true;
                    this->helpInterfaceinit();
                }
            }
            else if ((0 == strcmp(pNode->getName(),ONLINE_REWARD_BUTTON))&&!m_stop)
            {
                if (m_waitTime==0)
                {
                    this->onlineRewardEnd(true);
                }
            }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            else if (0 == strcmp(pNode->getName(), TEST_AD_BUTTON))
            {
                JniMethodInfo minfoStore;//定义Jni函数信息结构体 
                if (!JniHelper::getStaticMethodInfo(minfoStore,"skt/board/fish/Fish","adMoreGame", "()V")) { 
                    CCLog("jni:not have adMoreGame"); 
                }else{ 
                    CCLog("jni:have adMoreGame"); 
                    minfoStore.env->CallStaticVoidMethod(minfoStore.classID, minfoStore.methodID);
                    minfoStore.env->DeleteLocalRef(minfoStore.classID);
                }
            }
#endif
            break;
        }
    case TOUCH_EVENT_CANCELED:
        break;
    default:
        break;
    }
}

void SpriteLayer::showGoldAmn(){
    CCArmature *armature = CCArmature::create("jinbidiaoluo");
    armature->getAnimation()->playWithIndex(0);
    armature->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/2, MyGame::DEFAULT_STANDARD_HEIGHT/2+90));
    armature->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&SpriteLayer::showGoldAmnEnd));
    this->addChild(armature, CONTROLER_BUTTON_Z_ORDER);
}

void SpriteLayer::showGoldAmnEnd(cocos2d::extension::CCArmature * armature, cocos2d::extension::MovementEventType eventType, const char *name){
    if (COMPLETE == eventType)
    {
        removeChild(armature);
    }
}

void SpriteLayer::laserAmnEnd(cocos2d::extension::CCArmature * armature, cocos2d::extension::MovementEventType eventType, const char *name){
    if (COMPLETE == eventType)
    {
        //m_laserHarmRemainTime = 0;
        removeChild(armature);
        refreshCannon();
    }
}
