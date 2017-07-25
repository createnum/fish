#include "taskUI.h"
#include "taskDict.h"
#include <string>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::gui;

static const char *TASK_CONTROL_BUTTON = "Button_26";
static const int SHOW_TASK_END_AMN_TIME = 20;

TaskUI* TaskUI::create(int taskId){
    TaskUI* pRet = new TaskUI();
    if (pRet && pRet->init(taskId))
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

bool TaskUI::init(int taskId){
    do{
        CC_BREAK_IF(!TouchGroup::init());

        m_currentTask = TaskDict::getInstance()->getTaskInfo(taskId);
        CC_BREAK_IF(!m_currentTask);
        m_end = false;
        m_success = false;

        const float duration = 0.5f;
        const float minScale = 0.8f;
        const float maxScale = 1.2f;
        const string uiPath = "ui/game/renwutishijiemian_2.ExportJson";
        m_widget = extension::GUIReader::shareReader()->widgetFromJsonFile(uiPath.c_str());
        CC_BREAK_IF(!m_widget);
        this->addWidget(m_widget);

        Button* controlButton = dynamic_cast<Button*>(m_widget->getChildByName(TASK_CONTROL_BUTTON));
        controlButton->addTouchEventListener(this, toucheventselector(TaskUI::touchedCallBack));
        CCAction *action = CCSequence::create(CCScaleTo::create(duration, minScale), CCScaleTo::create(duration, maxScale), CCScaleTo::create(duration, minScale), CCScaleTo::create(duration, maxScale), CCScaleTo::create(duration, 1.0f), NULL);
        controlButton->runAction(action);

        Widget* parentWidget = m_widget->getChildByName("Panel_27");
        ImageView* iconView = dynamic_cast<ImageView*>(parentWidget->getChildByName("ImageView_35"));

        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

        string plistPath = "ui/game/taskFishes0.plist";
        string pngPath = "ui/game/taskFishes0.png";
        cache->addSpriteFramesWithFile(plistPath.c_str(),pngPath.c_str());
        if (1==m_currentTask->evenType)
        {
            char str[128] = {0};
            sprintf(str, "taskFish_%d.png", m_currentTask->fishIds.front());
            iconView->loadTexture(str,UI_TEX_TYPE_PLIST);
        }else{
            iconView->loadTexture("taskFish_yubi.png",UI_TEX_TYPE_PLIST);
        }
        cache->removeSpriteFramesFromFile(plistPath.c_str());
        Label* taskName = dynamic_cast<Label*>(parentWidget->getChildByName("Label_29"));
        taskName->setText(m_currentTask->taskName);
        Label* rewardGold = dynamic_cast<Label*>(parentWidget->getChildByName("Label_29_3"));
        char str[128] = {0};
        sprintf(str, "+%d", m_currentTask->rewardGold);
        rewardGold->setText(str);
        Label* rewardExp = dynamic_cast<Label*>(parentWidget->getChildByName("Label_29_4"));
        sprintf(str, "+%d", m_currentTask->rewardExp);
        rewardExp->setText(str);

        this->setRemainTime(m_currentTask->taskTime);
        this->setSuccessCount(0);
        return true;
    }while(0);
    return false;
}

TaskUI::~TaskUI(void)
{
    m_widget = 0;
}

void TaskUI::update(float duration){
    if (m_end)
    {
        return;
    }
    setRemainTime(m_taskCurrentRemainTime-duration);
}

void TaskUI::setRemainTime(float time){
    if (m_end)
    {
        return;
    }
    if (time<=0)
    {
        taskEnd(false); 
    }
    bool showEndAmn = !needShowTaskEndAmn(m_taskCurrentRemainTime) && needShowTaskEndAmn(time);
    m_taskCurrentRemainTime = time;

    Widget* parentWidget = m_widget->getChildByName("Panel_27");
    Label* remainTime = dynamic_cast<Label*>(parentWidget->getChildByName("Label_28"));
    char str[128] = {0};
    int second = ((int) m_taskCurrentRemainTime);
    if (showEndAmn)
    {
        const float duration = 0.5f;
        const float minScale = 0.8f;
        const float maxScale = 1.2f;
        parentWidget->stopAllActions();
        parentWidget->setPosition(ccp(23,262));
        parentWidget->setScale(1.0f);
        Button* controlButton = dynamic_cast<Button*>(m_widget->getChildByName(TASK_CONTROL_BUTTON));
        controlButton->addTouchEventListener(0,0);
        CCActionInterval *action = CCSequence::create(CCScaleTo::create(duration, minScale), CCScaleTo::create(duration, maxScale), NULL);
        CCRepeatForever *repeat = CCRepeatForever::create(action);
        controlButton->runAction(repeat);
    }
    int min = second / 60;
    second %= 60;
    int sumSecond = m_currentTask->taskTime;
    int sumMin = sumSecond / 60;
    sumSecond %= 60;
    sprintf(str, "%02d:%02d/%02d:%02d", min, second, sumMin, sumSecond);
    remainTime->setText(str);

}

void TaskUI::setSuccessCount(int count){
    if (m_end)
    {
        return;
    }
    m_taskCurrentSuccessCount= count;
    if (m_taskCurrentSuccessCount >= m_currentTask->needCount)
    {
        taskEnd(true);
        return;
    }
    Widget* parentWidget = m_widget->getChildByName("Panel_27");
    Label* successInfo = dynamic_cast<Label*>(parentWidget->getChildByName("Label_29_2"));
    char str[128] = {0};
    sprintf(str, "%d/%d", m_taskCurrentSuccessCount, m_currentTask->needCount);
    successInfo->setText(str);
}

bool TaskUI::catchFish(int fishId){
    if (m_end)
    {
        return false;
    }
    if (1 == m_currentTask->evenType)
    {
        bool find = false;
        for (vector<int>::const_iterator iter = m_currentTask->fishIds.begin(); iter != m_currentTask->fishIds.end(); iter++)
        {
            if ((*iter) < 0 || fishId == (*iter))
            {
                find = true;
                break;
            }
        }
        if (find)
        {
            setSuccessCount(m_taskCurrentSuccessCount + 1);
            return true;
        }
    }
    return false;
}

bool TaskUI::gainGold(int gold){
    if (m_end)
    {
        return false;
    }
    if (1 != m_currentTask->evenType){
        setSuccessCount(m_taskCurrentSuccessCount + gold);
        return true;
    }
    return false;
}

bool TaskUI::needShowTaskEndAmn(float remainTime){
    return remainTime <= SHOW_TASK_END_AMN_TIME;
}

void TaskUI::taskEnd(bool  success){
    m_end = true;
    m_success = success;
}

void TaskUI::touchedCallBack(CCObject* pSender,TouchEventType pType){
    switch(pType){
    case TOUCH_EVENT_BEGAN:
        break;
    case TOUCH_EVENT_MOVED:
        break;
    case TOUCH_EVENT_ENDED:
        {
            Widget *pNode = (Widget*)(pSender);
            cocos2d::CCLog("scheduler : taskUI touchedCallBack (%s)", pNode->getName());
            if ((0 == strcmp(pNode->getName(), TASK_CONTROL_BUTTON)))
            {
                Widget* panelWidget = m_widget->getChildByName("Panel_27");
                if (panelWidget->getScale() < 1)
                {
                    CCActionInterval* scale = CCEaseSineIn::create(CCScaleTo::create(1.0f, 1.0f));
                    CCActionInterval* move = CCEaseSineIn::create(CCMoveTo::create(1.0f, ccp(23,262)));
                    panelWidget->runAction(CCSpawn::createWithTwoActions(scale, move));
                }else{
                    CCActionInterval *scale = CCEaseSineOut::create(CCScaleTo::create(1.0f, 0.0f));
                    CCActionInterval* move = CCEaseSineOut::create(CCMoveTo::create(1.0f, ccp(28,390)));
                    panelWidget->runAction(CCSpawn::createWithTwoActions(scale, move));
                }
            }
            break;
        }
    case TOUCH_EVENT_CANCELED:
        break;
    default:
        break;
    }
}