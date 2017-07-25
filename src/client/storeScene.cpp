#include "storeScene.h"
#include "myGame.h"
#include "SimpleAudioEngine.h"
#include "exchangeDict.h"
#include "configMacro.h"
#include "gameScene.h"
#include "tipsDict.h"
#include <stdlib.h>


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 

using namespace cocos2d;
using namespace cocos2d::gui;
#define UI_PATH                    "ui/game/shangchengjiemian_1.ExportJson"
#define CLOSE_BUTTON_TAG_NAME    "Button_94"
#define ITEM_UI_PATH                    "ui/game/shangchengitem.ExportJson"
#define BUY_BUTTON_NAME         "Button_38_0_0"
#define UUCUN_BUY_BUTTON        "Button_11"

static const char *GIFT_UI_PATH = "ui/game/xianshiyh_1.ExportJson";
static const char *GIFT_CLOSE_BUTTON_NAME = "Button_94";
static const char *GIFT_CONFIRM_BUTTON_NAME = "Button_165";

static const char *MINI_GIFT_UI_PATH = "ui/game/yiyuanlibao_1_0.ExportJson";
static const char *MINI_GIFT_CLOSE_BUTTON_NAME = "Button_94";
static const char *MINI_GIFT_CONFIRM_BUTTON_NAME = "Button_165";

static const char *PAY_UI_PATH = "ui/game/zhifujiem0_1.ExportJson";
static const char *CONCEL_PAY_BUTTON ="Button_11_0";
/*static const char *UUCUN_BUY_BUTTON = "Button_11";*/

StoreScene* StoreScene::m_inst = 0;


bool StoreScene::init(){
    do{
        vector<ExchangeInfo*> exchangeInfos;
        ExchangeDict::getInstance()->getStoreInfo(MyGame::getInstance()->m_storeId)->getExchangeInfos(exchangeInfos);
        CC_BREAK_IF(!TouchGroup::init());
//         if (STORE_UUCUN == MyGame::getInstance()->m_storeId)
//         {
//             m_widget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PATH);
//             Label *label_1 = dynamic_cast<Label*>(m_widget->getChildByName("Label_12"));
//             CC_BREAK_IF(!label_1);
//             label_1->removeFromParentAndCleanup(true);
//             Label *label_2 = dynamic_cast<Label*>(m_widget->getChildByName("Label_13"));
//             label_2->removeFromParentAndCleanup(true);
//         }
//         else
        {
            m_widget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PATH);
            CC_BREAK_IF(!m_widget);
        }
        Button* closeButton = dynamic_cast<Button*>(m_widget->getChildByName(CLOSE_BUTTON_TAG_NAME));
        CC_BREAK_IF(!closeButton);
        closeButton->addTouchEventListener((CCObject*)this, toucheventselector(StoreScene::touchedCallBack)); 

        float itemStartX = 25;
        float itemStartY = 175;
        const int xStep = 125;
		ListView * itemListListView = dynamic_cast<ListView*>(m_widget->getChildByName("ListView_13"));
		CC_BREAK_IF(!itemListListView);
        for(unsigned int i=0;i<exchangeInfos.size();i++){
            ExchangeInfo *exchangeInfo = exchangeInfos[i];
            Widget *itemWidget = extension::GUIReader::shareReader()->widgetFromJsonFile(ITEM_UI_PATH);
			Widget *parentWidget = itemWidget->getChildByName("Panel_159");

			char info[80];
			sprintf(info, "%d", exchangeInfo->gold + exchangeInfo->presentGold);
			LabelAtlas* sumGold = dynamic_cast<LabelAtlas*>(parentWidget->getChildByName("LabelAtlas_170"));
			sumGold->setStringValue(info);

			sprintf(info, "%d", exchangeInfo->gold);
			Label* goldLabel = dynamic_cast<Label*>(parentWidget->getChildByName("Label_52_0"));
			goldLabel->setText(info);

			if (exchangeInfo->presentGold > 0)
			{
				sprintf(info, "%d", exchangeInfo->presentGold);
				LabelAtlas* presentGold = dynamic_cast<LabelAtlas*>(parentWidget->getChildByName("ImageView_50_0")->getChildByName("LabelAtlas_171"));
				presentGold->setStringValue(info);
			}
			else{
				parentWidget->removeChild(parentWidget->getChildByName("ImageView_50_0"), true);
			}

			Button* buyButton = dynamic_cast<Button*>(parentWidget->getChildByName(BUY_BUTTON_NAME));
			buyButton->setTag(exchangeInfo->rmb);
			buyButton->addTouchEventListener((CCObject*)this, toucheventselector(StoreScene::touchedCallBack));

			if (exchangeInfo->rmb < 100)
			{
				sprintf(info, "%01.2f", (float)exchangeInfo->rmb / 100);
			}
			else{
				sprintf(info, "%d", exchangeInfo->rmb / 100);
			}

			Label* rmbLabel = dynamic_cast<Label*>(buyButton->getChildByName("Label_41_0_0"));
			rmbLabel->setText(info);
			itemListListView->pushBackCustomItem(itemWidget);
            itemStartX += xStep;
        }
        this->addWidget(m_widget);
        m_inst = this;
        return true;
    }while(0);
    return false;
}

