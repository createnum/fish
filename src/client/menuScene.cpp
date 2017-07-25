#include "menuScene.h"
#include "myGame.h"
#include "SimpleAudioEngine.h"
#include "configMacro.h"
#include "storeScene.h"
#include "storeScene.h"
#include "SpriteLayer.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif

using namespace cocos2d;
using namespace gui;
using namespace CocosDenshion;

#define UI_PATH                    "ui/menu/dengrujiem_1.ExportJson"
#define START_BUTTON_TAG_NAME    "Button_24"
#define SETTING_BUTTON_TAG_NAME    "Button_26"

static const char* SETTING_CLOSE_BUTTON = "Button_103";
static const char* SETTING_MUSIC_BUTTON = "Button_105";
static const char* SETTING_HELP_BUTTON = "Button_105_1";
static const char* SETTING_ABOUT_BUTTON = "Button_105_1_0";
static const char* ABOUT_001RMB_BUTTON = "Button_6_0";
static const char* ABOUT_01RMB_BUTTON = "Button_6_1";
static const char* ABOUT_CLOSE_BUTTON = "Button_6";
static const char* MORE_BUTTON = "Button_More";
static const char* TEST_AD_BUTTON = "Button_Ad_Test";

static const char* UUCUN_LINK_BUTTON = "Button_7";
static const char* UUCUN_MORE_BUTTON = "Button_7_0";

static const int ABOUT_WIDGET_TAG = 10000;
static const int HELP_TAG = 10001;

static const int BG_Z_ORDER = -2;
static const int BUBBLE_Z_ORDER = -1;
static const int DIALOG_Z_ORDER = 0;

CCScene* MenuScene::scene(){
	CCScene* scene = NULL;
	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		MenuScene* layer = MenuScene::create();
		scene->addChild(layer);

	} while (0);
	return scene;
}
bool MenuScene::init(){
	do{
		CC_BREAK_IF(!TouchGroup::init());
		m_exit = false;
		this->setKeypadEnabled(true);
		this->clear();
		CCSprite *bg = CCSprite::create("ui/menu/dengru.jpg");
		CC_BREAK_IF(!bg);
		bg->setAnchorPoint(ccp(0.5f, 0.5f));
		bg->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH / 2, MyGame::DEFAULT_STANDARD_HEIGHT / 2));
		bg->setScaleX(-1.0f);
		this->addChild(bg, BG_Z_ORDER);

		Widget *widget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PATH);
		CC_BREAK_IF(!widget);
		this->addWidget(widget);

		Button* button = dynamic_cast<Button*>(widget->getChildByName(START_BUTTON_TAG_NAME));
		CC_BREAK_IF(!button);
		button->addTouchEventListener((CCObject*)this, toucheventselector(MenuScene::touchedCallBack));

		button = dynamic_cast<Button*>(widget->getChildByName(SETTING_BUTTON_TAG_NAME));
		CC_BREAK_IF(!button);
		button->addTouchEventListener((CCObject*)this, toucheventselector(MenuScene::touchedCallBack));

		if (MyGame::getInstance()->m_moreGameNormalPng.length() > 0 && MyGame::getInstance()->m_moreGameClickedPng.length() > 0)
		{
			Button* moreButton = Button::create();
			moreButton->loadTextureNormal(MyGame::getInstance()->m_moreGameNormalPng.c_str());
			moreButton->loadTexturePressed(MyGame::getInstance()->m_moreGameClickedPng.c_str());
			moreButton->setName(MORE_BUTTON);
			moreButton->setPosition(ccp(700, 430));
			moreButton->setTouchEnabled(true);
			moreButton->addTouchEventListener((CCObject*)this, toucheventselector(MenuScene::touchedCallBack));
			widget->addChild(moreButton);


			
		}

		if (MyGame::SHOW_TEST_AD_BUTTON)
		{
			Button* moreButton = Button::create();
			moreButton->loadTextureNormal("ui/menu/ad.jpg");
			moreButton->loadTexturePressed("ui/menu/ad.jpg");
			moreButton->setName(TEST_AD_BUTTON);
			moreButton->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH / 2, 40));
			moreButton->setScale(0.7f);
			moreButton->setTouchEnabled(true);
			moreButton->addTouchEventListener((CCObject*)this, toucheventselector(MenuScene::touchedCallBack));
			widget->addChild(moreButton, 100);
		}

		//        if (STORE_UUCUN == MyGame::getInstance()->m_storeId)
		//        {
		//            button = dynamic_cast<Button*>(widget->getChildByName(UUCUN_MORE_BUTTON));
		//            CC_BREAK_IF(!button);
		//            button->addTouchEventListener((CCObject*)this, toucheventselector(MenuScene::touchedCallBack));
		// 
		//            button = dynamic_cast<Button*>(widget->getChildByName(UUCUN_LINK_BUTTON));
		//            CC_BREAK_IF(!button);
		//            button->addTouchEventListener((CCObject*)this, toucheventselector(MenuScene::touchedCallBack));
		//        }

		this->scheduleUpdate();
		m_addBubbleRemainTime = 0;
		return true;
	} while (0);
	return false;
}

