#include "checkinLayer.h"
#include "cocos-ext.h"
#include "myGame.h"

using namespace cocos2d;
using namespace cocos2d::gui;

static const char* CONFIRM_BUTTON_NAME = "Button_94";

CheckinLayer* CheckinLayer::create(int dayCount, int maxDayCount, int gold){
    CheckinLayer* pRet = new CheckinLayer();
    if (pRet && pRet->init(dayCount, maxDayCount, gold)) 
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

bool CheckinLayer::init(int dayCount, int maxDayCount, int gold){
    do{
        CC_BREAK_IF(!TouchGroup::init());
        Widget *widget = extension::GUIReader::shareReader()->widgetFromJsonFile("ui/game/meiridengrujiem_1.ExportJson");
        CC_BREAK_IF(!widget);
        this->addWidget(widget);

        int x = 305;
        int y = 289;
        for (int i=0;i<maxDayCount; i++)
        {
            char *path;
            if (i < dayCount)
            {
                path = "ui/game/meiridengrujiem/xingx01.png";
            }else{
                path = "ui/game/meiridengrujiem/xingx02.png";
            }
            ImageView *star = ImageView::create();
            star->loadTexture(path);
            star->setPosition(ccp(x,y));
            widget->addChild(star,100);
            x+=31;
        }

        Button* closeButton = dynamic_cast<Button*>(widget->getChildByName(CONFIRM_BUTTON_NAME));
        CC_BREAK_IF(!closeButton);
        closeButton->addTouchEventListener((CCObject*)this, toucheventselector(CheckinLayer::touchedCallBack));

        LabelAtlas* dayLabel = dynamic_cast<LabelAtlas*>(widget->getChildByName("LabelAtlas_92"));
        char str[64] ={};
        sprintf(str, "%d", dayCount);
        dayLabel->setStringValue(str);

        LabelAtlas* goldLabel = dynamic_cast<LabelAtlas*>(widget->getChildByName("LabelAtlas_93"));
        sprintf(str, "%d", gold);
        goldLabel->setStringValue(str);

        return true;
    }while(0);
    return false;
}

CheckinLayer::~CheckinLayer(void)
{
}

void CheckinLayer::touchedCallBack(cocos2d::CCObject* pSender, cocos2d::gui::TouchEventType pType){
    switch(pType){
    case TOUCH_EVENT_BEGAN:
        break;
    case TOUCH_EVENT_MOVED:
        break;
    case TOUCH_EVENT_ENDED:
        {
            Widget* pNode = (Widget*)(pSender);
            if(0 == strcmp(pNode->getName(), CONFIRM_BUTTON_NAME))
            {
                MyGame::getInstance()->getCheckinReward();
                MyStatisticsLog::sendCheckinLog();
                this->removeFromParentAndCleanup(true);
            }
            break;
        }
    case TOUCH_EVENT_CANCELED:
        break;
    default:
        break;
    }
}

bool CheckinLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    bool ret = TouchGroup::ccTouchBegan(pTouch, pEvent);
    return true;
}