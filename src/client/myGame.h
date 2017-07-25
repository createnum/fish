#ifndef __MY_GAME_H__
#define __MY_GAME_H__
#include <vector>
#include <string>
#include "configMacro.h"
class ClientGameDesk;
namespace cocos2d{
    class CCNode;
}

class MyGame
{
public:
    static const int DEFAULT_STANDARD_WIDTH = 800;//组件适配的默认基准屏幕宽度(用于组件在不同屏幕的自适应宽度)
    static const int DEFAULT_STANDARD_HEIGHT = 480;//组件适配的默认基准屏幕高度(用于组件在不同屏幕的自适应宽度)
    static const bool SHOW_TEST_AD_BUTTON = false;
    static bool withinScreen(int x, int y){
        return x > 0 && y > 0 && x < DEFAULT_STANDARD_WIDTH && y < DEFAULT_STANDARD_HEIGHT;
    }
    static MyGame* getInstance(){
        return m_inst;
    }
    MyGame(){
        MyGame_();
    }
    ~MyGame(){
        MyGame__();
    }
    int init();
    int handleExitToLogonUIEvent();
    static bool isFileExist(const char *path);
    void saveGameData(int level, int exp, int rageValue, int currentDay, int currentDayRunTimes, int lastId,int taskCurrentSuccessCount,float taskCurrentRemainTime,bool firstGame,int orderId,float waitTime);
    void changeToGameScene();
    void changeToMenuScene();
    void tryShowCheckinLayer(cocos2d::CCNode* parent);
    void getCheckinReward();
protected://simple object(including unliable pointer) - (1) liable pointer - (2) complex object - (3) father class - (4)
    void MyGame_();
    int MyGame__();
protected:
    static MyGame* m_inst;
    std::string m_gameDataPath;
    int m_lastCheckinDay;
    int m_checkinCount;
    int m_gold;
    bool m_soundIsOn;
    bool m_isInitChannelSdk;
    bool m_lastCanTerrtupted;
    unsigned int m_lastSoundId;
public:
    std::string m_screenshotName;
    int m_storeId;
    std::string m_testinAppKey;
    std::string m_testinChannelId;
    std::string m_moreGameNormalPng;
    std::string m_moreGameClickedPng;
    void requestInit();
    void initSoundInfo();
    void setGold(int gold);
    void addGold(int gold);
    int getGold();
    void setSoundIsOn(bool soundIsOn);
    void playBackgroundMusic(bool formation=false);
    void playerSoundEffect(const char * soundEffectPath, bool loop,bool isTerrtupted);
    bool getSoundIsOn();
    void paySuccess(int rmb, const char* orderId);
    void payFailed(int rmb, const char *reason);
};

class MyStatisticsLog{
public:
    static std::string m_payPlace;
    static void sendFinishLaunchingLog();
    static void sendForegroundLog();
    static void sendBackgroundLog();
    static void sendLoginLog();
    static void sendLogoutLog();
    static void sendCheckinLog();
    static void sendPayLog(int rmb);
    static void sendPaySuccessLog(int rmb, const char *orderId);
    static void sendPayFailedLog(int rmb, const char *reason);
    static void sendLevelChangeLog(int level);
};

#endif//__MY_GAME_H__

