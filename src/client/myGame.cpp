#include "myGame.h"
#include "statisticsLog.h"
#include "myUtilsConfig.h"
#include "platform/CCCommon.h"
#include "gameScene.h"
#include "SimpleAudioEngine.h"
#include "configMacro.h"
#include <sstream>
#include <fstream>
#include "platform/CCFileUtils.h"
#include "exchangeDict.h"
#include "storeScene.h"
#include "menuScene.h"
#include "support/user_default/CCUserDefault.h"
#include "CommonFunc.h"
#include "checkinLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#else 
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "TalkingDataAppCpa.h"
#endif
#endif 

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;
MyGame* MyGame::m_inst = 0;

static const char* DTAT_OWN_GOLD_COUNT_KEY = "ownGoldCount";
static const char* CHECKIN_LAST_DAY_KEY = "checkinLastDay";
static const char* CHECKIN_COUNT_KEY = "checkinCount";
static const int DEFAULT_GOLD_COUNT = 400;
static const string GAME_DATA_FILE_NAME = string("singleGameData.dat");

static const int CHECKIN_MAX_DATE = 7;
static const int CHECKIN_REWARD_INFO[] = {100,160,220,280,350,420,480};

int MyGame::init(){
    cocos2d::CCLog("my game init");
    m_inst = this;

    m_gameDataPath = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + GAME_DATA_FILE_NAME;

    cocos2d::CCLog("Read main log info successfully!");
    m_gold = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(DTAT_OWN_GOLD_COUNT_KEY, DEFAULT_GOLD_COUNT);
    m_lastCheckinDay = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(CHECKIN_LAST_DAY_KEY, 0);
    m_checkinCount = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(CHECKIN_COUNT_KEY, 0);

    m_lastCanTerrtupted = true;
    m_lastSoundId = 0;
    //背景音乐
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(BG_MUSIC_PATH_1);
//     SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(BG_MUSIC_PATH_2);
//     SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(FISH_FORMAITON_MUSIC_PATH_1);
//     SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(FISH_FORMAITON_MUSIC_PATH_2);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.8f);
    //音效
    SimpleAudioEngine::sharedEngine()->preloadEffect(BOMB_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(GAIN_FISH_GOLD_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(LASER_WAIT_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(LASER_LAUNCH_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(LEVEL_UP_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(LIGHTNING_LINK_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(REWARD_GOLD_DOWN_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(IPHONE6_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(GREAT_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(MORE_GREAT_MUSIC_PATH);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
    
    m_isInitChannelSdk = false;
    m_storeId = 0;
    return 0;
}

void MyGame::MyGame_(){
    //reset (1) here
    //m_serverIp = "192.168.254.225";
    //m_serverPort = 11001;
    //set (2) zero here
}

int MyGame::MyGame__(){
    // release sys resource here
    // delete (2) here
    MyGame_();
    return 0;
}

void MyGame::requestInit(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
    {
        JniMethodInfo minfoStore;//定义Jni函数信息结构体 
        bool isHave = JniHelper::getStaticMethodInfo(minfoStore,"skt/board/fish/Fish","getStoreId", "()I");  

        if (!isHave) { 
            CCLog("jni:not have getStoreId"); 
        }else{ 
            CCLog("jni:have getStoreId"); 
            jint sid = minfoStore.env->CallStaticIntMethod(minfoStore.classID, minfoStore.methodID);
            //调用此函数 
            m_storeId = sid;
            minfoStore.env->DeleteLocalRef(minfoStore.classID);
        } 
        CCLog("jni-java getStoreId run end:%d", m_storeId); 
    }
    {
        JniMethodInfo minfo;//定义Jni函数信息结构体 
        if (!JniHelper::getStaticMethodInfo(minfo,"channel/Testin","getAppKey", "()Ljava/lang/String;")) { 
            CCLog("jni:not have getTestinAppKey"); 
        }else{ 
            CCLog("jni:have getTestinAppKey");
            jstring result= (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);  
            m_testinAppKey = JniHelper::jstring2string(result);
            minfo.env->DeleteLocalRef(result);
            minfo.env->DeleteLocalRef(minfo.classID);
        } 
        CCLog("jni-java getStoreId run end:%d", m_storeId); 
    }
    {
        JniMethodInfo minfo;//定义Jni函数信息结构体 
        if (!JniHelper::getStaticMethodInfo(minfo,"channel/Testin","getChannelId", "()Ljava/lang/String;")) { 
            CCLog("jni:not have getTestinChannelId"); 
        }else{ 
            CCLog("jni:have getTestinChannelId"); 
            jstring result= (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);  
            m_testinChannelId = JniHelper::jstring2string(result);
            minfo.env->DeleteLocalRef(result);
            minfo.env->DeleteLocalRef(minfo.classID);
        } 
        CCLog("jni-java getStoreId run end:%d", m_storeId); 
    }
    {
        JniMethodInfo minfo;//定义Jni函数信息结构体 
        if (!JniHelper::getStaticMethodInfo(minfo,"skt/board/fish/Fish","getMoreGameNormalPng", "()Ljava/lang/String;")) { 
            CCLog("jni:not have getMoreGameNormalPng"); 
        }else{ 
            CCLog("jni:have getMoreGameNormalPng"); 
			jstring result= (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
            m_moreGameNormalPng = JniHelper::jstring2string(result);
            minfo.env->DeleteLocalRef(result);
            minfo.env->DeleteLocalRef(minfo.classID);
        } 
        CCLog("jni-java getMoreGameNormalPng run end:%d", m_storeId); 
    }
    {
        JniMethodInfo minfo;//定义Jni函数信息结构体 
        if (!JniHelper::getStaticMethodInfo(minfo,"skt/board/fish/Fish","getMoreGameClickedPng", "()Ljava/lang/String;")) { 
            CCLog("jni:not have getMoreGameClickedPng"); 
        }else{ 
            CCLog("jni:have getMoreGameClickedPng"); 
			jstring result= (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
            m_moreGameClickedPng = JniHelper::jstring2string(result);
            minfo.env->DeleteLocalRef(result);
            minfo.env->DeleteLocalRef(minfo.classID);
        } 
        CCLog("jni-java getMoreGameClickedPng run end:%d", m_storeId); 
    }
#endif 
}

int MyGame::handleExitToLogonUIEvent()
{
    return 0;
}

void MyGame::initSoundInfo(){
    bool soundIsOn = cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey(MUSIC_SWITCH, true);
    this->setSoundIsOn(soundIsOn);
    playBackgroundMusic(false);
}

void MyGame::setGold(int gold){
    m_gold = gold;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(DTAT_OWN_GOLD_COUNT_KEY, gold);
}

void MyGame::addGold(int gold){
    setGold(m_gold+gold);
}

int MyGame::getGold(){
    return m_gold;
}

bool MyGame::isFileExist(const char *path)
{
    FILE *fp = fopen(path, "r");
    bool bRet = false;

    if (fp)
    {
        bRet = true;
        fclose(fp);
    }

    return bRet;
}

void MyGame::saveGameData(int level, int exp, int rageValue, int currentDay, int currentDayRunTimes, int lastId,int taskCurrentSuccessCount,float taskCurrentRemainTime,bool firstGame,int orderId,float waitTime){
     ofstream ofs(m_gameDataPath.c_str());
     if (ofs.is_open())
     {
        ofs<<level<<" "<<exp<<" "<<rageValue<<" "<<currentDay<<" "<<currentDayRunTimes<<" "<<lastId<<" "<<taskCurrentSuccessCount<<" "<<taskCurrentRemainTime<<" "<<firstGame<<" "<<orderId<<" "<<waitTime;
        ofs.close();
    }
}

void MyGame::tryShowCheckinLayer(CCNode *parent){
    int currentDay = CommonFunc::getCurrentDay();
    if (m_lastCheckinDay != currentDay)
    {
        if (0 != m_checkinCount)
        {
            int ld = m_lastCheckinDay % 100;
            int lm = m_lastCheckinDay / 100 % 100;
            int ly = m_lastCheckinDay / 10000;
            int cd = currentDay % 100;
            int cm = currentDay / 100 % 100;
            int cy = currentDay / 10000;
            bool needReset = true;
            if (ly == cy)
            {
                if (lm == cm)
                {
                    needReset = (ld + 1) != cd;
                }else if(lm + 1 == cm){
                    int lmMaxDayCount = 0;
                    switch(lm){
                    case 1:
                    case 3:
                    case 5:
                    case 7:
                    case 8:
                    case 10:
                    case 12:
                        lmMaxDayCount = 31;
                        break;
                    case 2:
                        if (ly % 400 == 0 || (ly % 100 != 0 && ly % 4 == 0))
                        {
                            lmMaxDayCount = 29;
                        }else{
                            lmMaxDayCount = 28;
                        }
                        break;
                    default:
                        lmMaxDayCount = 30;
                        break;
                    }
                    needReset = ld != lmMaxDayCount || cd != 1;
                }else{
                    needReset = true;
                }
            }else{
                needReset = lm != 12 || ld != 31 || cm !=1 || cd != 1;
            }
            if (needReset)
            {
                m_checkinCount = 0;
            }
        }
        CheckinLayer* layer = CheckinLayer::create(m_checkinCount+1, CHECKIN_MAX_DATE, CHECKIN_REWARD_INFO[m_checkinCount]);
        parent->addChild(layer, 100);
    }
}

void MyGame::getCheckinReward(){
    int currentDay = CommonFunc::getCurrentDay();
    if (m_lastCheckinDay != currentDay){
        MyGame::getInstance()->playerSoundEffect(REWARD_GOLD_DOWN_MUSIC_PATH,false,false);
        this->addGold(CHECKIN_REWARD_INFO[m_checkinCount]);
        GameScene::m_inst->m_spriteLayer->refreshGold(m_gold);
        GameScene::m_inst->m_spriteLayer->showGoldAmn();
        m_checkinCount++;
        if (m_checkinCount>=CHECKIN_MAX_DATE)
        {
            m_checkinCount = 0;
        }
        m_lastCheckinDay = currentDay;
        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(CHECKIN_LAST_DAY_KEY, m_lastCheckinDay);
        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(CHECKIN_COUNT_KEY, m_checkinCount);
    }
}

void MyGame::changeToGameScene(){
    MyStatisticsLog::sendLoginLog();
    cocos2d::CCLog("scheduler b: changeToGameScene");
    int level = 1;
    int exp=0;
    int rageValue=0;
    int currentDay = -1;
    int currentDayRunTimes =0;
    int lastId = -1;
    int taskCurrentSuccessCount=0;
    float taskCurrentRemainTime=0;
    bool firstGame = true;
    int orderId = 0;
    float waitTime = 0;
    if (isFileExist(m_gameDataPath.c_str()))
    {
        ifstream ifs(m_gameDataPath.c_str());
        if (ifs.is_open())
        {
            ifs>>level>>exp>>rageValue>>currentDay>>currentDayRunTimes>>lastId>>taskCurrentSuccessCount>>taskCurrentRemainTime>>firstGame>>orderId>>waitTime;
            ifs.close();
        }
    }
    // 清理ＧＵＩ管理类（这个不要放到最后清理）  
    cocos2d::extension::GUIReader::purge(); 
    // 清理动作（这里指Action动作，不是动作编辑器的动作。）  
    cocos2d::extension::ActionManager::purge();  
    // 清理数据管理  
    cocos2d::extension::DictionaryHelper::purgeDictionaryHelper();  
    // 释放掉不用的纹理   
    cocos2d::CCTextureCache::sharedTextureCache()->removeAllTextures();  
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    cocos2d::CCScene *scene = GameScene::scene(level, exp, rageValue, currentDay, currentDayRunTimes, lastId,taskCurrentSuccessCount,taskCurrentRemainTime,firstGame,orderId,waitTime);
    cocos2d::CCDirector::sharedDirector()->replaceScene(scene);
//    playerSoundEffect(READY_EFFECT_PATH);
    cocos2d::CCLog("scheduler e: changeToGameScene");
}

void MyGame::changeToMenuScene(){
    MyStatisticsLog::sendLogoutLog();
    cocos2d::CCLog("scheduler b: changeToMenuScene");
    GameScene::m_inst->m_spriteLayer->m_stop = true;
    // 清理ＧＵＩ管理类（这个不要放到最后清理）  
    cocos2d::extension::GUIReader::purge();
    // 清理动作（这里指Action动作，不是动作编辑器的动作。）  
    cocos2d::extension::ActionManager::purge();  
    // 清理数据管理  
    cocos2d::extension::DictionaryHelper::purgeDictionaryHelper();  
    // 释放掉不用的纹理   
    cocos2d::CCTextureCache::sharedTextureCache()->removeAllTextures();  
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCDirector::sharedDirector()->replaceScene(MenuScene::scene());
    cocos2d::CCLog("scheduler e: changeToMenuScene");
}

void MyGame::setSoundIsOn(bool soundIsOn){
    m_soundIsOn = soundIsOn;
    cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey(MUSIC_SWITCH, soundIsOn);
    if (soundIsOn)
    {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }else
    {
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
}
bool MyGame::getSoundIsOn(){
    return m_soundIsOn;
}
void MyGame::playBackgroundMusic(bool formation){
    if (!m_soundIsOn)
    {
        return;
    }
    char * path;
    if (0 == (abs(rand()) % 2))
    {
        if (formation)
        {
            path = FISH_FORMAITON_MUSIC_PATH_1;
        }else{
            path = BG_MUSIC_PATH_1;
        }
    }else{
        if (formation)
        {
            path = FISH_FORMAITON_MUSIC_PATH_2;
        }else{
            path = BG_MUSIC_PATH_2;
        }
    }
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(path, true);
}
//播放音效
void MyGame::playerSoundEffect(const char * soundEffectPath, bool loop,bool isTerrtupted)
{
    if (m_soundIsOn)
    {
        if (!m_lastCanTerrtupted && SimpleAudioEngine::sharedEngine()->getEffectIsPlaying(m_lastSoundId))
        {
            return;
        }
        else
        {
            m_lastSoundId = SimpleAudioEngine::sharedEngine()->playEffect(soundEffectPath,false);
            m_lastCanTerrtupted = isTerrtupted;
        }
    }
}

void MyGame::paySuccess(int rmb, const char* orderId){
    MyStatisticsLog::sendPaySuccessLog(rmb, orderId);
    const ExchangeInfo* exchangeInfo = ExchangeDict::getInstance()->getStoreInfo(MyGame::getInstance()->m_storeId)->getExchangeInfo(rmb);
    int addedGold = 0;
    if (NULL != exchangeInfo)
    {
        addedGold = exchangeInfo->gold + exchangeInfo->presentGold;
    }else if(10 == rmb){
        addedGold = 10;
    }else{
        return;
    }
    int newGold = MyGame::getInstance()->getGold() + addedGold;
    MyGame::getInstance()->setGold(newGold);
    if (10 == rmb)
    {
        //             cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(GAIN_NEWBIE_GIFT, NEWBIE_GIFT_IS_GAIN);
        //             MenuScene::removeNewbieGift();
    }
    StoreScene::updateMyGoldShow();
    GameScene::updateGoldShow(newGold);
}

void MyGame::payFailed(int rmb, const char *reason){
    MyStatisticsLog::sendPayFailedLog(rmb, reason);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
extern "C" 
{
    void Java_channel_Pay_paySuccess(JNIEnv* env, jobject obj, jint rmb, jstring orderId){
        char *str = (char*)env->GetStringUTFChars(orderId, NULL); 
        MyGame::getInstance()->paySuccess(rmb, str);
        env->ReleaseStringUTFChars(orderId, str);
    }
    void Java_channel_Pay_payFail(JNIEnv* env, jobject obj, jint rmb, jstring context){
        char *str = (char*)env->GetStringUTFChars(context, NULL); 
        MyGame::getInstance()->payFailed(rmb, str);
        env->ReleaseStringUTFChars(context, str);
    }
}
#else 
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
[TalkingDataAppCpa onPay:@" Your_userId " 
withOrderId:@"your_orderId"  withAmount:@"order_amount" 
withCurrency:@"CNY" ];
#endif
#endif 

string MyStatisticsLog::m_payPlace="";

void MyStatisticsLog::sendFinishLaunchingLog(){
    const char *action = "finishLaunching";
    StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendForegroundLog(){
    const char *action = "foreground";
    StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendBackgroundLog(){
    const char *action = "background";
    StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendLoginLog(){
    const char *action = "login";
    StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendLogoutLog(){
    const char *action = "logout";
    StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendCheckinLog(){
    const char *action = "checkin";
    StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendPayLog(int rmb){
    const char *action = "pay";
    char rmbStr[64]={0};
    sprintf(rmbStr,"%d",rmb);
    StatisticsLog::getInstance()->sendLogData(action, rmbStr, m_payPlace.c_str());

    m_payPlace = "";
}

void MyStatisticsLog::sendPaySuccessLog(int rmb, const char *orderId){
    const char *action = "paySuccess";
    char str[64] = {0};
    sprintf(str, "%d", rmb);
    StatisticsLog::getInstance()->sendLogData(action, str, orderId);
}

void MyStatisticsLog::sendPayFailedLog(int rmb, const char *reason){
    const char *action = "payFailed";
    char str[64] = {0};
    sprintf(str, "%d", rmb);
    StatisticsLog::getInstance()->sendLogData(action, str, reason);
}

void MyStatisticsLog::sendLevelChangeLog(int level){
    const char *action = "level";
    char str[64] = {0};
    sprintf(str, "%d", level);
    StatisticsLog::getInstance()->sendLogData(action, str);
}