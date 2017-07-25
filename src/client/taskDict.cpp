#include "taskDict.h"
#include "CommonFunc.h"
#include "support/tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"

#define XML_PATH      "res/task.xml"

using namespace std;

TaskDict* TaskDict::m_dict = 0;

TaskDict* TaskDict::getInstance()
{
    if (!m_dict)
    {
        m_dict = new TaskDict();
        m_dict->loadInfos();
    }
    return m_dict;
}

TaskDict::TaskDict(void)
{
}

TaskDict::~TaskDict(void)
{
}

int TaskDict::loadInfos()
{
    tinyxml2::XMLDocument doc;
    if(!CommonFunc::loadInnerDictFile(doc, XML_PATH)){
        return 1;
    }
    tinyxml2::XMLElement * curNode = doc.RootElement()->FirstChildElement("data");
    while(curNode){
        TaskInfo *info = new TaskInfo();
        info->id = curNode->IntAttribute("taskId");
        info->taskName = curNode->Attribute("taskName");
        info->evenType = curNode->IntAttribute("eventType");
        if (1 == info->evenType)
        {
            const char *fishIdInfo = curNode->Attribute("fish_id");
            const char *spl = ",";
            char *p;
            char str[64]={0};
            sprintf(str, fishIdInfo);
            p = strtok(str, spl);
            while(p)
            {
                int fishId=0;
                TIXML_SSCANF(p,"%d",&fishId);
                info->fishIds.push_back(fishId);
                p=strtok(NULL,spl);
            }
        }
        info->taskTime = curNode->IntAttribute("taskTime");
        info->needCount = curNode->IntAttribute("needNum");
        info->rewardGold = curNode->IntAttribute("rewardGold");
        info->rewardExp = curNode->IntAttribute("rewardExp");

        vector<TaskInfo*>::iterator iter = m_taskInfos.begin();
        for (; iter != m_taskInfos.end(); iter++)
        {
            if ((*iter)->id > info->id)
            {
                break;
            }
        }
        m_taskInfos.insert(iter, 1, info);
        curNode = curNode->NextSiblingElement("data");
    }
    return 0;
}

const TaskInfo* TaskDict::firstTaskInfo() const{
    return m_taskInfos.front();
}

const TaskInfo* TaskDict::nextTaskInfo(int id) const{
    int index = abs(rand()) % m_taskInfos.size();
    return m_taskInfos[index];
//     vector<TaskInfo*>::const_iterator iter = m_taskInfos.begin();
//     for (; iter != m_taskInfos.end(); iter++)
//     {
//         if (id == (*iter)->id)
//         {
//             break;
//         }
//     }
//     if (iter == m_taskInfos.end())
//     {
//         return firstTaskInfo();
//     }
//     iter++;
//     if (iter == m_taskInfos.end())
//     {
//         return firstTaskInfo();
//     }
//     return (*iter);
}

const TaskInfo* TaskDict::getTaskInfo(int id) const{
    for (vector<TaskInfo*>::const_iterator iter = m_taskInfos.begin(); iter != m_taskInfos.end(); iter++)
    {
        if (id == (*iter)->id)
        {
            return (*iter);
        }
    }
    return 0;
}
