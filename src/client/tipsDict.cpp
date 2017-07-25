#include "tipsDict.h"
#include "CommonFunc.h"
#include "support/tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"

#define XML_PATH      "res/tips.xml"

using namespace std;

TipsDict* TipsDict::m_dict = 0;

TipsDict* TipsDict::getInstance()
{
    if (!m_dict)
    {
        m_dict = new TipsDict();
        m_dict->loadInfos();
    }
    return m_dict;
}

TipsDict::TipsDict(void)
{
}

TipsDict::~TipsDict(void)
{
}

int TipsDict::loadInfos()
{
    tinyxml2::XMLDocument doc;
    if(!CommonFunc::loadInnerDictFile(doc, XML_PATH)){
        return 1;
    }
    tinyxml2::XMLElement * curNode = doc.RootElement()->FirstChildElement("data");
    while(curNode){
        int key = curNode->IntAttribute("key");
        string value = curNode->Attribute("value");
        m_tipsMap.insert(pair<int,string>(key,value));
        curNode = curNode->NextSiblingElement("data");
    }
    return 0;
}

const std::string& TipsDict::getTips(int key) const{
    map<int, string>::const_iterator iter = m_tipsMap.find(key);
    if (iter == m_tipsMap.end())  
    {
        return "";
    }else{
        return (*iter).second;
    }
}
