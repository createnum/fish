#include "RewardDict.h"
#include "CommonFunc.h"
#include "support/tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"

#define XML_PATH      "res/reward.xml"

using namespace std;

RewardDict* RewardDict::m_dict = 0;

RewardDict* RewardDict::getInstance()
{
    if (!m_dict)
    {
        m_dict = new RewardDict();
        m_dict->loadInfos();
    }
    return m_dict;
}

RewardDict::RewardDict(void)
{
}

RewardDict::~RewardDict(void)
{
    for (vector<RewardConfig*>::const_iterator iter = m_rewards.begin(); iter != m_rewards.end(); iter++)
    {
        delete (*iter);
    }
    m_rewards.clear();
}

int RewardDict::loadInfos()
{
    tinyxml2::XMLDocument doc;
    if(!CommonFunc::loadInnerDictFile(doc, XML_PATH)){
        return 1;
    }
    tinyxml2::XMLElement * curNode = doc.RootElement()->FirstChildElement("data");
    while(curNode){
        RewardConfig *config = new RewardConfig();
        config->orderId = curNode->IntAttribute("orderId");
        config->waitTime = curNode->IntAttribute("waitTime");
        config->rewardGold = curNode->IntAttribute("rewardGold");
        vector<RewardConfig*>::iterator iter = m_rewards.begin();
        for (; iter != m_rewards.end(); iter++)
        {
            if ((*iter)->orderId > config->orderId)
            {
                break;
            }
        }
        m_rewards.insert(iter, 1, config);
        curNode = curNode->NextSiblingElement("data");
    }
    return 0;
}

const RewardConfig* RewardDict::nextRewardConfig(int id) const{
    vector<RewardConfig*>::const_iterator iter = m_rewards.begin();
    for (; iter != m_rewards.end(); iter++)
    {
        if (id == (*iter)->orderId)
        {
            break;
        }
    }
    if (iter == m_rewards.end())//not find
    {
        return m_rewards[0];
    }
    ++iter;
    if (iter == m_rewards.end())//last
    {
        return NULL;
    }else{
        return *iter;
    }
}