StoreScene::~StoreScene(void)
{
    m_widget = 0;
    m_inst = 0;
}
unsigned int StoreScene::confirmPayment(int rmb)
{ 
    CCNode* widget = PaymentStoreScene::create(rmb);
    CCDirector::sharedDirector()->getRunningScene()->addChild(widget);
    return rmb;
}

void StoreScene::decidePayWays(int rmb)
{
//     if (STORE_UUCUN == MyGame::getInstance()->m_storeId)
//     {
//         confirmPayment(rmb);
//     }
//     else
    {
        androidPay(rmb);
    } 
}

void StoreScene::touchedCallBack(CCObject* pSender,TouchEventType pType){
    switch(pType){
    case TOUCH_EVENT_BEGAN:
        break;
    case TOUCH_EVENT_MOVED:
        break;
    case TOUCH_EVENT_ENDED:
        {
            Widget* pNode = (Widget*)(pSender);
            if(0 == strcmp(pNode->getName(), CLOSE_BUTTON_TAG_NAME))
            {
                this->removeFromParentAndCleanup(true);
                GameScene::m_inst->m_spriteLayer->resumeGame();
            }
            else if (0 == strcmp(pNode->getName(), BUY_BUTTON_NAME))
            {
                decidePayWays(pNode->getTag());
            }
            break;
        }
    case TOUCH_EVENT_CANCELED:
        break;
    default:
        break;
    }
}
void StoreScene::androidPay(int rmb)
{
    MyStatisticsLog::sendPayLog(rmb);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台 
    JniMethodInfo minfo;//定义Jni函数信息结构体 
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"channel/Pay","pay", "(I)V");  

    if (!isHave) { 
        CCLog("jni:此函数不存在"); 
    }else{ 
        CCLog("jni:此函数存在"); 
        //调用此函数 
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, rmb); 
        minfo.env->DeleteLocalRef(minfo.classID);
    } 
    CCLog("jni-java函数执行完毕"); 
#endif 
}


bool StoreScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    bool ret = TouchGroup::ccTouchBegan(pTouch, pEvent);
    return true;
}

void StoreScene::_updateMyGoldShow()
{
}

