#include "fishDict.h"
#include "CommonFunc.h"
#include "support/tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"

#define XML_PATH      "res/fishes.xml"

using namespace std;

FishDict* FishDict::m_dict = 0;

FishDict* FishDict::getInstance()
{
    if (!m_dict)
    {
        m_dict = new FishDict();
        m_dict->loadInfos();
    }
    return m_dict;
}

FishDict::FishDict(void)
{
}

FishDict::~FishDict(void)
{
    for (map<int, FishConfig*>::const_iterator iter = m_fishesMap.begin(); iter != m_fishesMap.end(); iter++)
    {
        delete (*iter).second;
    }
    m_fishesMap.clear();
}

int FishDict::loadInfos()
{
    tinyxml2::XMLDocument doc;
    if(!CommonFunc::loadInnerDictFile(doc, XML_PATH)){
        return 1;
    }
    tinyxml2::XMLElement * curNode = doc.RootElement()->FirstChildElement("data");
    while(curNode){
        FishConfig *config = new FishConfig();
        config->id = curNode->IntAttribute("id");
        config->name = curNode->Attribute("name");
        config->exp = curNode->IntAttribute("exp");
        config->gold = curNode->IntAttribute("gold");
        config->png_path = curNode->Attribute("png_path");
        config->plist_path = curNode->Attribute("plist_path");
        config->normal_name = curNode->Attribute("normal_name");
        config->normal_count = curNode->IntAttribute("normal_count");
        config->dead_name = curNode->Attribute("dead_name");
        config->dead_count = curNode->IntAttribute("dead_count");
        config->isSmall = 0 != curNode->IntAttribute("isSmall");
        config->isThunder = 0 != curNode->IntAttribute("thunder");
        config->speed = curNode->FloatAttribute("speed");
        config->accelerateSpeed = curNode->FloatAttribute("accelerateSpeed");
        config->decelerateSpeed = curNode->FloatAttribute("decelerateSpeed");
        config->maxNum = curNode->IntAttribute("maxNum");
        config->probability = curNode->IntAttribute("probability");
        config->showDeadTips = curNode->IntAttribute("showDeadTips");
        const char* s = curNode->Attribute("deadSound");
        if (s)
        {
            config->deadSound = string(s);
        }else{
            config->deadSound = string("");
        }
        for (int i=0;i<config->probability;i++)
        {
            m_probabilityFishIds.push_back(config->id);
        }
        m_fishesMap.insert(pair<int,FishConfig*>(config->id, config));
        curNode = curNode->NextSiblingElement("data");
    }
    return 0;
}
const FishConfig* FishDict::getFishConfig(int id) const{
    map<int, FishConfig*>::const_iterator iter = m_fishesMap.find(id);
    if (iter == m_fishesMap.end())
    {
        return 0;
    }else{
        return (*iter).second;
    }
}

int FishDict::getRandomFishId() const{
    int index = abs(rand()) % m_probabilityFishIds.size();
    return m_probabilityFishIds[index];
}
