#include "AppDelegate.h"
#include "myGame.h"
#include "cocos2d.h"
#include "menuScene.h"
#include "bezierTest.h"
#include "SimpleAudioEngine.h"
#include "testinAdapter.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::cmdSchedule(float interval){
}

bool AppDelegate::applicationDidFinishLaunching() {
    MyGame::getInstance()->requestInit();
    MyStatisticsLog::sendFinishLaunchingLog();
    //TestinAdapter::init(MyGame::getInstance()->m_testinAppKey.c_str(), MyGame::getInstance()->m_testinChannelId.c_str());
    cocos2d::CCLog("applicationDidFinishLaunching");
    MyGame::getInstance()->initSoundInfo();
    cocos2d::CCLog("applicationDidFinishLaunching1");
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    cocos2d::CCLog("applicationDidFinishLaunching1.1");
    //pDirector->getScheduler()->scheduleSelector(schedule_selector(AppDelegate::cmdSchedule), this, 0, false);
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    cocos2d::CCLog("applicationDidFinishLaunching1.2");

    pDirector->setOpenGLView(pEGLView);
    cocos2d::CCLog("applicationDidFinishLaunching2");
    // Set the design resolution
    pEGLView->setDesignResolutionSize(MyGame::DEFAULT_STANDARD_WIDTH, MyGame::DEFAULT_STANDARD_HEIGHT, kResolutionShowAll);
    cocos2d::CCLog("applicationDidFinishLaunching3");
    //CCSize frameSize = pEGLView->getFrameSize();使用多套资源的时候，先根据分辨率选择资源
    //CCFileUtils::sharedFileUtils()->setResourceDirectory(smallResource.directory);//设置对应的资源目录
    //pDirector->setContentScaleFactor(smallResource.size.height/designResolutionSize.height);//计算当前资源与标准尺寸的缩放比
    
    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 35);
    cocos2d::CCLog("applicationDidFinishLaunching4");
    pDirector->startAnimation();
    cocos2d::CCLog("applicationDidFinishLaunching5");

    // create a scene. it's an autorelease object
    cocos2d::CCLog("try create login scene");
    CCScene *menuScene = MenuScene::scene();
    //CCScene *menuScene = BezierTest::scene();
    pDirector->pushScene(menuScene);
    cocos2d::CCLog("push login scene");
    //CCScene *scene = MainMenu::scene(); 
    //pDirector->pushScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    cocos2d::CCLog("applicationDidEnterBackground");
    MyStatisticsLog::sendBackgroundLog();
    //TestinAdapter::onStop();
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
     CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    cocos2d::CCLog("applicationWillEnterForeground");
    MyStatisticsLog::sendForegroundLog();
    //TestinAdapter::onStart();
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    if (MyGame::getInstance()->getSoundIsOn())
    {
         CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
         CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
}