void MenuScene::touchedCallBack(CCObject* pSender, TouchEventType pType){
	switch (pType){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
	{
		Widget *pNode = (Widget*)(pSender);
		cocos2d::CCLog("scheduler : MenuScene touchedCallBack (%s)", pNode->getName());
		if (0 == strcmp(pNode->getName(), START_BUTTON_TAG_NAME))
		{
			if (!m_exit)
			{
				m_exit = true;
				MyGame::getInstance()->changeToGameScene();
			}
		}
		else if (0 == strcmp(pNode->getName(), SETTING_BUTTON_TAG_NAME))
		{
			SettingLayer* layer = SettingLayer::create();
			this->addChild(layer, DIALOG_Z_ORDER);
		}
		else if (0 == strcmp(pNode->getName(), MORE_BUTTON))
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
			JniMethodInfo minfoStore;//定义Jni函数信息结构体 
			bool isHave = JniHelper::getStaticMethodInfo(minfoStore,"skt/board/fish/Fish","moreGame", "()V");  
			if (!isHave) { 
				CCLog("jni:not have moreGame"); 
			}else{ 
				CCLog("jni:have moreGame"); 
				minfoStore.env->CallStaticVoidMethod(minfoStore.classID, minfoStore.methodID);
				minfoStore.env->DeleteLocalRef(minfoStore.classID);
			}
#endif
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
		else if (0 == strcmp(pNode->getName(), UUCUN_MORE_BUTTON))
		{
			JniMethodInfo minfoStore;//定义Jni函数信息结构体 
			bool isHave = JniHelper::getStaticMethodInfo(minfoStore,"channel/UucunChannel","moreGame", "(I)V");  
			if (!isHave) { 
				CCLog("jni:not have moreGame"); 
			}else{ 
				CCLog("jni:have moreGame"); 
				minfoStore.env->CallStaticVoidMethod(minfoStore.classID, minfoStore.methodID, 0);
				minfoStore.env->DeleteLocalRef(minfoStore.classID);
			}
		}else if (0 == strcmp(pNode->getName(), UUCUN_LINK_BUTTON))
		{
			JniMethodInfo minfoStore;//定义Jni函数信息结构体 
			bool isHave = JniHelper::getStaticMethodInfo(minfoStore,"channel/UucunChannel","moreGame", "(I)V");  
			if (!isHave) { 
				CCLog("jni:not have moreGame"); 
			}else{ 
				CCLog("jni:have moreGame"); 
				minfoStore.env->CallStaticVoidMethod(minfoStore.classID, minfoStore.methodID, 1);
				minfoStore.env->DeleteLocalRef(minfoStore.classID);
			}
		}else if (0 == strcmp(pNode->getName(), TEST_AD_BUTTON))
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

void MenuScene::keyBackClicked(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
	JniMethodInfo minfoStore;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(minfoStore,"skt/board/fish/Fish","exitGame", "()V");  
	if (!isHave) { 
		CCLog("jni:not exitGame"); 
	}else{ 
		CCLog("jni:exitGame"); 
		minfoStore.env->CallStaticVoidMethod(minfoStore.classID, minfoStore.methodID);
		CCLog("jni:exitGame 2"); 
		minfoStore.env->DeleteLocalRef(minfoStore.classID);
		CCLog("jni:exitGame 3"); 
	}
	//cocos2d::CCLog("scheduler : MenuScene keyBackClicked end app");
	//CCDirector::sharedDirector()->end();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	//ScrollTips::showSimpleNote_Center("You pressed the close button. Windows Store Apps do not implement a close button.");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MenuScene::bubbleMoveActionEnd(cocos2d::CCNode* sender){
	this->removeChild(sender, true);
}

void MenuScene::update(float delta){
	this->m_addBubbleRemainTime -= delta;
	if (m_addBubbleRemainTime <= 0)
	{
		m_addBubbleRemainTime = 0.5f;
		for (int i = 0; i < 1; i++){
			CCSprite *sprite = CCSprite::create("ui/game/bubble.png");
			sprite->setScale((rand() % 100 + 50)*0.01f);
			CCSize size = sprite->getContentSize();
			sprite->setPosition(ccp(abs(rand() % (MyGame::DEFAULT_STANDARD_WIDTH - 40)) + 20, -size.height / 2));
			CCAction* action = CCSequence::create(CCMoveBy::create(5.0f, ccp(0, MyGame::DEFAULT_STANDARD_HEIGHT + size.height)), CCCallFuncN::create(this, callfuncN_selector(MenuScene::bubbleMoveActionEnd)), NULL);
			sprite->runAction(action);
			this->addChild(sprite, BUBBLE_Z_ORDER);
		}
	}
}

MenuScene::~MenuScene(void)
{
}

bool SettingLayer::init(){
	do{
		CC_BREAK_IF(!TouchGroup::init());
		Widget *widget = extension::GUIReader::shareReader()->widgetFromJsonFile("ui/game/shezhijiem_1.json");
		CC_BREAK_IF(!widget);
		this->addWidget(widget);

		Widget *pWidget = widget->getChildByName("ImageView_100");
		CC_BREAK_IF(!pWidget);
		Button *button = dynamic_cast<Button*>(pWidget->getChildByName(SETTING_CLOSE_BUTTON));
		CC_BREAK_IF(!button);
		button->addTouchEventListener((CCObject*)this, toucheventselector(SettingLayer::touchedCallBack));

		button = dynamic_cast<Button*>(pWidget->getChildByName(SETTING_MUSIC_BUTTON));
		CC_BREAK_IF(!button);
		button->addTouchEventListener((CCObject*)this, toucheventselector(SettingLayer::touchedCallBack));

		button = dynamic_cast<Button*>(pWidget->getChildByName(SETTING_HELP_BUTTON));
		CC_BREAK_IF(!button);
		button->addTouchEventListener((CCObject*)this, toucheventselector(SettingLayer::touchedCallBack));

		button = dynamic_cast<Button*>(pWidget->getChildByName(SETTING_ABOUT_BUTTON));
		CC_BREAK_IF(!button);
		button->addTouchEventListener((CCObject*)this, toucheventselector(SettingLayer::touchedCallBack));

		m_showHelp = false;
		refreshSoundButton(MyGame::getInstance()->getSoundIsOn());
		return true;
	} while (0);
	return false;
}

void SettingLayer::refreshSoundButton(bool isOn){
	Widget* pWidget = this->getWidgetByName("Panel_97")->getChildByName("ImageView_100");
	Button* button = dynamic_cast<Button*>(pWidget->getChildByName(SETTING_MUSIC_BUTTON));
	if (isOn)
	{
		const char* path = "ui/game/shezhijiem/shenykaig01.png";
		button->loadTextures(path, path, path);
	}
	else{
		const char* path = "ui/game/shezhijiem/shenykaig00.png";
		button->loadTextures(path, path, path);
	}
}

void SettingLayer::touchedCallBack(CCObject* pSender, TouchEventType pType){
	switch (pType){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
	{
		Widget *pNode = (Widget*)(pSender);
		cocos2d::CCLog("scheduler : SettingLayer touchedCallBack (%s)", pNode->getName());
		if (0 == strcmp(pNode->getName(), SETTING_CLOSE_BUTTON))
		{
			this->removeFromParentAndCleanup(true);
		}
		else if (0 == strcmp(pNode->getName(), SETTING_MUSIC_BUTTON))
		{
			bool isOn = MyGame::getInstance()->getSoundIsOn();
			isOn = !isOn;
			MyGame::getInstance()->setSoundIsOn(isOn);
			this->refreshSoundButton(isOn);
		}
		else if (0 == strcmp(pNode->getName(), SETTING_HELP_BUTTON)){
			if (!m_showHelp)
			{
				m_showHelp = true;
				CCLayerColor * helpLayer = CCLayerColor::create(ccc4(128, 128, 128, 125), 800, 480);
				this->addChild(helpLayer, 500, HELP_TAG);
				CCSprite *help = CCSprite::create("ui/game/help.jpg");
				//help->setScale(0.8f);
				help->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH / 2, MyGame::DEFAULT_STANDARD_HEIGHT / 2));
				helpLayer->addChild(help);
			}
		}
		else if (0 == strcmp(pNode->getName(), SETTING_ABOUT_BUTTON)){
			Widget *widget = extension::GUIReader::shareReader()->widgetFromJsonFile("ui/game/guangyu_2.json");
			widget->setTag(ABOUT_WIDGET_TAG);


			//关于界面添加 0.01  0.1元计费点 2016/5/30   修改人：宋兴
			{
				Button *button_001 = dynamic_cast<Button*>(widget->getChildByName(ABOUT_001RMB_BUTTON));
				button_001->addTouchEventListener((CCObject*)this, toucheventselector(SettingLayer::touchedCallBack));

				Button *button_01 = dynamic_cast<Button*>(widget->getChildByName(ABOUT_01RMB_BUTTON));
				button_01->addTouchEventListener((CCObject*)this, toucheventselector(SettingLayer::touchedCallBack));
			}


			this->addWidget(widget);
			Button *button = dynamic_cast<Button*>(widget->getChildByName(ABOUT_CLOSE_BUTTON));
			button->addTouchEventListener((CCObject*)this, toucheventselector(SettingLayer::touchedCallBack));



		}
		else if (0 == strcmp(pNode->getName(), ABOUT_CLOSE_BUTTON)){
			this->removeWidget(this->getWidgetByTag(ABOUT_WIDGET_TAG));
		}
		else if (0 == strcmp(pNode->getName(), ABOUT_001RMB_BUTTON)){
			MyStatisticsLog::sendPayLog(10);
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台 
			JniMethodInfo minfo;//定义Jni函数信息结构体 
			//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
			bool isHave = JniHelper::getStaticMethodInfo(minfo, "channel/Pay", "pay", "(I)V");

			if (!isHave) {
				CCLog("jni:此函数不存在");
			}
			else{
				CCLog("jni:此函数存在");
				//调用此函数 
				minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, 10);
				minfo.env->DeleteLocalRef(minfo.classID);
			}
			CCLog("jni-java函数执行完毕");
			#endif 
		}
		else if (0 == strcmp(pNode->getName(), ABOUT_01RMB_BUTTON)){
			MyStatisticsLog::sendPayLog(1);
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台 
			JniMethodInfo minfo;//定义Jni函数信息结构体 
			//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
			bool isHave = JniHelper::getStaticMethodInfo(minfo, "channel/Pay", "pay", "(I)V");

			if (!isHave) {
				CCLog("jni:此函数不存在");
			}
			else{
				CCLog("jni:此函数存在");
				//调用此函数 
				minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, 1);
				minfo.env->DeleteLocalRef(minfo.classID);
			}
			CCLog("jni-java函数执行完毕");
			#endif 
		}

		break;
	}
	case TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}
}

bool SettingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	bool ret = TouchGroup::ccTouchBegan(pTouch, pEvent);
	return true;
}

void SettingLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	if (m_showHelp)
	{
		m_showHelp = false;
		this->removeChildByTag(HELP_TAG);
		return;
	}
	else{
		TouchGroup::ccTouchEnded(pTouch, pEvent);
	}
}

SettingLayer::~SettingLayer(){

}