bool GiftStoreScene::init(){
    do{
        CC_BREAK_IF(!TouchGroup::init());
     
        Widget *rootWidget = extension::GUIReader::shareReader()->widgetFromJsonFile(GIFT_UI_PATH);
        CC_BREAK_IF(!rootWidget);
        Button* closeButton = dynamic_cast<Button*>(rootWidget->getChildByName(GIFT_CLOSE_BUTTON_NAME));
        CC_BREAK_IF(!closeButton);
        closeButton->addTouchEventListener((CCObject*)this, toucheventselector(GiftStoreScene::touchedCallBack)); 

        Button* confirmButton = dynamic_cast<Button*>(rootWidget->getChildByName(GIFT_CONFIRM_BUTTON_NAME));
        CC_BREAK_IF(!confirmButton);
        confirmButton->addTouchEventListener((CCObject*)this, toucheventselector(GiftStoreScene::touchedCallBack)); 

        const ExchangeInfo* exchangeInfo = ExchangeDict::getInstance()->getStoreInfo(MyGame::getInstance()->m_storeId)->getGiftInfo();
        if (exchangeInfo)
        {
            char str[128] = {0};
            LabelAtlas* goldLabel = dynamic_cast<LabelAtlas*>(rootWidget->getChildByName("LabelAtlas_162"));
            sprintf(str, "%d", exchangeInfo->gold);
            goldLabel->setStringValue(str);

            LabelAtlas* presentGoldLabel = dynamic_cast<LabelAtlas*>(rootWidget->getChildByName("ImageView_163")->getChildByName("LabelAtlas_164"));
            sprintf(str, "%d", exchangeInfo->presentGold);
            presentGoldLabel->setStringValue(str);

            Label *label = dynamic_cast<Label*>(rootWidget->getChildByName("ImageView_156")->getChildByName("Label_167"));
            sprintf(str, TipsDict::getInstance()->getTips(TipsDict::KEY_UUCUN_SALE_LABEL).c_str(),exchangeInfo->rmb/100);
            label->setText(str);

            confirmButton->setTag(exchangeInfo->rmb);
        }
        this->addWidget(rootWidget);
        return true;
    }while(0);
    return false;
}

GiftStoreScene::~GiftStoreScene(void)
{
}

void GiftStoreScene::touchedCallBack(CCObject* pSender,TouchEventType pType){
    switch(pType){
    case TOUCH_EVENT_BEGAN:
        break;
    case TOUCH_EVENT_MOVED:
        break;
    case TOUCH_EVENT_ENDED:
        {
            Widget* pNode = (Widget*)(pSender);
            if(0 == strcmp(pNode->getName(), GIFT_CLOSE_BUTTON_NAME))
            {
                StoreScene* mallUI = StoreScene::create();
                this->getParent()->addChild(mallUI, this->getZOrder());
                this->removeFromParentAndCleanup(true);
            }
            else if (0 == strcmp(pNode->getName(), GIFT_CONFIRM_BUTTON_NAME))
            {
                StoreScene::androidPay(pNode->getTag());
                this->removeFromParentAndCleanup(true);
                GameScene::m_inst->m_spriteLayer->resumeGame();
                break;
            }
            break;
        }
    case TOUCH_EVENT_CANCELED:
        break;
    default:
        break;
    }
}


bool GiftStoreScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    bool ret = TouchGroup::ccTouchBegan(pTouch, pEvent);
    return true;
}

bool MiniGiftStoreScene::init(){
    do{
        CC_BREAK_IF(!TouchGroup::init());

        Widget *rootWidget;
        rootWidget = extension::GUIReader::shareReader()->widgetFromJsonFile(MINI_GIFT_UI_PATH);
        CC_BREAK_IF(!rootWidget);
        
        Button* closeButton = dynamic_cast<Button*>(rootWidget->getChildByName(MINI_GIFT_CLOSE_BUTTON_NAME));
        CC_BREAK_IF(!closeButton);
        closeButton->addTouchEventListener((CCObject*)this, toucheventselector(MiniGiftStoreScene::touchedCallBack)); 

        Button* confirmButton = dynamic_cast<Button*>(rootWidget->getChildByName(MINI_GIFT_CONFIRM_BUTTON_NAME));
        CC_BREAK_IF(!confirmButton);
        confirmButton->addTouchEventListener((CCObject*)this, toucheventselector(MiniGiftStoreScene::touchedCallBack));

		ImageView* img = dynamic_cast<ImageView*>(rootWidget->getChildByName("ImageView_156"));
		Label* label8 = dynamic_cast<Label*>(img->getChildByName("Label_10"));
		label8->setText("System");
        const ExchangeInfo *exchangeInfo = ExchangeDict::getInstance()->getStoreInfo(MyGame::getInstance()->m_storeId)->getMiniGiftInfo();
        if (exchangeInfo)
        {
             confirmButton->setTag(exchangeInfo->rmb);
        }
        this->addWidget(rootWidget);
        return true;
    }while(0);
    return false;
}

MiniGiftStoreScene::~MiniGiftStoreScene(void)
{
}

