#ifndef __TASK_UI_H__
#define __TASK_UI_H__
#include "cocos2d.h"
#include "cocos-ext.h"

class TaskInfo;

class TaskUI:public cocos2d::gui::TouchGroup
{
public:
    static TaskUI* create(int taskId);
    ~TaskUI(void);
    bool init(int taskId);
    virtual void update(float delta);
    void setRemainTime(float time);
    void setSuccessCount(int count);
    bool catchFish(int fishId);
    bool gainGold(int gold);
    bool needShowTaskEndAmn(float remainTime);
    void taskEnd(bool  success);
    void touchedCallBack(cocos2d::CCObject* pSender, cocos2d::gui::TouchEventType pType);
    cocos2d::gui::Widget * m_widget;
    const TaskInfo *m_currentTask;
    bool m_end;
    bool m_success;
    float m_taskCurrentRemainTime;//任务结束剩余时间
    int m_taskCurrentSuccessCount;//任务单元已经完成次数
};

#endif //__TASK_UI_H__

