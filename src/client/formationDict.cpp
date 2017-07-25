#include "formationDict.h"
#include "CommonFunc.h"
#include "support/tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"
#include "myGame.h"

#define FORMATION_LINE_XML_PATH      "res/line.xml"
#define FORMATION_XML_PATH      "res/formation.xml"

using namespace std;

FormationDict* FormationDict::m_dict = 0;

void FormationLine::makeRandomBeginPosition(cocos2d::CCPoint &point) const{
    const int topGap = 100;
    const int fishWidthHalf = 150;
    int x,y;
    switch(beginArea){
    case 0:
        x = 0-fishWidthHalf;
        y = abs(rand() % topGap);
        break;
    case 1:
        x = 0-fishWidthHalf;
        y = abs(rand() % (MyGame::DEFAULT_STANDARD_HEIGHT - topGap * 2)) + topGap;
        break;
    case 2:
        x = 0-fishWidthHalf;
        y = MyGame::DEFAULT_STANDARD_HEIGHT - abs(rand() % topGap);
        break;
    case 3:
        x = abs(rand() % MyGame::DEFAULT_STANDARD_WIDTH);
        y = MyGame::DEFAULT_STANDARD_HEIGHT+fishWidthHalf;
        break;
    case 4:
        x = MyGame::DEFAULT_STANDARD_WIDTH+fishWidthHalf;
        y = abs(rand() % topGap);
        break;
    case 5:
        x = MyGame::DEFAULT_STANDARD_WIDTH+fishWidthHalf;
        y = abs(rand() % (MyGame::DEFAULT_STANDARD_HEIGHT - topGap * 2)) + topGap;
        break;
    case 6:
        x = MyGame::DEFAULT_STANDARD_WIDTH+fishWidthHalf;
        y = MyGame::DEFAULT_STANDARD_HEIGHT - abs(rand() % topGap);
        break;
    case 7:
        x = abs(rand() % MyGame::DEFAULT_STANDARD_WIDTH);
        y = 0-fishWidthHalf;
        break;
    default:
        x=y=0;
        CCAssert(false,"");
        break;
    }
    point.x=x;
    point.y=y;
}

FormationDict* FormationDict::getInstance()
{
    if (!m_dict)
    {
        m_dict = new FormationDict();
        m_dict->loadInfos();
    }
    return m_dict;
}

FormationDict::FormationDict(void)
{
}

FormationDict::~FormationDict(void)
{
    for (map<int, FormationLine*>::const_iterator iter = m_linesMap.begin(); iter != m_linesMap.end(); iter++)
    {
        delete (*iter).second;
    }
    m_linesMap.clear();
    for (map<int, FormationConfig*>::const_iterator iter = m_formationsMap.begin(); iter != m_formationsMap.end(); iter++)
    {
        delete (*iter).second;
    }
    m_formationsMap.clear();
}

void FormationDict::printMap(){
    using namespace cocos2d;
    for (map<int, FormationConfig*>::const_iterator iter = m_formationsMap.begin(); iter != m_formationsMap.end(); iter++)
    {
        FormationConfig *config = (*iter).second;
        CCLog("map id:%d", config->m_id);
        for (vector<FormationFish*>::iterator fishIter = config->m_fishes.begin(); fishIter != config->m_fishes.end(); fishIter++)
        {
            FormationFish *fish = *fishIter;
            CCLog("fishId:%d\t isLine:%d\t beginX:%d beginY:%d bControlX:%d bControlY:%d endX:%d endY:%d eControlX:%d eControlY:%d delay:%f", fish->fishId, fish->type, fish->beginPoint.x, fish->beginPoint.y, fish->line->bControlPoint.x, fish->line->bControlPoint.y, fish->line->endPoint.x, fish->line->endPoint.y, fish->line->eControlPoint.x, fish->line->eControlPoint.y, fish->delay);
        }
    }
}

inline bool checkPoint(cocos2d::CCPoint &point){
    return abs((int)(point.x)) < 2000 && abs((int)(point.y)) < 2000;
}