void MiniGiftStoreScene::touchedCallBack(CCObject* pSender,TouchEventType pType){
    switch(pType){
    case TOUCH_EVENT_BEGAN:
        break;
    case TOUCH_EVENT_MOVED:
        break;
    case TOUCH_EVENT_ENDED:
        {
            Widget* pNode = (Widget*)(pSender);
            if(0 == strcmp(pNode->getName(), MINI_GIFT_CLOSE_BUTTON_NAME))
            {
                this->removeFromParentAndCleanup(true);
                GameScene::m_inst->m_spriteLayer->resumeGame();
            }
            else if (0 == strcmp(pNode->getName(), MINI_GIFT_CONFIRM_BUTTON_NAME))
            {   
                StoreScene::androidPay(pNode->getTag());
                this->removeFromParentAndCleanup(true);
                GameScene::m_inst->m_spriteLayer->resumeGame();
                break;
            }
            break;
        }
    case TOUCH_EVENT_CANCELED:
        break;
    default:
        break;
    }
}

bool MiniGiftStoreScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    bool ret = TouchGroup::ccTouchBegan(pTouch, pEvent);
    return true;
}

CCNode *PaymentStoreScene::create(int rmb)
{
    PaymentStoreScene *pRet = new PaymentStoreScene();
    if (pRet && pRet->init(rmb))
    { 
      pRet->autorelease(); 
      return pRet; 
    } 
    else 
    { 
      delete pRet; 
      pRet = NULL; 
      return NULL; 
    } 
}

bool PaymentStoreScene::init(int rmb)
{
    do 
    {
        CC_BREAK_IF(!TouchGroup::init());
        m_rmb=rmb;
        const ExchangeInfo* exchangeInfo = ExchangeDict::getInstance()->getStoreInfo(MyGame::getInstance()->m_storeId)->getExchangeInfo(m_rmb);
        int allGold = exchangeInfo->gold + exchangeInfo->presentGold;
        Widget* payWidget = extension::GUIReader::shareReader()->widgetFromJsonFile(PAY_UI_PATH); 
        Label* label_1 = dynamic_cast<Label*>(payWidget->getChildByName("ImageView_100")->getChildByName("Label_104_0"));
        char str[128] = {0};
        sprintf(str, TipsDict::getInstance()->getTips(TipsDict::KEY_UUCUN_PAYUP_LABEL).c_str(), allGold);
        label_1->setText(str);
        Label* label_2 = dynamic_cast<Label*>(payWidget->getChildByName("ImageView_100")->getChildByName("Label_104")); 
        sprintf(str, TipsDict::getInstance()->getTips(TipsDict::KEY_UUCUN_PAYDOWN_LABEL).c_str(), m_rmb/100);
        label_2->setText(str);

        Button* buyButton = dynamic_cast<Button*>(payWidget->getChildByName(UUCUN_BUY_BUTTON));
        buyButton->addTouchEventListener((CCObject*)this, toucheventselector(PaymentStoreScene::touchedCallBack));
        buyButton->setTag(exchangeInfo->rmb);
        Button* cancelButton = dynamic_cast<Button*>(payWidget->getChildByName(CONCEL_PAY_BUTTON));
        cancelButton->addTouchEventListener((CCObject*)this,toucheventselector(PaymentStoreScene::touchedCallBack));
        
        this->addWidget(payWidget);  

        return true;
    } while (0);
    return false;
}

void PaymentStoreScene::touchedCallBack(CCObject* pSender,TouchEventType pType)
{
    switch(pType){
    case TOUCH_EVENT_BEGAN:
        break;
    case TOUCH_EVENT_MOVED:
        break;
    case TOUCH_EVENT_ENDED:
        {
            Widget* pNode = (Widget*)(pSender);
            if (0 == strcmp(pNode->getName(),CONCEL_PAY_BUTTON))
            {
                this->removeFromParentAndCleanup(true);
            }
            else if (0 == strcmp(pNode->getName(),UUCUN_BUY_BUTTON))
            {
                StoreScene::androidPay(m_rmb);
            }
            break;
        }
    case TOUCH_EVENT_CANCELED:
        break;
    default:
        break;
    }
}
bool PaymentStoreScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    bool ret = TouchGroup::ccTouchBegan(pTouch, pEvent);
    return true;
}
