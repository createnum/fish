#include "levelDict.h"
#include "CommonFunc.h"
#include "support/tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"

#define XML_PATH      "res/level.xml"

using namespace std;

LevelDict* LevelDict::m_dict = 0;

LevelDict* LevelDict::getInstance()
{
    if (!m_dict)
    {
        m_dict = new LevelDict();
        m_dict->loadInfos();
    }
    return m_dict;
}

LevelDict::LevelDict(void)
{
}

LevelDict::~LevelDict(void)
{
    for (map<int, LevelConfig*>::const_iterator iter = m_fishesMap.begin(); iter != m_fishesMap.end(); iter++)
    {
        delete (*iter).second;
    }
    m_fishesMap.clear();
}

int LevelDict::loadInfos()
{
    tinyxml2::XMLDocument doc;
    if(!CommonFunc::loadInnerDictFile(doc, XML_PATH)){
        return 1;
    }
    tinyxml2::XMLElement * curNode = doc.RootElement()->FirstChildElement("data");
    while(curNode){
        LevelConfig *config = new LevelConfig();
        config->level = curNode->IntAttribute("level");
        config->upgradeExp = curNode->IntAttribute("upgradeExp");
        config->showName = curNode->Attribute("showName");
        config->bombCount = curNode->IntAttribute("bombCount");
        config->rewardGold = curNode->IntAttribute("rewardGold");
        const char *cannon = curNode->Attribute("cannonAdd");
        config->cannonAdd = 0 == cannon ? string("") : string(cannon);
        m_fishesMap.insert(pair<int,LevelConfig*>(config->level, config));
        curNode = curNode->NextSiblingElement("data");
    }
    return 0;
}
const LevelConfig* LevelDict::getLevelConfig(int id) const{
    map<int, LevelConfig*>::const_iterator iter = m_fishesMap.find(id);
    if (iter == m_fishesMap.end())
    {
        return 0;
    }else{
        return (*iter).second;
    }
}
