#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "myGame.h"
#include "SimpleAudioEngine.h"
#include "statisticsLog.h"
#include <string>

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    StatisticsLog log;
    std::string serverPath("http://ddz.geiliyou.com:10017/fishLog/newLog");//("http://192.168.254.76:8080/gm/newLog");
    std::string baseInfo("&appId=skt.board.fish&channelId=&version=2.3&phoneBrand=win32&phoneType=xp&systemVersion=xp&imsi=&key=1&operatorId=-1");
    std::string verifyInfo("skt.board.fish1");//appid+key
    log.init(serverPath, baseInfo, verifyInfo);
    log.sendLogData("winB","","");
    // create the application instance
    MyGame myGame;
    myGame.init();


    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("fish");
    eglView->setFrameSize(MyGame::DEFAULT_STANDARD_WIDTH, MyGame::DEFAULT_STANDARD_HEIGHT);
    int ret = CCApplication::sharedApplication()->run();
    CocosDenshion::SimpleAudioEngine::end();
    return ret;
}