int FormationDict::loadInfos()
{
    tinyxml2::XMLDocument lineDoc;
    if(!CommonFunc::loadInnerDictFile(lineDoc, FORMATION_LINE_XML_PATH)){
        return 1;
    }
    tinyxml2::XMLElement * curNode = lineDoc.RootElement()->FirstChildElement("data");
    while(curNode){
        FormationLine *config = new FormationLine();
        config->id = curNode->IntAttribute("id");
        config->beginArea = curNode->Attribute("beginArea")[0] - 'a';
        config->type = curNode->IntAttribute("type");
        const char *beginPoint = curNode->Attribute("beginPoint");
        const char *endPoint = curNode->Attribute("endPoint");
        int beginX, beginY;
        TIXML_SSCANF(beginPoint,"%d,%d",&beginX,&beginY);
        TIXML_SSCANF(endPoint,"%f,%f",&config->endPoint.x,&config->endPoint.y);
        if (!checkPoint(config->endPoint))
        {
            CCAssert(false, "");
        }
        config->endPoint.x-=beginX;
        config->endPoint.y-=beginY;
        if (0 != config->type)
        {
            const char *bControlPoint = curNode->Attribute("bControlPoint");
            const char *eControlPoint = curNode->Attribute("eControlPoint");
            TIXML_SSCANF(bControlPoint,"%f,%f",&config->bControlPoint.x,&config->bControlPoint.y);
            TIXML_SSCANF(eControlPoint,"%f,%f",&config->eControlPoint.x,&config->eControlPoint.y);
            if (!checkPoint(config->bControlPoint) || !checkPoint(config->eControlPoint))
            {
                CCAssert(false, "");
            }
            config->bControlPoint.x -= beginX;
            config->bControlPoint.y-=beginY;
            config->eControlPoint.x-=beginX;
            config->eControlPoint.y-=beginY;
        }
        if (2 == config->type)
        {
            const char *beginPoint2 = curNode->Attribute("beginPoint2");
            const char *bControlPoint2 = curNode->Attribute("bControlPoint2");
            const char *endPoint2 = curNode->Attribute("endPoint2");
            const char *eControlPoint2 = curNode->Attribute("eControlPoint2");
            int beginX2, beginY2;
            TIXML_SSCANF(beginPoint2,"%d,%d",&beginX2,&beginY2);
            TIXML_SSCANF(bControlPoint2,"%f,%f",&config->bControlPoint2.x,&config->bControlPoint2.y);
            TIXML_SSCANF(endPoint2,"%f,%f",&config->endPoint2.x,&config->endPoint2.y);
            TIXML_SSCANF(eControlPoint2,"%f,%f",&config->eControlPoint2.x,&config->eControlPoint2.y);
            if (!checkPoint(config->bControlPoint2) || !checkPoint(config->endPoint2) || !checkPoint(config->eControlPoint2))
            {
                CCAssert(false, "");
            }
            config->bControlPoint2.x-=beginX2;
            config->bControlPoint2.y-=beginY2;
            config->endPoint2.x-=beginX2;
            config->endPoint2.y-=beginY2;
            config->eControlPoint2.x-=beginX2;
            config->eControlPoint2.y-=beginY2;
        }
        m_linesMap.insert(pair<int,FormationLine*>(config->id, config));
        const char *purpose = curNode->Attribute("purpose");
        if (0 == strcmp(purpose, "general"))
        {
            m_bigFishLines.push_back(config);
            m_smallFishLines.push_back(config);
        }else if (0 == strcmp(purpose, "fishArray"))
        {
        }else if (0 == strcmp(purpose, "bigFish"))
        {
            m_bigFishLines.push_back(config);
        }else if (0 == strcmp(purpose, "smallFish"))
        {
            m_smallFishLines.push_back(config);
        }else{
            CCAssert(false, "");
        }
        curNode = curNode->NextSiblingElement("data");
    }

    tinyxml2::XMLDocument formationDoc;
    if(!CommonFunc::loadInnerDictFile(formationDoc, FORMATION_XML_PATH)){
        return 1;
    }
    curNode = formationDoc.RootElement()->FirstChildElement("data");
    while(curNode){
        int id = curNode->IntAttribute("id");
        int lineId = curNode->IntAttribute("lineId");
        int fishId = curNode->IntAttribute("fishId");
        const char *beginPoint = curNode->Attribute("beginPoint");
        float delay = curNode->FloatAttribute("delay");
        bool isMiniFormation = 0 != curNode->IntAttribute("fishArraytype");

        const FormationLine *line = getLine(lineId);
        FormationFish *fishConfig = new FormationFish();
        fishConfig->fishId = fishId;
        fishConfig->type = line->type;
        TIXML_SSCANF(beginPoint,"%f,%f",&fishConfig->beginPoint.x,&fishConfig->beginPoint.y);
        fishConfig->line = line;
        fishConfig->delay=delay;

        FormationConfig* config = findFormationConfig(id);
        if (0==config)
        {
            config = new FormationConfig();
            config->m_id=id;
            m_formationsMap.insert(pair<int,FormationConfig*>(config->m_id, config));
            if (isMiniFormation)
            {
                m_miniFormations.push_back(config);
            }else{
                m_sceneFormations.push_back(config);
            }
        }
        vector<FormationFish*>::iterator iter = config->m_fishes.begin();
        for (; iter != config->m_fishes.end(); iter++)
        {
            float d = (*iter)->delay;
            if (d > fishConfig->delay)
            {
                break;
            }
        }
        config->m_fishes.insert(iter, 1, fishConfig);
        curNode = curNode->NextSiblingElement("data");
    }
    //printMap();
    return 0;
}

const FormationLine *FormationDict::randomBigFishLine() const{
    int index = abs(rand()) % m_bigFishLines.size();
    return m_bigFishLines[index];
}

const FormationLine *FormationDict::randomSmallFishLine() const{
    int index = abs(rand()) % m_smallFishLines.size();
    return m_smallFishLines[index];
}

const FormationLine* FormationDict::getLine(int id) const {
    map<int, FormationLine*>::const_iterator iter = m_linesMap.find(id);
    if (iter == m_linesMap.end())
    {
        return 0;
    }else{
        return (*iter).second;
    }
}

const FormationConfig* FormationDict::randomFormationConfig() const{
    int index = rand() % m_sceneFormations.size();
    return m_sceneFormations[index];
}

const FormationConfig* FormationDict::randomMiniFormationConfig() const{
    int index = rand() % m_miniFormations.size();
    return m_miniFormations[index];
}

FormationConfig* FormationDict::findFormationConfig(int id) const{
    map<int, FormationConfig*>::const_iterator iter = m_formationsMap.find(id);
    if (iter == m_formationsMap.end())
    {
        return 0;
    }else{
        return (*iter).second;
    }
}
